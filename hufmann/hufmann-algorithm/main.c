#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "tree.h"
#include "binfile_io.h"

#define CHAR 256

// Tree traversal inspiration: https://www.geeksforgeeks.org/given-a-binary-tree-print-all-root-to-leaf-paths/

static const int CHAR_PATH_LEN = 500;

void printFileType(struct stat *fileStat);

void printFileSize(struct stat *fileStat);

void readFileByChar(FILE *filePtr, char **fileContents);

void groupByChars(size_t fileSize, const char *fileContents, struct map **map);

void getTwoMinTrees(struct forest *forest, struct tree **minTree1, struct tree **minTree2);

struct tree *combineTrees(struct tree *minTree1, struct tree *minTree2);

struct forest *packTree(struct forest *forest);

struct forest *updateForest(const struct forest *forest, struct tree *combinedTree);

void printSizeValidation(size_t fileSize, const struct forest *packedForest);

// Tree traversal
void traversePaths(struct tree *tree, char **encodingTable, int *maxPathLen);

void recTraversePaths(struct tree *tree, int data, char **encodingTable, int *path, int pathLen, int *maxPathLen);

void addPath(const int *ints, int len, int c, char **encodingTable);

void encode(const char *fileContents, char **encodingTable);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Second program argument not specified!\n");
        exit(EXIT_SUCCESS);
    }

    char *filePath = argv[1];

    printf("filePath %s\n", filePath);

    struct stat fileStat;
    if (stat(filePath, &fileStat) == -1) { // Call stat() and save results to fileStat
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printFileType(&fileStat);
    printFileSize(&fileStat);

    FILE *filePtr = fopen(filePath, "r");
    if (filePtr == NULL) {
        printf("<ERROR> File doesn't exist!\n");
    }

    size_t fileSize = (size_t) (fileStat).st_size;

    char *fileContents = malloc(fileSize);
    readFileByChar(filePtr, &fileContents);

    struct map *fileCharFreq = createMap((int) fileSize);

    groupByChars(fileSize, fileContents, &fileCharFreq);

    printf("a:         %d\n", get(fileCharFreq, 'a'));
    printf("b:         %d\n", get(fileCharFreq, 'b'));
    printf("c:         %d\n", get(fileCharFreq, 'c'));
    printf("d:         %d\n", get(fileCharFreq, 'd'));
    printf("space:     %d\n", get(fileCharFreq, ' '));
    printf("new lines: %d\n", get(fileCharFreq, '\n'));

    int numOfDifferentChars = getEntriesTotal(fileCharFreq);
    printf("Num of different characters: %d\n", numOfDifferentChars);

    struct forest *forest = makeForest(fileCharFreq);

    printf("Forest size: %d\n", forest->size);

    struct forest *packedForest = packTree(forest);

    printSizeValidation(fileSize, packedForest);

    struct tree *encodingTree = packedForest->treeList[0];

    size_t nb_menu = CHAR;

    char **encodingTable = calloc(nb_menu + 1, sizeof(char *));
    if (!encodingTable) {
        perror("calloc encodingTable");
        exit(EXIT_FAILURE);
    };

    int maxPathLen = 0;
    traversePaths(encodingTree, encodingTable, &maxPathLen);

    printf("Encoding table: \n");
    int count = 0;
    for (int i = 0; i < CHAR; i++) {
        if (encodingTable[i] != NULL) {
            printf("%d   %c          %s\n", i, i, encodingTable[i]);
            count++;
        }
    }
    printf("Elements in table: %d, max path length: %d\n", count, maxPathLen);

    // INPUT
    struct BITFILE *inputBitFile = malloc(sizeof(struct BITFILE));

    inputBitFile->filePtr = fopen(filePath, "rb"); // Open for reading in binary mode.

    int *counter = (int *) malloc(sizeof(int));
    *counter = -1;
    inputBitFile->counter = counter;

    int cnt = 0;
    int bitsInFile = (int) fileSize * 8;
    for (int i = 0; i < bitsInFile; i++) {
        int aBit = getBit(inputBitFile);
        printf("%d", aBit);
        if (cnt == 7) {
            printf("\n");
            cnt = 0;
        } else {
            cnt++;
        }
    }
    printf("\n");
    fclose(inputBitFile->filePtr);

    // OUTPUT
    // encode(fileContents, encodingTable);

    return 0;
}

void encode(const char *fileContents, char **encodingTable) {
    struct BITFILE *outputBitFile = malloc(sizeof(struct BITFILE));

    outputBitFile->filePtr = fopen("output.bin", "wb");

    int *outputBitFileCounter = (int *) malloc(sizeof(int));
    *outputBitFileCounter = -1;
    outputBitFile->counter = outputBitFileCounter;

    char *encodedChar = encodingTable[fileContents[0]];
    printf("encodedChar %s\n", encodedChar);

    putBits(encodingTable, encodedChar, outputBitFile);

    fclose(outputBitFile->filePtr);
}

void traversePaths(struct tree *tree, char **encodingTable, int *maxPathLen) {
    int path[CHAR_PATH_LEN];

    recTraversePaths(tree, -1, encodingTable, path, 0, maxPathLen);
}

