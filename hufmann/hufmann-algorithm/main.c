#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "map.h"
#include "tree.h"
#include "binfile_io.h"

#define CHAR 256

// Tree traversal inspiration: https://www.geeksforgeeks.org/given-a-binary-tree-print-all-root-to-leaf-paths/

static const int CHAR_PATH_LEN = 1024;

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

void encode(const char *fileContents, size_t fileSize, char *outputFilePath, char **encodingTable);

void decode(const char *filePath, size_t fileSize, struct tree *encodingTree, const char *decompressedFilePath);

void recGetDecodedChar(struct tree *tree, char *decodable, int *decodedChar);

char getBitAsChar(int aBit);

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Two program arguments should be given at least!\n");
        exit(EXIT_SUCCESS);
    }

    int decodeFlag = 0;
    int c;

    while ((c = getopt(argc, argv, "d::")) != -1)
        switch (c) {
            case 'd':
                decodeFlag = 1;
                break;
            default:
                abort();
        }

    printf("<INFO> decodeFlag: %d\n", decodeFlag);

    char *filePath;
    char *outFilePath;
    char *decompressedFilePath = "decompressed.txt";
    if (decodeFlag == 1) {
        filePath = argv[2];
        outFilePath = argv[3];
        decompressedFilePath = argv[4];
    } else {
        filePath = argv[1];
        outFilePath = argv[2];
    }

    printf("<INFO> Input file: %s\n", filePath);
    printf("<INFO> Output file: %s\n", outFilePath);

    struct stat fileStat;
    if (stat(filePath, &fileStat) == -1) { // Call stat() and save results to fileStat
        perror("stat");
        exit(EXIT_FAILURE);
    }

    struct stat outFileStat;
    if (stat(outFilePath, &outFileStat) == -1) {
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
    size_t outFileSize = (size_t) (outFileStat).st_size;

    char *fileContents = malloc(fileSize + 1);
    readFileByChar(filePtr, &fileContents);

    fileContents[fileSize + 1] = '\0';

    struct map *fileCharFreq = createMap((int) fileSize + 1);

    groupByChars(fileSize + 1, fileContents, &fileCharFreq);

    // printf("a:         %d\n", get(fileCharFreq, 'a'));
    printf("end of file:         %d\n", get(fileCharFreq, '\0'));

    int numOfDifferentChars = getEntriesTotal(fileCharFreq);
    printf("Num of different characters: %d\n", numOfDifferentChars);

    struct forest *forest = makeForest(fileCharFreq);
    printf("Forest size: %d\n", forest->size);

    struct forest *packedForest = packTree(forest);
    printSizeValidation(fileSize + 1, packedForest);

    struct tree *encodingTree = packedForest->treeList[0];

    char **encodingTable = calloc(CHAR + 1, sizeof(char *)); // calloc zeros the allocated memory
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
    printf("Encoding table size: %d, max path length: %d\n", count, maxPathLen);

    if (decodeFlag == 1) { // DECODING
        decode(outFilePath, outFileSize, encodingTree, decompressedFilePath);
    } else { // ENCODING
        char *endOfFileChar = (char *) malloc((maxPathLen + 2) * sizeof(char)); // max length + one more + \0
        for (int i = 0; i < (maxPathLen + 1); i++) {
            endOfFileChar[i] = '0';
        }
        endOfFileChar[maxPathLen + 1] = '\0';

        encode(fileContents, fileSize, outFilePath, encodingTable);
    }

    return 0;
}

void recGetDecodedChar(struct tree *tree, char *decodable, int *decodedChar) {
    int decodableLength = (int) strlen(decodable);

    char bit = decodable[0];

    if (tree->left == NULL && tree->right == NULL) {
        *decodedChar = tree->c;
        return;
    } else {
        if (decodableLength < 1) {
            *decodedChar = 0;
            return;
        }

        char *decodableTail = (char *) malloc(decodableLength * sizeof(char));
        substr(decodable, decodableLength - 1, 1, decodableTail);

        if (bit == '0') {
            recGetDecodedChar(tree->left, decodableTail, decodedChar);
        } else {
            recGetDecodedChar(tree->right, decodableTail, decodedChar);
        }
    }
}

void decode(const char *filePath, size_t fileSize, struct tree *encodingTree, const char *decompressedFilePath) {
    struct BITFILE *inputBitFile = malloc(sizeof(struct BITFILE));

    inputBitFile->filePtr = fopen(filePath, "rb"); // Open for reading in binary mode.
    FILE *decompressedFilePtr = fopen(decompressedFilePath, "w");

    int *counter = (int *) malloc(sizeof(int));
    *counter = -1;
    inputBitFile->counter = counter;

    int count = 2;
    int aBit = getBit(inputBitFile);

    char *decodable = (char *) malloc(count * sizeof(char));
    decodable[0] = getBitAsChar(aBit);
    decodable[1] = '\0';

    for (int i = 1; i < (int) fileSize * 8; i++) {
        int *decodedChar = (int *) malloc(sizeof(int));
        *decodedChar = 0;

        // printf("decodable %s\n", decodable);
        recGetDecodedChar(encodingTree, decodable, decodedChar);

        int currentDecodableLength;
        char *newDecodable;
        if (*decodedChar == 0) {
            currentDecodableLength = (int) strlen(decodable);
            newDecodable = (char *) malloc((currentDecodableLength + 1) * sizeof(char));
            strcpy(newDecodable, decodable);
        } else {
            printf("%c", (char) *decodedChar);
            fputc((char) *decodedChar, decompressedFilePtr);

            currentDecodableLength = 0;
            newDecodable = (char *) malloc((currentDecodableLength + 1) * sizeof(char));
        }

        aBit = getBit(inputBitFile);

        newDecodable[currentDecodableLength] = getBitAsChar(aBit);
        newDecodable[currentDecodableLength + 1] = '\0';

        // printf("new decodable %s\n", newDecodable);

        decodable = newDecodable;
    }
    printf("\n");
    fclose(inputBitFile->filePtr);
    fclose(decompressedFilePtr);
}

char getBitAsChar(int aBit) {
    if (aBit == 0) {
        return '0';
    } else {
        return '1';
    }
}

void
encode(const char *fileContents, size_t fileSize, char *outputFilePath, char **encodingTable) {
    struct OUTPUT_BITFILE *outputBitFile = malloc(sizeof(struct BITFILE));
    char *emptyBuffer = (char *) malloc(sizeof(char));
    emptyBuffer[0] = '\0';

    outputBitFile->buffer = emptyBuffer;
    outputBitFile->filePtr = fopen(outputFilePath, "wb");

    for (int i = 0; i < fileSize; i++) {
        char *encodedChar = encodingTable[fileContents[i]];
        // printf("char: %c, encoding: %s\n", fileContents[i], encodedChar);

        putBits(encodedChar, outputBitFile);
    }
    if (putBits(encodingTable[0], outputBitFile) == 1) {
        forceFlush(outputBitFile);
    }

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
            struct tree *combinedTree = combineTrees(minTree1, minTree2);

            currentForest = updateForest(currentForest, combinedTree);
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
    contents[n] = '\0';
}

void printFileSize(struct stat *fileStat) {
    printf("<INFO> File size:                %lld bytes\n",
           (long long) (*fileStat).st_size);
}

void printFileType(struct stat *fileStat) {
    printf("<INFO> File type:                ");

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
