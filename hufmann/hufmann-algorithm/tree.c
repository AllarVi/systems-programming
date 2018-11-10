//
// Created by Allar Viinamäe on 10/11/2018.
//

#include "tree.h"

struct forest *makeForest(struct map *fileCharFreq) {
    int forestSize = getNumOfDiffChars(fileCharFreq);

    struct forest *newForest = malloc(sizeof(struct forest));
    newForest->size = forestSize;
    newForest->treeList = malloc(sizeof(struct treeList *) * forestSize);

    for (int i = 0; i < forestSize; i++)
        newForest->treeList[i] = NULL;

    return newForest;
};
