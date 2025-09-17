#include <stdlib.h>
#include "BST_bestfit.h"

// Structure représentant un noeud de l'AVL (Arbre de recherche binaire équilibré)
struct BSTNode_t {
    Disk *disk;
    struct BSTNode_t *left;
    struct BSTNode_t *right;
    int height;
};

// Accesseur
Disk *bstGetDisk(BSTNode *node) {
    return node ? node->disk : NULL;
}

// Fonctions utilitaires AVL
static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int height(BSTNode *node) {
    return node ? node->height : 0;
}

static int getBalance(BSTNode *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

static BSTNode *rotateRight(BSTNode *y) {
    BSTNode *x = y->left;
    BSTNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

static BSTNode *rotateLeft(BSTNode *x) {
    BSTNode *y = x->right;
    BSTNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

BSTNode *bstInsert(BSTNode *node, Disk *d) {
    if (!node) {
        BSTNode *newNode = malloc(sizeof(BSTNode));
        if (!newNode) exit(1);
        newNode->disk = d;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (diskFreeSpace(d) < diskFreeSpace(node->disk))
        node->left = bstInsert(node->left, d);
    else
        node->right = bstInsert(node->right, d);

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && diskFreeSpace(d) < diskFreeSpace(node->left->disk))
        return rotateRight(node);
    if (balance < -1 && diskFreeSpace(d) >= diskFreeSpace(node->right->disk))
        return rotateLeft(node);
    if (balance > 1 && diskFreeSpace(d) >= diskFreeSpace(node->left->disk)) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && diskFreeSpace(d) < diskFreeSpace(node->right->disk)) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

BSTNode *minValueNode(BSTNode *node) {
    BSTNode *current = node;
    while (current->left)
        current = current->left;
    return current;
}

BSTNode *bstRemove(BSTNode *root, Disk *d) {
    if (!root) return NULL;

    if (d == root->disk) {
        if (!root->left || !root->right) {
            BSTNode *temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }
        BSTNode *succ = minValueNode(root->right);
        root->disk = succ->disk;
        root->right = bstRemove(root->right, succ->disk);
    } else if (diskFreeSpace(d) < diskFreeSpace(root->disk)) {
        root->left = bstRemove(root->left, d);
    } else {
        root->right = bstRemove(root->right, d);
    }

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

BSTNode *bstFindBestFit(BSTNode *root, size_t fileSize, BSTNode **bestFit) {
    if (!root) return NULL;

    size_t freeSpace = diskFreeSpace(root->disk);
    if (freeSpace >= fileSize) {
        if (!*bestFit || freeSpace < diskFreeSpace((*bestFit)->disk)) {
            *bestFit = root;
        }
    }

    bstFindBestFit(root->left, fileSize, bestFit);
    bstFindBestFit(root->right, fileSize, bestFit);

    return *bestFit;
}

void bstFree(BSTNode *root) {
    if (!root) return;
    bstFree(root->left);
    bstFree(root->right);
    free(root);
}
