#include <stddef.h>
#include <stdlib.h>

#include "BP.h"
#include "Disk.h"
#include "File.h"
#include "LinkedList.h"
#include "PQ.h"

/*
    === STRATÉGIE WORST-FIT ===

    Cette stratégie consiste à placer chaque fichier dans le disque ayant le PLUS d'espace libre.
    Objectif : éviter la fragmentation en remplissant les disques les moins pleins.

    Étapes :
    1. Trier les fichiers par taille décroissante.
    2. Tenter de les insérer dans le disque le plus vide (max espace libre).
    3. Si aucun disque n’a assez d’espace, créer un nouveau disque.
*/

static int compareFileSizeDescending(const void *f1, const void *f2)
{
    return -compareFileSize(f1, f2);  // Négation pour ordre décroissant
}


// Récupère l'élément à l'index donné dans une liste chaînée
static void *llGet(const List *list, size_t index)
{
    if (!list || index >= llLength(list))
        return NULL;

    Node *current = llHead(list);
    for (size_t i = 0; i < index; ++i)
    {
        current = llNext(current);
    }

    return llData(current);
}

size_t binpacking(size_t diskSize, List *files, List *disks)
{
    llSort(files, compareFileSizeDescending);

    PQ *pq = pqCreate(llLength(files), compareDiskFreeSpace);
    if (!pq)
        return 0;

    size_t n_files = llLength(files);

    for (size_t i = 0; i < n_files; ++i)
    {
        File *file = llGet(files, i);  

        Disk *best_disk = pqExtractMax(pq); 

        if (best_disk && diskAddFile(best_disk, file)) 
        {
            pqInsert(pq, best_disk);  
        }
        else
        {
            if (best_disk)
                pqInsert(pq, best_disk);  // Remis même si inutilisable (état inchangé)

            // Création d’un nouveau disque si nécessaire
            Disk *new_disk = diskCreate(diskSize);
            diskAddFile(new_disk, file);
            llInsertLast(disks, new_disk);  // Ajout à la liste des disques
            pqInsert(pq, new_disk);         // Et dans la file de priorité
        }
    }

    size_t nb_disks = llLength(disks);
    pqFree(pq);  
    return nb_disks;
}
