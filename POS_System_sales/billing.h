#ifndef BILLING_H
#define BILLING_H

typedef struct {
    int id;
    char name[50];
    int qty_sold;
    float total_price;
} Sale;

void billing();

#endif
