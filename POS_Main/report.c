
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "report.h"
#include "billing.h"

/* Linked list node for sales */
typedef struct SaleNode {
    Sale sale;
    struct SaleNode *next;
} SaleNode;

/* Read sales.dat into linked list and print with visualization */
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

    /* Visualize linked list */
    printf("\n====== SALES REPORT (Linked List Traversal) ======\n");
    if (!head) { printf("(no sales)\n"); return; }
    printf("Traversal order (head -> ... -> tail):\n");
    SaleNode *cur = head;
    int idx = 1;
    printf("%-5s %-20s %-6s %-10s\n", "ID", "Name", "Qty", "Total");
    printf("---------------------------------------------\n");
    while (cur) {
        printf("%-5d %-20s %-6d ₹%-10.2f\n",
               cur->sale.id, cur->sale.name, cur->sale.qty_sold, cur->sale.total_price);
        cur = cur->next;
        idx++;
    }
    printf("---------------------------------------------\n");
    printf("💵 Total Revenue: ₹%.2f\n", totalRevenue);

    /* free list */
    cur = head;
    while (cur) {
        SaleNode *t = cur->next;
        free(cur);
        cur = t;
    }
}