void recTraversePaths(struct tree *tree, int data, char **encodingTable, int *path, int pathLen, int *maxPathLen) {
    if (tree == NULL)
        return;

    // append this node to the path array
    if (data != -1) {
        path[pathLen] = data;
        pathLen++;
    }

    if (tree->left == NULL && tree->right == NULL) {
        if (*maxPathLen < pathLen) {
            *maxPathLen = pathLen;
        }
        addPath(path, pathLen, tree->c, encodingTable);
    } else {
        recTraversePaths(tree->left, 0, encodingTable, path, pathLen, maxPathLen);
        recTraversePaths(tree->right, 1, encodingTable, path, pathLen, maxPathLen);
    }
}

void addPath(const int *ints, const int len, const int c, char **encodingTable) {
    // Int array to string
    char encoding[len];
    int n = 0;
    for (int i = 0; i < len; i++) {
        n += sprintf(&encoding[n], "%d", ints[i]);
    }

    // New pointer for string
    char *key;
    key = (char *) malloc(len * sizeof(char));
    strcpy(key, encoding);

    encodingTable[c] = key;
}

void printSizeValidation(size_t fileSize, const struct forest *packedForest) {
    if (fileSize == packedForest->treeList[0]->freq) {
        printf("<INFO> Packed forest size matches file size\n");
    } else {
        printf("<WARNING> Packed forest size does not match file size!\n");
    }
}

struct forest *packTree(struct forest *forest) {
    struct forest *currentForest = forest;

    while (currentForest->size > 1) {
        struct tree *minTree1;
        struct tree *minTree2;
        getTwoMinTrees(currentForest, &minTree1, &minTree2);

        if (minTree2 != NULL) {
            // printf("Forest min: %c %d\n", minTree1->c, minTree1->freq);
            // printf("Forest min: %c %d\n", minTree2->c, minTree2->freq);

            struct tree *combinedTree = combineTrees(minTree1, minTree2);

            // printf("Combined tree: %c %d\n", combinedTree->c, combinedTree->freq);

            currentForest = updateForest(currentForest, combinedTree);

            // printf("<DEBUG> Updated forest size: %d\n", currentForest->size);
        }
    }

    return currentForest;
}

struct forest *updateForest(const struct forest *forest, struct tree *combinedTree) {
    int newSize = forest->size - 1;

    struct forest *updatedForest = malloc(sizeof(struct forest));
    updatedForest->size = newSize;
    updatedForest->treeList = malloc(sizeof(struct treeList *) * newSize);

    updatedForest->treeList[0] = combinedTree;
    for (int i = 1; i < newSize; i++) {
        updatedForest->treeList[i] = forest->treeList[i + 1];
    }

    return updatedForest;
}

struct tree *combineTrees(struct tree *minTree1, struct tree *minTree2) {
    struct tree *combinedTree = malloc(sizeof(struct tree));
    combinedTree->left = minTree1;
    combinedTree->right = minTree2;
    combinedTree->c = 0;
    combinedTree->freq = minTree1->freq + minTree2->freq;
    return combinedTree;
}

void getTwoMinTrees(struct forest *forest, struct tree **minTree1, struct tree **minTree2) {
    if (forest->size < 2) {
        (*minTree1) = forest->treeList[0];
        (*minTree2) = NULL;
        return;
    }

    bubbleSort(forest);

    (*minTree1) = forest->treeList[0];
    (*minTree2) = forest->treeList[1];
}


void groupByChars(size_t fileSize, const char *fileContents, struct map **map) {
    struct map *fileCharFreq = *map;

    for (int i = 0; i < fileSize; i++) {
        char charInFile = fileContents[i];
        if (get(fileCharFreq, charInFile) != -1) { // Char exists
            int cCount = get(fileCharFreq, charInFile);
            cCount++;
            put(fileCharFreq, charInFile, cCount);
        } else {
            put(fileCharFreq, charInFile, 1);
        }
    }
}

void readFileByChar(FILE *filePtr, char **fileContents) {
    char *contents = *fileContents; // dereference

    int c;
    size_t n = 0;
    while ((c = fgetc(filePtr)) != EOF) {
        contents[n++] = (char) c;
    }

    printf("Num of characters: %d\n", (int) n);
    // terminate with the null character
    contents[n] = '\0';
}

void printFileSize(struct stat *fileStat) {
    printf("File size:                %lld bytes\n",
           (long long) (*fileStat).st_size);
}

void printFileType(struct stat *fileStat) {
    printf("File type:                ");

    switch ((*fileStat).st_mode & S_IFMT) { // Compare with type of file mask
        case S_IFBLK:
            printf("block device\n");
            break;
        case S_IFCHR:
            printf("character device\n");
            break;
        case S_IFDIR:
            printf("directory\n");
            break;
        case S_IFIFO:
            printf("FIFO/pipe\n");
            break;
        case S_IFLNK:
            printf("symlink\n");
            break;
        case S_IFREG:
            printf("regular file\n");
            break;
        case S_IFSOCK:
            printf("socket\n");
            break;
        default:
            printf("unknown?\n");
            break;
    }
}
