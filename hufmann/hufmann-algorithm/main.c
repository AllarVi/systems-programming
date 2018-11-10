#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

void printFileType(struct stat *fileStat);

void printFileSize(struct stat *fileStat);

void readFileByChar(FILE *filePtr, char **fileContents);

void groupByChars(size_t fileSize, const char *fileContents, struct map **map);

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
    put(fileCharFreq, 'a', 10);

    groupByChars(fileSize, fileContents, &fileCharFreq);

    printf("a:         %d\n", get(fileCharFreq, 'a'));
    printf("b:         %d\n", get(fileCharFreq, 'b'));
    printf("c:         %d\n", get(fileCharFreq, 'c'));
    printf("d:         %d\n", get(fileCharFreq, 'd'));
    printf("space:     %d\n", get(fileCharFreq, ' '));
    printf("new lines: %d\n", get(fileCharFreq, '\n'));

    int numOfDifferentChars = getNumOfDiffChars(fileCharFreq);
    printf("Num of different characters: %d\n", numOfDifferentChars);

    return 0;
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
