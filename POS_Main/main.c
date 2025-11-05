#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"
#include "billing.h"
#include "report.h"

int main() {
    int mainChoice, subChoice;
    loadInventory();  // Load BST from inventory.dat at program start

    while (1) {
        printf("\n==============================\n");
        printf("      🧾 POINT OF SALE SYSTEM\n");
        printf("==============================\n");
        printf("1. Inventory Management\n");
        printf("2. Billing (Queue)\n");
        printf("3. Generate Sales Report\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &mainChoice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (mainChoice) {

            /* ---------------------- INVENTORY MENU ---------------------- */
            case 1:
                while (1) {
                    printf("\n--- INVENTORY MANAGEMENT ---\n");
                    printf("1. Add Item\n");
                    printf("2. View Items\n");
                    printf("3. Search Item\n");
                    printf("4. Update Item\n");
                    printf("5. Delete Item\n");
                    printf("6. Back to Main Menu\n");
                    printf("Enter your choice: ");
                    if (scanf("%d", &subChoice) != 1) { 
                        printf("Invalid input.\n"); 
                        continue; 
                    }

                    switch (subChoice) {
                        case 1: addItem(); break;
                        case 2: viewItems(); break;
                        case 3: searchItem(); break;
                        case 4: updateItem(); break;
                        case 5: deleteItem(); break;
                        case 6: goto end_inventory;
                        default: printf("Invalid choice.\n");
                    }
                }
                end_inventory:
                break;

            /* ---------------------- BILLING MENU ---------------------- */
            case 2:
                while (1) {
                    printf("\n--- BILLING & QUEUE ---\n");
                    printf("1. Add Customer to Queue\n");
                    printf("2. Remove Next Customer\n");
                    printf("3. Delete Specific Customer from Queue\n");
                    printf("4. View Queue\n");
                    printf("5. Process Queue & Buy Items\n");
                    printf("6. Back to Main Menu\n");
                    printf("Enter your choice: ");
                    if (scanf("%d", &subChoice) != 1) { 
                        printf("Invalid input.\n"); 
                        continue; 
                    }

                    switch (subChoice) {
                        case 1: addToQueue(); break;
                        case 2: removeFromQueue(); break;
                        case 3: deleteFromQueue(); break;
                        case 4: viewQueue(); break;
                        case 5: buyItem(); break;
                        case 6: goto end_billing;
                        default: printf("Invalid choice.\n");
                    }
                }
                end_billing:
                break;

            /* ---------------------- REPORTS (Direct) ---------------------- */
            case 3:
                printf("\n--- SALES REPORT ---\n");
                generateReport();   // 👈 directly calls the report function
                break;

            /* ---------------------- EXIT ---------------------- */
            case 4:
                printf("\n💾 Saving inventory and exiting...\n");
                saveInventory();
                freeInventory();
                printf("✅ Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
