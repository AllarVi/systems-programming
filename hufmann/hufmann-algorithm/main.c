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

void printFileType(struct stat *fileStat);

void printFileSize(struct stat *fileStat);

void readFileByChar(FILE *filePtr, unsigned char **fileContents);

void groupByChars(size_t fileSize, const unsigned char *fileContents, struct map **map);

void getTwoMinTrees(struct forest *forest, struct tree **minTree1, struct tree **minTree2);

struct tree *combineTrees(struct tree *minTree1, struct tree *minTree2);

struct forest *updateForest(const struct forest *forest, struct tree *combinedTree);

// Tree traversal
void traversePaths(struct tree *tree, char **encodingTable, int *maxPathLen);

void recTraversePaths(struct tree *tree, int data, char **encodingTable, int *path, int pathLen, int *maxPathLen);

void addPath(const int *ints, int len, int c, char **encodingTable);

void encode(const unsigned char *fileContents, size_t fileSize, char *outputFilePath, char **encodingTable);

void decode(const char *filePath, size_t fileSize, struct tree *encodingTree, const char *decompressedFilePath,
            int fullTreeBits);

void recGetDecodedChar(struct tree *tree, unsigned char *decodable, int *decodedChar);

unsigned char getBitAsChar(int aBit);

struct map *collectFrequencies(size_t fileSizeWithEOF, const unsigned char *fileContents);

char **getEncodingTable(struct tree *encodingTree);

unsigned char *getFileContents(FILE *filePtr, size_t fileSizeWithEOF);

int printEncodingTable(char *const *encodingTable);

void putTree(unsigned char *treeEncoding, char *outputFilePath);

struct tree *getDecodingTree(char *filePath);

struct BITFILE *getInputBitFile(char *filePath);

unsigned char *getBitAsString(struct BITFILE *inputBitFile);

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
        // perror("stat");
        // exit(EXIT_FAILURE);
    }

    printFileType(&fileStat);
    printFileSize(&fileStat);

    FILE *filePtr = fopen(filePath, "r");
    if (filePtr == NULL) {
        printf("<ERROR> File doesn't exist!\n");
    }

    size_t fileSize = (size_t) (fileStat).st_size;
    size_t fileSizeWithEOF = fileSize;

    size_t outFileSize = (size_t) (outFileStat).st_size;

    unsigned char *fileContents = getFileContents(filePtr, fileSizeWithEOF);

    fileContents[fileSizeWithEOF] = 255; // Add EOF char
    printf("EOF char: %c, code: %d\n", fileContents[fileSizeWithEOF], fileContents[fileSizeWithEOF]);
    printf("fileContents: %s\n", fileContents);

    struct map *fileCharFreq = collectFrequencies(fileSizeWithEOF, fileContents);

    struct tree *encodingTree = getTree(fileSizeWithEOF, fileCharFreq);

    char **encodingTable = getEncodingTable(encodingTree);

    printf("Encoding table: \n");
    int encodingTableSize = printEncodingTable(encodingTable);

    unsigned char *treeEncoding = (unsigned char *) malloc(
            (encodingTableSize * 3) * sizeof(unsigned char)); // 3 as @ and two leaves
    encodeTree(encodingTree, treeEncoding);
    printf("Encoded tree: %s\n", treeEncoding);

    // TODO: get decoding tree
    // 4*1 + 5*9 = 49 bits
    // 49 + 9 end bits = 58
    // total 56 + 8 = 64 bits = 8 bytes

    if (decodeFlag == 1) { // DECODING
        struct tree *decodingTree = getDecodingTree(filePath);
        char **decodingTable = getEncodingTable(decodingTree);

        printf("Decoding table: \n");
        int decodingTableSize = printEncodingTable(decodingTable);
        printf("Decoding table size: %d\n", decodingTableSize);
        int decodedTreeSize = decodingTableSize - 1 + decodingTableSize * 9 + 9;
        int extraTreeBits = 8 - (decodedTreeSize % 8);
        int fullTreeBits = decodedTreeSize + extraTreeBits;

        printf("Decoded tree size %d\n", fullTreeBits);
        decode(filePath, fileSize, decodingTree, outFilePath, fullTreeBits);
    } else { // ENCODING
        putTree(treeEncoding, outFilePath);
        encode(fileContents, fileSize, outFilePath, encodingTable);
    }

    return 0;
}

struct tree *recGetDecodingTree(struct BITFILE *bitFile);

unsigned char *getOneByte(struct BITFILE *bitFile);

struct tree *getDecodingTree(char *filePath) {
    struct BITFILE *compressedBitFile = getInputBitFile(filePath);

    printf("Decoded tree: \n");
    struct tree *node = recGetDecodingTree(compressedBitFile);
    printf("\n");

    fclose(compressedBitFile->filePtr);

    return node;
}

