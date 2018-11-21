//
// Bubble-sort inspiration: https://www.geeksforgeeks.org/bubble-sort/
//

#include <stdbool.h>
#include "tree.h"
#include <stdio.h>
#include "string.h"

struct tree *getTree(size_t fileSizeWithEOF, struct map *fileCharFreq) {
    struct forest *forest = makeForest(fileCharFreq);

    struct forest *packedForest = packTree(forest);
    printSizeValidation(fileSizeWithEOF, packedForest);

    // *** Encoding tree ***
    struct tree *encodingTree = packedForest->treeList[0]; // Get encoding tree from forest

    return encodingTree;
}

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

void printSizeValidation(size_t fileSize, const struct forest *packedForest) {
    if (fileSize == packedForest->treeList[0]->freq) {
        printf("<INFO> Packed forest size matches file size\n");
    } else {
        printf("<WARNING> Packed forest size does not match file size!\n");
    }
}

char *encodeTree(struct tree *tree, char *treeEncoding) {

    if (tree->left == NULL && tree->right == NULL) {
        char *el = (char *) malloc(2 * sizeof(char));
        el[0] = (char) tree->c;
        el[1] = '\0';
        printf("Reached leaf with %s\n", el);
        // strcat(treeEncoding, el);
        return el;
    } else {
        strcat(treeEncoding, "@");
    }

    strcat(treeEncoding, encodeTree(tree->left, treeEncoding));
    strcat(treeEncoding, encodeTree(tree->right, treeEncoding));

    return "";
}

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
