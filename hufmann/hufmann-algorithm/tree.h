#ifndef HUFMANN_ALGORITHM_TREE_H
#define HUFMANN_ALGORITHM_TREE_H

#include "map.h"

struct forest {
    int size;
    struct tree **treeList;
};

struct tree {
    /* if left==0, it is a leaf. Otherwise it has two branches */
    struct tree *left;
    struct tree *right;   /* not  in a leaf */
    int c;                /* only in a leaf */
    int freq;             /* occurence rate of the tree */
};


struct tree *getTree(size_t fileSizeWithEOF, struct map *fileCharFreq);

struct forest *packTree(struct forest *forest);

void printSizeValidation(size_t fileSize, const struct forest *packedForest);

struct forest *makeForest(struct map *fileCharFreq);

unsigned char *encodeTree(struct tree *tree, unsigned char *treeEncoding);

void swap(struct tree **xp, struct tree **yp);

void bubbleSort(struct forest *forest);

#endif //HUFMANN_ALGORITHM_TREE_H
