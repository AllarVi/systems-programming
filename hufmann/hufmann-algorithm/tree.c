//
// Created by Allar Viinamäe on 10/11/2018.
//

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
