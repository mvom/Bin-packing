#include <stdlib.h>
#include <stdio.h>
#include "PQ.h"

// Supprime le max de la PQ (interne)
void *pqPop(PQ *pq);

// Structure opaque de file de priorité (max-heap)
struct PQ_t {
    size_t capacity;
    size_t size;
    void **data;
    int (*compare)(const void *, const void *);
};

// -------------------------- Fonctions internes --------------------------

// Remonte un élément pour restaurer l'ordre du tas
static void siftUp(PQ *pq, size_t index) {
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (pq->compare(pq->data[index], pq->data[parent]) > 0) {
            void *tmp = pq->data[index];
            pq->data[index] = pq->data[parent];
            pq->data[parent] = tmp;
            index = parent;
        } else {
            break;
        }
    }
}

// Redescend un élément pour restaurer l'ordre du tas
static void siftDown(PQ *pq, size_t index) {
    while (1) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t largest = index;

        if (left < pq->size && pq->compare(pq->data[left], pq->data[largest]) > 0) {
            largest = left;
        }
        if (right < pq->size && pq->compare(pq->data[right], pq->data[largest]) > 0) {
            largest = right;
        }

        if (largest != index) {
            void *tmp = pq->data[index];
            pq->data[index] = pq->data[largest];
            pq->data[largest] = tmp;
            index = largest;
        } else {
            break;
        }
    }
}

// -------------------------- Fonctions publiques --------------------------

// Création d'une file de priorité
PQ *pqCreate(size_t capacity, int (*compare)(const void *, const void *)) {
    PQ *pq = malloc(sizeof(PQ));
    if (!pq) return NULL;

    pq->capacity = capacity;
    pq->size = 0;
    pq->data = malloc(capacity * sizeof(void *));
    if (!pq->data) {
        free(pq);
        return NULL;
    }
    pq->compare = compare;

    return pq;
}

// Libère la mémoire
void pqFree(PQ *pq) {
    if (pq) {
        free(pq->data);
        free(pq);
    }
}

// Insertion d'un élément
bool pqInsert(PQ *pq, void *key) {
    if (pq->size >= pq->capacity) return false;

    pq->data[pq->size] = key;
    siftUp(pq, pq->size);
    pq->size++;
    return true;
}

// Supprime et retourne l'élément max
void *pqPop(PQ *pq) {
    if (pq->size == 0) return NULL;

    void *top = pq->data[0];
    pq->data[0] = pq->data[--pq->size];
    siftDown(pq, 0);
    return top;
}

// Alias de pqPop
void *pqExtractMax(PQ *pq) {
    return pqPop(pq);
}

// Retourne l'élément max sans le retirer
void *pqGetMax(const PQ *pq) {
    if (pq->size == 0) return NULL;
    return pq->data[0];
}

// Taille actuelle
size_t pqSize(const PQ *pq) {
    return pq->size;
}

// Capacité maximale
size_t pqCapacity(const PQ *pq) {
    return pq->capacity;
}
