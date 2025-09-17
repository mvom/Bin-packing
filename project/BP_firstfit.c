#include <stdlib.h>
#include "Disk.h"
#include "LinkedList.h"
#include "BST_firstfit.h"
#include "BP.h"

/*
    === STRATÉGIE FIRST-FIT (avec AVL) ===

    - Les fichiers sont triés par taille décroissante.
    - Chaque fichier est placé dans le *premier* disque ayant suffisamment d'espace.
    - Les disques sont organisés dans un AVL pour accélérer la recherche.
    - L’AVL est mis à jour après chaque insertion (le free space change).
*/

// === TRI DESCENDANT PAR TAILLE DE FICHIER ===
static int compareFileSizeDescending(const void *f1, const void *f2) {
    return -compareFileSize(f1, f2);
}

// === STRATÉGIE FIRST-FIT (AVL) ===
size_t binpacking(size_t diskSize, List *files, List *disks) {
    FFNode *root = NULL;

    llSort(files, compareFileSizeDescending);

    for (Node *n = llHead(files); n != NULL; n = llNext(n)) {
        File *f = llData(n);
        FFNode *target = NULL;

        bstFindFirstFit(root, fileSize(f), &target);

        if (target != NULL) {
            Disk *d = ffGetDisk(target);  
            diskAddFile(d, f);
            root = bstRemove(root, (size_t) d);
            root = bstInsert(root, d);
        } else {
            Disk *newDisk = diskCreate(diskSize);
            diskAddFile(newDisk, f);
            llInsertLast(disks, newDisk);
            root = bstInsert(root, newDisk);
        }
    }

    size_t numDisks = llLength(disks);
    bstFree(root);
    return numDisks;
}
