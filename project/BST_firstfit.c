#include "BST_firstfit.h"
#include "Disk.h"
#include <stdlib.h>

// === STRUCTURE AVL POUR FIRST-FIT ===
struct FFNode_t {
    Disk *disk;
    size_t index;
    size_t maxFree;
    int height;
    struct FFNode_t *left, *right;
};

// Getter
Disk *ffGetDisk(FFNode *node) {
    return node ? node->disk : NULL;
}

// === AVL UTILS ===
static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int height(FFNode *node) {
    return node ? node->height : 0;
}

static size_t maxFree(FFNode *node) {
    return node ? node->maxFree : 0;
}

static void updateNode(FFNode *node) {
    if (node) {
        node->height = 1 + max(height(node->left), height(node->right));
        node->maxFree = max(
            diskFreeSpace(node->disk),
            max(maxFree(node->left), maxFree(node->right))
        );
    }
}

static FFNode *rotateRight(FFNode *y) {
    FFNode *x = y->left;
    FFNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    updateNode(y);
    updateNode(x);

    return x;
}

static FFNode *rotateLeft(FFNode *x) {
    FFNode *y = x->right;
    FFNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    updateNode(x);
    updateNode(y);

    return y;
}

// === INSERTION AVL ===
FFNode *bstInsert(FFNode *node, Disk *d) {
    size_t index = (size_t) d;

    if (!node) {
        FFNode *newNode = malloc(sizeof(FFNode));
        if (!newNode) exit(1);
        newNode->disk = d;
        newNode->index = index;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        newNode->maxFree = diskFreeSpace(d);
        return newNode;
    }

    if (index < node->index)
        node->left = bstInsert(node->left, d);
    else
        node->right = bstInsert(node->right, d);

    updateNode(node);

    int balance = height(node->left) - height(node->right);

    if (balance > 1 && index < node->left->index)
        return rotateRight(node);
    if (balance < -1 && index > node->right->index)
        return rotateLeft(node);
    if (balance > 1 && index > node->left->index) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && index < node->right->index) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// === FIRST-FIT SEARCH ===
void bstFindFirstFit(FFNode *node, size_t fileSize, FFNode **result) {
    if (!node || maxFree(node) < fileSize) return;

    if (node->left && maxFree(node->left) >= fileSize) {
        bstFindFirstFit(node->left, fileSize, result);
        if (*result) return;
    }

    if (diskFreeSpace(node->disk) >= fileSize) {
        *result = node;
        return;
    }

    bstFindFirstFit(node->right, fileSize, result);
}

// === SUPPRESSION AVL ===
FFNode *bstRemove(FFNode *root, size_t index) {
    if (!root) return NULL;

    if (index < root->index)
        root->left = bstRemove(root->left, index);
    else if (index > root->index)
        root->right = bstRemove(root->right, index);
    else {
        if (!root->left || !root->right) {
            FFNode *temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }

        FFNode *succ = root->right;
        while (succ->left) succ = succ->left;

        root->disk = succ->disk;
        root->index = succ->index;
        root->right = bstRemove(root->right, succ->index);
    }

    updateNode(root);

    int balance = height(root->left) - height(root->right);

    if (balance > 1 && height(root->left->left) >= height(root->left->right))
        return rotateRight(root);
    if (balance > 1 && height(root->left->left) < height(root->left->right)) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && height(root->right->right) >= height(root->right->left))
        return rotateLeft(root);
    if (balance < -1 && height(root->right->right) < height(root->right->left)) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// === LIBÉRATION ===
void bstFree(FFNode *root) {
    if (!root) return;
    bstFree(root->left);
    bstFree(root->right);
    free(root);
}
