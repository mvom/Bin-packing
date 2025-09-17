#include <stdlib.h>
#include "LinkedList.h"
#include "Disk.h"
#include "File.h"
#include "BP.h"

/*
    === STRATÉGIE FIRST-FIT (avec TRI DÉCROISSANT) ===

    Cette méthode place chaque fichier sur le PREMIER disque disponible où il peut rentrer.
    - Les fichiers sont d'abord triés par taille décroissante.
    - Si le fichier ne rentre pas sur le disque courant, un nouveau disque est créé.
    - Simple et efficace, cette approche réduit la fragmentation dans certains cas.
*/

static int compareFileSizeDescending(const void *f1, const void *f2)
{
    return -compareFileSize(f1, f2);  
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{

    llSort(files, compareFileSizeDescending);

    Node *current = llHead(files);
    Disk *currentDisk = diskCreate(diskSize);
    llInsertLast(disks, currentDisk);
    size_t nbDisks = 1;

    while (current != NULL)
    {
        File *f = llData(current);
        if (!diskAddFile(currentDisk, f))
        {
            currentDisk = diskCreate(diskSize);
            llInsertLast(disks, currentDisk);
            diskAddFile(currentDisk, f);
            nbDisks++;  
        }

        current = llNext(current);
    }

    return nbDisks; 
}