struct tree *recGetDecodingTree(struct BITFILE *bitFile) {
    struct tree *node = malloc(sizeof(struct tree));

    unsigned char *bitAsString = getBitAsString(bitFile);

    if (bitAsString[0] == '1') {
        node->c = 0;
        printf("@");
        node->left = recGetDecodingTree(bitFile);
        node->right = recGetDecodingTree(bitFile);
    } else if (bitAsString[0] == '0') {
        unsigned char *character = getOneByte(bitFile);
        if (strcmp("00000000", (char *) character) == 0) {
            return NULL;
        }
        unsigned char i = strtol((char *) character, 0, 2);
        node->c = i;
        printf("%c", i);
    }

    return node;
}

unsigned char *getOneByte(struct BITFILE *bitFile) {
    unsigned char *character = malloc(9 * sizeof(unsigned char));

    for (int i = 0; i < 8; i++) {
        int bit = getBit(bitFile);

        character[i] = getBitAsChar(bit);
    }

    character[8] = '\0';

    return character;
}

unsigned char *getBitAsString(struct BITFILE *inputBitFile) {
    unsigned char *bitAsString = (unsigned char *) malloc(2 * sizeof(unsigned char));

    int bit = getBit(inputBitFile);

    bitAsString[0] = getBitAsChar(bit);
    bitAsString[1] = '\0';

    return bitAsString;
}

struct BITFILE *getInputBitFile(char *filePath) {
    struct BITFILE *inputBitFile = malloc(sizeof(struct BITFILE));

    inputBitFile->filePtr = fopen(filePath, "rb");

    int *counter = (int *) malloc(sizeof(int));
    *counter = -1;
    inputBitFile->counter = counter;
    return inputBitFile;
}

void
putTree(unsigned char *treeEncoding, char *outputFilePath) {
    struct OUTPUT_BITFILE *outputBitFile = malloc(sizeof(struct BITFILE));
    unsigned char *emptyBuffer = (unsigned char *) malloc(sizeof(unsigned char));
    emptyBuffer[0] = '\0';

    outputBitFile->buffer = emptyBuffer;
    outputBitFile->filePtr = fopen(outputFilePath, "wb");

    for (int i = 0; i < strlen((char *) treeEncoding); i++) {
        if (treeEncoding[i] == '@') {
            putBits("1", outputBitFile);
            printf("char: %c, encoding: %s\n", treeEncoding[i], "1");
        } else {
            unsigned char *encodedChar = (unsigned char *) malloc(10 * sizeof(unsigned char));
            encodedChar[0] = '0';

            unsigned char c = treeEncoding[i];
            for (int j = 1; j < 9; j++) {
                int k = j - 1;

                unsigned char *charAsByte = malloc(sizeof(unsigned char));
                *charAsByte = c;
                *charAsByte = *charAsByte << k;

                int bitOfAChar = *charAsByte & 128;

                if (bitOfAChar == 0) {
                    encodedChar[j] = '0';
                } else {
                    encodedChar[j] = '1';
                }
            }
            encodedChar[9] = '\0';

            putBits((char *) encodedChar, outputBitFile);
            printf("char: %c, encoding: %s\n", treeEncoding[i], encodedChar);
        }
    }
    if (putBits("000000000", outputBitFile) == 1) {
        forceFlush(outputBitFile);
    }

    fclose(outputBitFile->filePtr);
}

int printEncodingTable(char *const *encodingTable) {
    int encodingTableSize = 0;
    for (int i = 0; i < CHAR; i++) {
        if (encodingTable[i] != NULL) {
            printf("%d   %c          %s\n", i, i, encodingTable[i]);
            encodingTableSize++;
        }
    }
    printf("Encoding table size: %d\n", encodingTableSize);
    return encodingTableSize;
}

unsigned char *getFileContents(FILE *filePtr, size_t fileSizeWithEOF) {
    unsigned char *fileContents = malloc(fileSizeWithEOF);
    readFileByChar(filePtr, &fileContents);
    return fileContents;
}

char **getEncodingTable(struct tree *encodingTree) {
    char **encodingTable = calloc(CHAR + 1, sizeof(char *)); // calloc zeros the allocated memory
    if (!encodingTable) {
        perror("calloc encodingTable");
        exit(EXIT_FAILURE);
    };

    int maxPathLen = 0;
    traversePaths(encodingTree, encodingTable, &maxPathLen);
    return encodingTable;
}

struct map *collectFrequencies(size_t fileSizeWithEOF, const unsigned char *fileContents) {
    struct map *fileCharFreq = createMap((int) fileSizeWithEOF);
    groupByChars(fileSizeWithEOF, fileContents, &fileCharFreq);

    int differentChars = getEntriesTotal(fileCharFreq);
    printf("<INFO> Different characters: %d\n", differentChars);
    return fileCharFreq;
}

