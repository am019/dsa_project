#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"
#include "billing.h"
#include "report.h"
#include "utils.h"
void clearInputBuffer();



void salesMenu() {
    int ch;
    do {
        printf("\n====== SALES MENU ======\n");
        printf("1. View Sales Report\n");
        printf("2. Return Items (Partial)\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &ch) != 1) {
            printf("❌ Invalid input! Please enter a number.\n");
            clearInputBuffer();  
            ch = -1;
            continue;
        }

        clearInputBuffer(); // remove leftover \n

        switch (ch) {
            case 1:
                generateReport();
                break;
            case 2:
                returnItems();
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (ch != 0);
}


#include <stdio.h>

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int choice;
    do {
        printf("\n========== POS SYSTEM ==========");
        printf("\n1. Add Item");
        printf("\n2. Update Item");
        printf("\n3. Delete Item");
        printf("\n4. Billing");
        printf("\n5. Inventory");
        printf("\n6. Sales Report / Return Menu");
        printf("\n0. Exit");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();  // remove invalid input
            choice = -1;         // force invalid choice to repeat loop
            continue;
        }

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                updateItem();
                break;
            case 3:
                deleteItem();
                break;
            case 4:
                billing();
                break;
            case 5:
                displayInventory();
                break;
            case 6:
                salesMenu();
                break;
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
