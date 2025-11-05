
#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"
#include "billing.h"
#include "report.h"

int main() {
    int choice;
    /* load inventory into BST at startup */
    loadInventory();

    while (1) {
        printf("\n========== POS SYSTEM (DSA Integrated) ==========\n");
        printf("1. Add Item (Easy) - uses file + BST\n");
        printf("2. View Items (Easy) - BST inorder (visualized)\n");
        printf("3. Search Item (Medium) - BST search (visualized)\n");
        printf("4. Update Item (Medium) - updates BST and file\n");
        printf("5. Buy Item (Medium-Hard) - enqueues customers (visualized queue)\n");
        printf("6. Generate Report (Medium-Hard) - uses linked list (visualized)\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { printf("Invalid input\n"); exit(0); }

        switch (choice) {
            case 1: addItem(); break;
            case 2: viewItems(); break;
            case 3: searchItem(); break;
            case 4: updateItem(); break;
            case 5: buyItem(); break;
            case 6: generateReport(); break;
            case 7:
                freeInventory();
                exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