void recGetDecodedChar(struct tree *tree, unsigned char *decodable, int *decodedChar) {
    int decodableLength = (int) strlen((char *) decodable);

    char bit = decodable[0];

    if (tree->left == NULL && tree->right == NULL) {
        *decodedChar = tree->c;
        return;
    } else {
        if (decodableLength < 1) {
            *decodedChar = 0;
            return;
        }

        unsigned char *decodableTail = (unsigned char *) malloc(decodableLength * sizeof(unsigned char));
        substr(decodable, decodableLength - 1, 1, decodableTail);

        if (bit == '0') {
            recGetDecodedChar(tree->left, decodableTail, decodedChar);
        } else {
            recGetDecodedChar(tree->right, decodableTail, decodedChar);
        }
    }
}

void decode(const char *filePath, size_t fileSize, struct tree *encodingTree, const char *decompressedFilePath,
            int fullTreeBits) {
    struct BITFILE *inputBitFile = malloc(sizeof(struct BITFILE));

    inputBitFile->filePtr = fopen(filePath, "rb"); // Open for reading in binary mode.
    FILE *decompressedFilePtr = fopen(decompressedFilePath, "w");

    int *counter = (int *) malloc(sizeof(int));
    *counter = -1;
    inputBitFile->counter = counter;

    for (int i = 0; i < fullTreeBits; i++) {
        getBit(inputBitFile);
    }

    int aBit = getBit(inputBitFile);

    unsigned char *decodable = (unsigned char *) malloc(2 * sizeof(unsigned char));
    decodable[0] = getBitAsChar(aBit);
    decodable[1] = '\0';

    for (int i = 1; i < (int) fileSize * 8; i++) {
        int *decodedChar = (int *) malloc(sizeof(int));
        *decodedChar = 0;

        // printf("decodable %s\n", decodable);
        recGetDecodedChar(encodingTree, decodable, decodedChar);

        if (*decodedChar == 255) {
            break;
        }

        int currentDecodableLength;
        unsigned char *newDecodable;

        // printf("decodedChar %d\n", *decodedChar);
        if (*decodedChar == 0) { // decoded bit == 0
            currentDecodableLength = (int) strlen((char *) decodable);
            newDecodable = (unsigned char *) malloc((currentDecodableLength + 1) * sizeof(unsigned char));
            strcpy((char *) newDecodable, (char *) decodable);
        } else { // decoded bit = 1
            printf("%c", (unsigned char) *decodedChar);
            fputc((unsigned char) *decodedChar, decompressedFilePtr);

            currentDecodableLength = 0;
            newDecodable = (unsigned char *) malloc((currentDecodableLength + 1) * sizeof(unsigned char));
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

unsigned char getBitAsChar(int aBit) { // itoa?
    if (aBit == 0) {
        return (unsigned char) '0';
    } else {
        return (unsigned char) '1';
    }
}

void
encode(const unsigned char *fileContents, size_t fileSize, char *outputFilePath, char **encodingTable) {
    struct OUTPUT_BITFILE *outputBitFile = malloc(sizeof(struct BITFILE));
    unsigned char *emptyBuffer = (unsigned char *) malloc(sizeof(unsigned char));
    emptyBuffer[0] = '\0';

    outputBitFile->buffer = emptyBuffer;
    outputBitFile->filePtr = fopen(outputFilePath, "ab+");

    for (int i = 0; i < fileSize; i++) {
        char *encodedChar = encodingTable[fileContents[i]];
        // printf("char: %c, encoding: %s\n", fileContents[i], encodedChar);

        putBits(encodedChar, outputBitFile);
    }
    if (putBits(encodingTable[255], outputBitFile) == 1) {
        forceFlush(outputBitFile);
    }

    fclose(outputBitFile->filePtr);
}

void traversePaths(struct tree *tree, char **encodingTable, int *maxPathLen) {
    int path[CHAR];

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


void groupByChars(size_t fileSize, const unsigned char *fileContents, struct map **map) {
    struct map *fileCharFreq = *map;

    for (int i = 0; i < fileSize + 1; i++) {
        unsigned char charInFile = fileContents[i];
        if (get(fileCharFreq, charInFile) != -1) { // Char exists
            int cCount = get(fileCharFreq, charInFile);
            cCount++;
            put(fileCharFreq, charInFile, cCount);
        } else {
            put(fileCharFreq, charInFile, 1);
        }
        // printf("<INFO> Character in file: %c, ascii: %d, count %d\n", charInFile, charInFile, get(fileCharFreq, charInFile));
    }
}

void readFileByChar(FILE *filePtr, unsigned char **fileContents) {
    unsigned char *contents = *fileContents; // dereference

    int c;
    size_t n = 0;
    while ((c = fgetc(filePtr)) != EOF) {
        contents[n++] = (unsigned char) c;
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
