
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

typedef struct Node {
    Item item;
    struct Node *left, *right;
} Node;

static Node *root = NULL;

/* Create node */
static Node* newNode(Item it) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->item = it;
    n->left = n->right = NULL;
    return n;
}

/* Insert into BST by id (no duplicates allowed) */
static Node* insertNode(Node *node, Item it) {
    if (!node) return newNode(it);
    if (it.id < node->item.id) node->left = insertNode(node->left, it);
    else if (it.id > node->item.id) node->right = insertNode(node->right, it);
    else {
        /* if duplicate id, update existing record */
        node->item = it;
    }
    return node;
}

/* Search by id and visualize path */
static Node* searchNode(Node *node, int id) {
    if (!node) return NULL;
    printf("Visiting node ID=%d\n", node->item.id);
    if (id == node->item.id) return node;
    if (id < node->item.id) return searchNode(node->left, id);
    return searchNode(node->right, id);
}

/* Inorder traversal to print inventory (sorted by id) */
static void inorderPrint(Node *node, int depth) {
    if (!node) return;
    inorderPrint(node->left, depth+1);
    for (int i=0;i<depth;i++) printf("  ");
    printf("ID:%-5d Name:%-20s Price:₹%-8.2f Qty:%-4d\n",
           node->item.id, node->item.name, node->item.price, node->item.quantity);
    inorderPrint(node->right, depth+1);
}

/* Inorder traversal to write to file */
static void inorderWrite(Node *node, FILE *fp) {
    if (!node) return;
    inorderWrite(node->left, fp);
    fwrite(&node->item, sizeof(Item), 1, fp);
    inorderWrite(node->right, fp);
}

/* Free BST */
static void freeTree(Node *node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

/* Public functions */

void loadInventory() {
    FILE *fp = fopen("inventory.dat", "rb");
    if (!fp) {
        /* no inventory yet */
        root = NULL;
        return;
    }
    Item it;
    while (fread(&it, sizeof(Item), 1, fp)) {
        root = insertNode(root, it);
    }
    fclose(fp);
}

void saveInventory() {
    FILE *fp = fopen("inventory.dat", "wb");
    if (!fp) {
        printf("Error: could not open inventory.dat for writing\n");
        return;
    }
    inorderWrite(root, fp);
    fclose(fp);
}

/* Keep previous behaviour but use BST */
void addItem() {
    Item item;
    printf("Enter item ID: ");
    if (scanf("%d", &item.id) != 1) { printf("Invalid input\n"); return; }
    printf("Enter item name: ");
    scanf("%s", item.name);
    printf("Enter price: ");
    scanf("%f", &item.price);
    printf("Enter quantity: ");
    scanf("%d", &item.quantity);

    /* append to file for persistence and insert into BST */
    FILE *fp = fopen("inventory.dat", "ab");
    if (!fp) { printf("File error!\n"); return; }
    fwrite(&item, sizeof(Item), 1, fp);
    fclose(fp);

    root = insertNode(root, item);
    printf("✅ Item added successfully!\n");
}

void viewItems() {
    if (!root) { printf("No inventory found!\n"); return; }
    printf("\n--- Inventory (BST inorder - sorted by ID) ---\n");
    inorderPrint(root, 0);
}

void searchItem() {
    if (!root) { printf("No inventory present. Load or add items first.\n"); return; }
    int id;
    printf("Enter item ID to search: ");
    scanf("%d", &id);
    printf("Searching path (visualized):\n");
    Node *n = searchNode(root, id);
    if (n) {
        printf("✅ Item found:\n");
        printf("ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n",
               n->item.id, n->item.name, n->item.price, n->item.quantity);
    } else {
        printf("❌ Item not found!\n");
    }
}

void updateItem() {
    if (!root) { printf("No inventory present. Load or add items first.\n"); return; }
    int id;
    printf("Enter item ID to update: ");
    scanf("%d", &id);
    Node *n = searchNode(root, id);
    if (!n) { printf("❌ Item not found!\n"); return; }

    printf("Enter new price: ");
    scanf("%f", &n->item.price);
    printf("Enter new quantity: ");
    scanf("%d", &n->item.quantity);

    /* persist full inventory */
    saveInventory();
    printf("✅ Item updated successfully!\n");
}

void freeInventory() {
    freeTree(root);
    root = NULL;
}
