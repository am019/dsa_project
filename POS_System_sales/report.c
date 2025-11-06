#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "report.h"
#include "billing.h"

typedef struct SaleNode {
    Sale sale;
    struct SaleNode *next;
} SaleNode;

/* Function to display report */
void generateReport() {
    FILE *fs = fopen("sales.dat", "rb");
    if (!fs) { printf("No sales data found!\n"); return; }

    SaleNode *head = NULL, *tail = NULL;
    Sale s;
    float totalRevenue = 0;

    while (fread(&s, sizeof(Sale), 1, fs)) {
        SaleNode *n = (SaleNode*)malloc(sizeof(SaleNode));
        n->sale = s;
        n->next = NULL;
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
        totalRevenue += s.total_price;
    }
    fclose(fs);

    printf("\n====== SALES REPORT ======\n");
    if (!head) { printf("(no sales)\n"); return; }

    printf("%-5s %-20s %-6s %-10s\n", "ID", "Name", "Qty", "Total");
    printf("---------------------------------------------\n");
    SaleNode *cur = head;
    while (cur) {
        printf("%-5d %-20s %-6d ₹%-10.2f\n",
               cur->sale.id, cur->sale.name, cur->sale.qty_sold, cur->sale.total_price);
        cur = cur->next;
    }
    printf("---------------------------------------------\n");
    printf("💵 Total Revenue: ₹%.2f\n", totalRevenue);

    while (head) {
        SaleNode *tmp = head;
        head = head->next;
        free(tmp);
    }
}

/* Partial return of items */
void returnItems() {
    FILE *fs = fopen("sales.dat", "rb");
    if (!fs) { 
        printf("No sales data found!\n"); 
        return; 
    }

    SaleNode *head = NULL, *tail = NULL;
    Sale s;

    while (fread(&s, sizeof(Sale), 1, fs)) {
        SaleNode *n = (SaleNode*)malloc(sizeof(SaleNode));
        n->sale = s;
        n->next = NULL;
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
    }
    fclose(fs);

    if (!head) { 
        printf("(no sales to return)\n"); 
        return; 
    }

    int retId, retQty;

    printf("\nEnter Sale ID to return: ");
    if (scanf("%d", &retId) != 1) {
        printf("❌ Invalid input! ID must be a number.\n");
        while (getchar() != '\n'); // clear buffer
        return;
    }

    printf("Enter quantity to return: ");
    if (scanf("%d", &retQty) != 1) {
        printf("❌ Invalid input! Quantity must be a number.\n");
        while (getchar() != '\n'); // clear buffer
        return;
    }

    SaleNode *cur = head;
    int found = 0;

    while (cur) {
        if (cur->sale.id == retId) {
            found = 1;
            if (retQty > cur->sale.qty_sold) {
                printf("⚠️ Cannot return more than sold quantity (%d).\n", cur->sale.qty_sold);
                break;
            }
            float unitPrice = cur->sale.total_price / cur->sale.qty_sold;
            cur->sale.qty_sold -= retQty;
            cur->sale.total_price -= unitPrice * retQty;

            printf("✅ Returned %d of '%s'. Updated qty: %d, new total: ₹%.2f\n",
                   retQty, cur->sale.name, cur->sale.qty_sold, cur->sale.total_price);
            break;
        }
        cur = cur->next;
    }

    if (!found) {
        printf("⚠️ Sale ID %d not found!\n", retId);
    }

    /* Write updated list back to file */
    fs = fopen("sales.dat", "wb");
    cur = head;
    while (cur) {
        fwrite(&cur->sale, sizeof(Sale), 1, fs);
        cur = cur->next;
    }
    fclose(fs);

    /* Free the list */
    while (head) {
        SaleNode *tmp = head;
        head = head->next;
        free(tmp);
    }

    printf("\nSales file updated successfully.\n");
}
