
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "billing.h"
#include "inventory.h"

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
    c->id = id; c->qty = qty; c->next = NULL;
    if (!q_rear) q_front = q_rear = c;
    else { q_rear->next = c; q_rear = c; }
}

/* Dequeue; returns 1 if dequeued into provided pointers, 0 if empty */
static int dequeue(int *id, int *qty) {
    if (!q_front) return 0;
    Customer *t = q_front;
    *id = t->id; *qty = t->qty;
    q_front = q_front->next;
    if (!q_front) q_rear = NULL;
    free(t);
    return 1;
}

/* Visualize queue state */
static void printQueueState() {
    printf("\n--- Billing Queue State ---\n");
    if (!q_front) { printf("(empty)\n"); return; }
    Customer *cur = q_front;
    int pos = 1;
    while (cur) {
        printf("%d) ItemID:%d Qty:%d\n", pos++, cur->id, cur->qty);
        cur = cur->next;
    }
    printf("---------------------------\n");
}

/* buyItem now enqueues the request, processes in FIFO order, updates BST and files */
void buyItem() {
    int id, qty;
    printf("Enter item ID to buy (will be queued): ");
    if (scanf("%d", &id) != 1) { printf("Invalid input\n"); return; }
    printf("Enter quantity: ");
    if (scanf("%d", &qty) != 1) { printf("Invalid input\n"); return; }

    enqueue(id, qty);
    printf("Added to billing queue. Current queue:\n");
    printQueueState();

    /* Process queue immediately (simulate cashier processing FIFO) */
    int pid, pqty;
    printf("\nProcessing queue...\n");
    while (dequeue(&pid, &pqty)) {
        /* search BST for item */
        printf("Processing customer for Item ID=%d Qty=%d\n", pid, pqty);
        /* We will use searchItem's internal search logic, but need access to BST's search.
           To avoid exposing internals we perform a file-based read to find and update.
           Simpler approach: read inventory.dat fully into memory, update, and rewrite */
        FILE *fp = fopen("inventory.dat", "rb");
        FILE *temp = fopen("temp.dat", "wb");
        int found = 0;
        if (!fp || !temp) { printf("File error!\n"); if (fp) fclose(fp); if (temp) fclose(temp); continue; }

        Item it;
        while (fread(&it, sizeof(Item), 1, fp)) {
            if (it.id == pid) {
                found = 1;
                if (pqty > it.quantity) {
                    printf("❌ Not enough stock for ID %d (available %d). Skipping.\n", pid, it.quantity);
                    /* write unchanged record */
                } else {
                    float total = it.price * pqty;
                    printf("💰 Customer Total = ₹%.2f\n", total);
                    it.quantity -= pqty;
                    /* record sale */
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
            /* reload BST from updated file */
            freeInventory();
            loadInventory();
            printf("✅ Processed and inventory updated.\n");
        } else {
            remove("temp.dat");
            printf("❌ Item ID %d not found in inventory.\n", pid);
        }
    }

    printf("All queued customers processed. Final queue state:\n");
    printQueueState();
}

/* ---------- MANUAL QUEUE OPERATIONS ---------- */

/* Add a customer manually to queue */
void addToQueue() {
    int id, qty;
    printf("Enter item ID to enqueue: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input\n"); return; }
    printf("Enter quantity: ");
    if (scanf("%d", &qty) != 1) { printf("Invalid input\n"); return; }

    enqueue(id, qty);
    printf("✅ Customer added to queue.\n");
    printQueueState();
}

/* Remove next customer manually (simulate serving one customer) */
void removeFromQueue() {
    int id, qty;
    if (dequeue(&id, &qty)) {
        printf("🧾 Served one customer: ItemID=%d, Qty=%d\n", id, qty);
    } else {
        printf("🚫 Queue is empty! Nothing to remove.\n");
    }
    printQueueState();
}

/* ---------- DELETE SPECIFIC CUSTOMER FROM QUEUE ---------- */
void deleteFromQueue() {
    if (!q_front) {
        printf("🚫 Queue is empty. Nothing to delete.\n");
        return;
    }

    int id;
    printf("Enter Item ID to delete from queue: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    Customer *cur = q_front, *prev = NULL;

    // Traverse queue to find matching ID
    while (cur && cur->id != id) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) {
        printf("❌ Item ID %d not found in queue.\n", id);
        return;
    }

    // Node found — adjust pointers
    if (!prev) {
        // deleting the first node
        q_front = cur->next;
        if (!q_front) q_rear = NULL; // queue became empty
    } else {
        prev->next = cur->next;
        if (!cur->next)
            q_rear = prev; // deleted last node
    }

    free(cur);
    printf("✅ Item ID %d removed from queue.\n", id);
    printQueueState();
}

/* ---------- VIEW CURRENT QUEUE ---------- */
void viewQueue() {
    printf("\n--- Current Billing Queue ---\n");
    if (!q_front) {
        printf("(empty)\n");
        printf("-----------------------------\n");
        return;
    }

    Customer *cur = q_front;
    int pos = 1;
    while (cur) {
        printf("%d) ItemID: %d | Quantity: %d\n", pos++, cur->id, cur->qty);
        cur = cur->next;
    }
    printf("-----------------------------\n");
}
