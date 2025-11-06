#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

typedef struct Node {
    Item item;
    struct Node *left, *right;
} Node;

static Node *root = NULL;

/* Helper to create a new node */
static Node* newNode(Item it) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->item = it;
    n->left = n->right = NULL;
    return n;
}

/* Insert into BST */
static Node* insertNode(Node *node, Item it) {
    if (!node) return newNode(it);
    if (it.id < node->item.id)
        node->left = insertNode(node->left, it);
    else if (it.id > node->item.id)
        node->right = insertNode(node->right, it);
    else
        printf("Duplicate ID ignored.\n");
    return node;
}

/* Save inventory recursively */
static void saveInventory(Node *node, FILE *fp) {
    if (!node) return;
    fwrite(&node->item, sizeof(Item), 1, fp);
    saveInventory(node->left, fp);
    saveInventory(node->right, fp);
}

/* Display inventory recursively */
static void displayRecursive(Node *node) {
    if (!node) return;
    displayRecursive(node->left);
    printf("%-5d %-20s %-10.2f %-6d\n",
           node->item.id, node->item.name, node->item.price, node->item.quantity);
    displayRecursive(node->right);
}

/* Load items from file */
void loadInventory() {
    FILE *fp = fopen("inventory.dat", "rb");
    if (!fp) return;
    Item it;
    while (fread(&it, sizeof(Item), 1, fp))
        root = insertNode(root, it);
    fclose(fp);
}

/* Add item */
void addItem() {
    Item item;

    printf("Enter Item ID: ");
    if (scanf("%d", &item.id) != 1) {
        printf("❌ Invalid input! ID must be a number.\n");
        while (getchar() != '\n'); // clear input buffer
        return;
    }

    printf("Enter Item Name: ");
    scanf("%s", item.name);

    printf("Enter Item Price: ");
    if (scanf("%f", &item.price) != 1) {
        printf("❌ Invalid input! Price must be a number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Quantity: ");
    if (scanf("%d", &item.quantity) != 1) {
        printf("❌ Invalid input! Quantity must be a number.\n");
        while (getchar() != '\n');
        return;
    }

    root = insertNode(root, item);
    FILE *fp = fopen("inventory.dat", "ab");
    fwrite(&item, sizeof(Item), 1, fp);
    fclose(fp);

    printf("✅ Item added successfully!\n");
}

/* Update item quantity or price */
void updateItem() {
    loadInventory();
    int id, found = 0;

    printf("Enter ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("❌ Invalid input! ID must be a number.\n");
        while (getchar() != '\n');
        return;
    }

    Node *cur = root;
    while (cur) {
        if (cur->item.id == id) {
            found = 1;

            printf("Enter new price: ");
            if (scanf("%f", &cur->item.price) != 1) {
                printf("❌ Invalid input! Price must be a number.\n");
                while (getchar() != '\n');
                return;
            }

            printf("Enter new quantity: ");
            if (scanf("%d", &cur->item.quantity) != 1) {
                printf("❌ Invalid input! Quantity must be a number.\n");
                while (getchar() != '\n');
                return;
            }

            printf("✅ Updated successfully!\n");
            break;
        }
        cur = (id < cur->item.id) ? cur->left : cur->right;
    }

    if (!found)
        printf("⚠️ Item not found!\n");

    FILE *fp = fopen("inventory.dat", "wb");
    saveInventory(root, fp);
    fclose(fp);
}


/* Delete item by ID */
static Node* deleteNode(Node *node, int id) {
    if (!node) return NULL;
    if (id < node->item.id)
        node->left = deleteNode(node->left, id);
    else if (id > node->item.id)
        node->right = deleteNode(node->right, id);
    else {
        if (!node->left) {
            Node *r = node->right;
            free(node);
            return r;
        } else if (!node->right) {
            Node *l = node->left;
            free(node);
            return l;
        } else {
            Node *succ = node->right;
            while (succ->left) succ = succ->left;
            node->item = succ->item;
            node->right = deleteNode(node->right, succ->item.id);
        }
    }
    return node;
}

void deleteItem() {
    loadInventory();
    int id;

    printf("Enter ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("❌ Invalid input! ID must be a number.\n");
        while (getchar() != '\n');
        return;
    }

    root = deleteNode(root, id);

    FILE *fp = fopen("inventory.dat", "wb");
    saveInventory(root, fp);
    fclose(fp);

    printf("✅ Item deleted successfully!\n");
}


/* Display inventory */
void displayInventory() {
    loadInventory();
    printf("\n====== INVENTORY LIST ======\n");
    printf("%-5s %-20s %-10s %-6s\n", "ID", "Name", "Price", "Qty");
    printf("-------------------------------------------\n");
    displayRecursive(root);
}


/* Free all nodes from memory */
void freeInventory() {
    if (!root) return;
    Node *stack[100];
    int top = -1;
    Node *cur = root;
    while (cur || top != -1) {
        while (cur) {
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];
        Node *right = cur->right;
        free(cur);
        cur = right;
    }
    root = NULL;
}
