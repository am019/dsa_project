#ifndef INVENTORY_H
#define INVENTORY_H

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
} Item;

void addItem();
void updateItem();
void deleteItem();
void displayInventory();
void loadInventory();
void freeInventory();

#endif
