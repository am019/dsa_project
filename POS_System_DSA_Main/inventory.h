
#ifndef INVENTORY_H
#define INVENTORY_H

#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN];
    float price;
    int quantity;
} Item;

/* Inventory (BST) functions */
void loadInventory();      /* load inventory.dat into BST (call at program start) */
void saveInventory();      /* overwrite inventory.dat from BST */
void addItem();            /* interactive add: append and insert to BST */
void viewItems();          /* print inventory (inorder) */
void searchItem();         /* search using BST (visualize path) */
void updateItem();         /* update item (modify BST and save file) */
void freeInventory();      /* free in-memory BST */

#endif
