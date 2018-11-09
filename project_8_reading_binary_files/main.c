#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

int main(int argc, char **argv) {
    int cFlag = 0;
    int rFlag = 0;
    int nFlag = 0;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "cr:n:")) != -1)
        switch (c) {
            case 'c':
                cFlag = 1;
                break;
            case 'r':
                rFlag = 1;
                break;
            case 'n':
                nFlag = 1;
                break;
            default:
                abort();
        }

    printf("cFlag = %d, rFlag = %d, nFlag = %d\n", cFlag, rFlag, nFlag);

    char *filePath = argv[1];

    FILE *filePtr;
    int valueFromFile = 0;

    // If the file does not exist, fopen() returns NULL.
    filePtr = fopen(filePath, "rb"); // Open for reading in binary mode.
    if (filePtr != NULL) {
        printf("Reading file value");

        fread(&valueFromFile, sizeof(valueFromFile), 1, filePtr);
        fclose(filePtr);

        printf("Old value from file: %d\n", valueFromFile);
    } else {
        printf("File doesn't exist, creating...\n");
    }

    int newValue;
    if (valueFromFile != 0) {
        newValue = valueFromFile;
        newValue++;
    } else {
        newValue = 1;
    }

    filePtr = fopen(filePath, "wb");

    fwrite(&newValue, sizeof(newValue), 1, filePtr);
    fclose(filePtr);

    filePtr = fopen(filePath, "rb");

    fread(&valueFromFile, sizeof(valueFromFile), 1, filePtr);
    fclose(filePtr);

    printf("runtimes: %d\n", valueFromFile);

    return 0;
}