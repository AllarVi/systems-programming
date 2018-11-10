//
// Bubble-sort inspiration: https://www.geeksforgeeks.org/bubble-sort/
//

#include <stdbool.h>
#include "tree.h"

struct forest *makeForest(struct map *fileCharFreq) {
    int forestSize = getEntriesTotal(fileCharFreq);

    struct forest *newForest = malloc(sizeof(struct forest));
    newForest->size = forestSize;
    newForest->treeList = malloc(sizeof(struct treeList *) * forestSize);

    struct node **entries = getEntries(fileCharFreq);

    for (int i = 0; i < forestSize; i++) {
        struct tree *newTree = malloc(sizeof(struct tree));
        newTree->left = NULL;
        newTree->right = NULL;
        newTree->c = entries[i]->key;
        newTree->freq = entries[i]->val;

        newForest->treeList[i] = newTree;
    }

    return newForest;
};

void swap(struct tree **xp, struct tree **yp) {
    struct tree *temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(struct forest *forest) {
    int forestSize = forest->size;
    bool swapped;

    for (int i = 0; i < forestSize - 1; i++) {
        swapped = false;
        for (int j = 0; j < forestSize - i - 1; j++) {
            if (forest->treeList[j]->freq > forest->treeList[j + 1]->freq) {
                swap(&forest->treeList[j], &forest->treeList[j + 1]);
                swapped = true;
            }
        }

        // IF no two elements were swapped by inner loop, then break
        if (swapped == false)
            break;
    }
}
