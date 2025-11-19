#include <stdio.h>

int getPositiveInt(const char *msg) {
    int x;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &x) != 1) {
            printf("Invalid input! Enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }
        if (x < 0) {
            printf("Negative values are NOT allowed. Try again.\n");
            continue;
        }
        return x;
    }
}

float getPositiveFloat(const char *msg) {
    float x;
    while (1) {
        printf("%s", msg);
        if (scanf("%f", &x) != 1) {
            printf("Invalid input! Enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }
        if (x < 0) {
            printf("Negative values are NOT allowed. Try again.\n");
            continue;
        }
        return x;
    }
}
