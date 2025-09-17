#ifndef BST_BESTFIT_H
#define BST_BESTFIT_H

#include "Disk.h"

// Déclaration opaque de la structure BSTNode
typedef struct BSTNode_t BSTNode;

// Fonctions AVL
BSTNode *bstInsert(BSTNode *node, Disk *d);
BSTNode *bstRemove(BSTNode *root, Disk *d);
BSTNode *bstFindBestFit(BSTNode *root, size_t fileSize, BSTNode **bestFit);
void bstFree(BSTNode *root);

// Accesseur pour récupérer le disque d’un nœud
Disk *bstGetDisk(BSTNode *node);

// Utilitaire
BSTNode *minValueNode(BSTNode *node);

#endif // BST_BESTFIT_H
