
#ifndef BILLING_H
#define BILLING_H
#include "inventory.h"

typedef struct {
    int id;
    char name[50];
    int qty_sold;
    float total_price;
} Sale;

void buyItem();
void addToQueue();
void removeFromQueue();
void deleteFromQueue();
void viewQueue();



#endif
