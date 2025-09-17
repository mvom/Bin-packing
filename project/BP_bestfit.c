#include <stdlib.h>
#include "Disk.h"
#include "LinkedList.h"
#include "BST_bestfit.h"
#include "BP.h"

/*
    === STRATÉGIE BEST-FIT avec AVL ===

    - Les fichiers sont triés par taille décroissante.
    - Pour chaque fichier, on cherche le disque avec l’espace libre le plus proche (juste assez grand).
    - Les disques sont organisés dans un arbre AVL (équilibré) pour des recherches efficaces.
*/
static int compareFileSizeDescending(const void *f1, const void *f2) {
    return -compareFileSize(f1, f2);
}

size_t binpacking(size_t diskSize, List *files, List *disks) {
    llSort(files, compareFileSizeDescending);
    BSTNode *root = NULL;
    size_t numDisks = 0;

    for (Node *curr = llHead(files); curr != NULL; curr = llNext(curr)) {
        File *f = llData(curr);
        BSTNode *best = NULL;

        bstFindBestFit(root, fileSize(f), &best);

        if (best) {
            Disk *d = bstGetDisk(best);        
            root = bstRemove(root, d);
            diskAddFile(d, f);
            root = bstInsert(root, d);
        } else {
            Disk *newDisk = diskCreate(diskSize);
            if (!newDisk) exit(1);
            diskAddFile(newDisk, f);
            llInsertLast(disks, newDisk);
            root = bstInsert(root, newDisk);
            numDisks++;
        }
    }

    bstFree(root);
    return numDisks;
}
