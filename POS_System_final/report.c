#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "report.h"
#include "billing.h"

typedef struct SaleNode {
    Sale sale;
    struct SaleNode *next;
} SaleNode;

/* ------------ Helper: Safe integer input --------------- */
int getValidatedInt(const char *msg) {
    int x;
    char buffer[100];

    while (1) {
        printf("%s", msg);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("❌ Input error.\n");
            continue;
        }

        if (sscanf(buffer, "%d", &x) == 1 && x >= 0)
            return x;

        printf("❌ Invalid input! Enter a positive number.\n");
    }
}

/* ------------ Helper: Safe string input ---------------- */
void getValidatedString(const char *msg, char *out) {
    while (1) {
        printf("%s", msg);
        if (!fgets(out, 50, stdin)) {
            printf("❌ Input error.\n");
            continue;
        }

        // remove newline
        out[strcspn(out, "\n")] = '\0';

        int valid = 1;
        for (int i = 0; out[i] != '\0'; i++) {
            if (!isalpha(out[i]) && out[i] != ' ') {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(out) > 0)
            return;

        printf("❌ Invalid name! Use letters only.\n");
    }
}

/* ------------------ SALES REPORT ----------------------- */
void generateReport() {
    FILE *fs = fopen("sales.dat", "rb");
    if (!fs) { printf("No sales data found!\n"); return; }

    SaleNode *head = NULL, *tail = NULL;
    Sale s;
    float totalRevenue = 0;

    while (fread(&s, sizeof(Sale), 1, fs)) {
        SaleNode *n = malloc(sizeof(SaleNode));
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

/* ------------------ RETURN ITEMS ----------------------- */
void returnItems() {
    FILE *fs = fopen("sales.dat", "rb");
    if (!fs) {
        printf("No sales data found!\n");
        return;
    }

    SaleNode *head = NULL, *tail = NULL;
    Sale s;

    while (fread(&s, sizeof(Sale), 1, fs)) {
        SaleNode *n = malloc(sizeof(SaleNode));
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

    char retName[50];
    getValidatedString("\nEnter Item Name to return: ", retName);

    int retQty = getValidatedInt("Enter quantity to return: ");

    SaleNode *cur = head;
    SaleNode *target = NULL;

    /* Find last matching sale by name */
    while (cur) {
        if (strcasecmp(cur->sale.name, retName) == 0) {
            target = cur;
        }
        cur = cur->next;
    }

    if (!target) {
        printf("⚠️ Item '%s' not found in sales!\n", retName);
        return;
    }

    if (retQty > target->sale.qty_sold) {
        printf("⚠️ Cannot return more than sold quantity (%d).\n", target->sale.qty_sold);
        return;
    }

    float unitPrice = target->sale.total_price / target->sale.qty_sold;

    target->sale.qty_sold -= retQty;
    target->sale.total_price -= unitPrice * retQty;

    printf("✅ Returned %d of '%s'. Updated qty: %d | New total: ₹%.2f\n",
           retQty, target->sale.name, target->sale.qty_sold, target->sale.total_price);

    /* Save updated sales file */
    fs = fopen("sales.dat", "wb");
    cur = head;
    while (cur) {
        fwrite(&cur->sale, sizeof(Sale), 1, fs);
        cur = cur->next;
    }
    fclose(fs);

    /* Free memory */
    while (head) {
        SaleNode *tmp = head;
        head = head->next;
        free(tmp);
    }

    printf("\nSales file updated successfully.\n");
}

