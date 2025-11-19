#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "billing.h"
#include "inventory.h"
#include "utils.h"

/* Simple queue for customers waiting to purchase */
typedef struct Customer {
    int id;
    int qty;
    struct Customer *next;
} Customer;

static Customer *q_front = NULL, *q_rear = NULL;

/* Enqueue */
static void enqueue(int id, int qty) {
    Customer *c = (Customer*)malloc(sizeof(Customer));
    c->id = id; 
    c->qty = qty; 
    c->next = NULL;

    if (!q_rear) 
        q_front = q_rear = c;
    else { 
        q_rear->next = c; 
        q_rear = c; 
    }
}

/* Dequeue; returns 1 if dequeued, 0 if empty */
static int dequeue(int *id, int *qty) {
    if (!q_front) return 0;
    Customer *t = q_front;
    *id = t->id; 
    *qty = t->qty;

    q_front = q_front->next;
    if (!q_front) q_rear = NULL;

    free(t);
    return 1;
}

/* Visualize queue */
static void printQueueState() {
    printf("\n--- Billing Queue State ---\n");
    if (!q_front) { 
        printf("(empty)\n"); 
        return; 
    }
    Customer *cur = q_front;
    int pos = 1;
    while (cur) {
        printf("%d) ItemID:%d Qty:%d\n", pos++, cur->id, cur->qty);
        cur = cur->next;
    }
    printf("---------------------------\n");
}

/* Billing */
void billing() {
    int id, qty;

    printf("Enter item ID to buy (will be queued): ");
    if (scanf("%d", &id) != 1) { 
        printf("Invalid input\n"); 
        while (getchar()!='\n');
        return; 
    }
    if (id < 0) {
        printf("❌ ID cannot be negative.\n");
        return;
    }

    printf("Enter quantity: ");
    if (scanf("%d", &qty) != 1) { 
        printf("Invalid input\n"); 
        while (getchar()!='\n');
        return; 
    }
    if (qty < 0) {
        printf("❌ Quantity cannot be negative.\n");
        return;
    }

    enqueue(id, qty);
    printf("Added to billing queue. Current queue:\n");
    printQueueState();

    /* Process queue FIFO */
    int pid, pqty;
    printf("\nProcessing queue...\n");

    while (dequeue(&pid, &pqty)) {

        printf("Processing customer for Item ID=%d Qty=%d\n", pid, pqty);

        FILE *fp = fopen("inventory.dat", "rb");
        FILE *temp = fopen("temp.dat", "wb");
        int found = 0;

        if (!fp || !temp) { 
            printf("File error!\n"); 
            if (fp) fclose(fp);
            if (temp) fclose(temp);
            continue; 
        }

        Item it;
        while (fread(&it, sizeof(Item), 1, fp)) {

            if (it.id == pid) {
                found = 1;

                if (pqty > it.quantity) {
                    printf("❌ Not enough stock for ID %d (available %d). Skipping.\n",
                           pid, it.quantity);
                } 
                else {
                    float total = it.price * pqty;
                    printf("💰 Customer Total = ₹%.2f\n", total);

                    it.quantity -= pqty;

                    /* Record sale */
                    FILE *fs = fopen("sales.dat", "ab");
                    if (fs) {
                        Sale sale;
                        sale.id = it.id;
                        strncpy(sale.name, it.name, sizeof(sale.name));
                        sale.qty_sold = pqty;
                        sale.total_price = total;
                        fwrite(&sale, sizeof(Sale), 1, fs);
                        fclose(fs);
                    }
                }
            }

            fwrite(&it, sizeof(Item), 1, temp);
        }

        fclose(fp);
        fclose(temp);

        if (found) {
            remove("inventory.dat");
            rename("temp.dat", "inventory.dat");

            freeInventory();
            loadInventory();

            printf("✅ Processed and inventory updated.\n");
        } 
        else {
            remove("temp.dat");
            printf("❌ Item ID %d not found in inventory.\n", pid);
        }
    }

    printf("All queued customers processed. Final queue state:\n");
    printQueueState();
}
