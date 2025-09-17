#ifndef BST_FIRSTFIT_H
#define BST_FIRSTFIT_H

#include <stddef.h>
#include "Disk.h"

typedef struct FFNode_t FFNode;  // Déclaration opaque

// Fonctions principales
FFNode *bstInsert(FFNode *node, Disk *d);
FFNode *bstRemove(FFNode *root, size_t index);
void bstFindFirstFit(FFNode *node, size_t fileSize, FFNode **result);
void bstFree(FFNode *root);

// Accesseur
Disk *ffGetDisk(FFNode *node);

#endif // BST_FIRSTFIT_H
