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

struct forest *makeForest(struct map *fileCharFreq);

#endif //HUFMANN_ALGORITHM_TREE_H
