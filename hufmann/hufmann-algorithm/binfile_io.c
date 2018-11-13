//
// Created by Allar Viinamäe on 13/11/2018.
//

#include "binfile_io.h"
#include <stdlib.h>

typedef FILE BITFILE;

void substr(const char *encodedChar, int length, int position, char *sub);

int getBit(struct BITFILE *bitFile) {
    if (*bitFile->counter == -1) {
        int intFromFile = fgetc(bitFile->filePtr);
        if (intFromFile == EOF) {
            return -1;
        }

        unsigned int *charFromFile = (unsigned int *) malloc(sizeof(unsigned char));
        *charFromFile = (unsigned char) intFromFile;

        bitFile->buffer = charFromFile;

        int *counter = (int *) malloc(sizeof(int));
        *counter = 7;
        bitFile->counter = counter;
    }

    unsigned char byteToRead = *bitFile->buffer;
    // printf("Byte to read %d\n", byteToRead);

    int toShift = *bitFile->counter;

    // printf("to shift %d\n", toShift);
    byteToRead = byteToRead >> toShift;

    // printf("Byte to read %d\n", byteToRead);

    *bitFile->counter = *bitFile->counter - 1;

    if ((byteToRead & 1) == 0) {
        return 0;
    }
    return 1;
}

void putBits(char **encodingTable, char *encodedChar, struct BITFILE *bitFile) {
    int length = (int) strlen(encodedChar);
    char *substringToWrite = (char *) malloc(9 * sizeof(char));

    if (length > 8) {
        char *substringToBuffer = (char *) malloc((length - 7) * sizeof(char));

        substr(encodedChar, 8, 0, substringToWrite);
        substr(encodedChar, length, 8, substringToBuffer);

        printf("Substring to write %s\n", substringToWrite);
        printf("Substring to buffer %s\n", substringToBuffer);
    }

    char charToWrite = strtol(substringToWrite, 0, 2);

    printf("Char to write %d\n", charToWrite);

    fputc(charToWrite, bitFile->filePtr);
}

void substr(const char *encodedChar, int length, int position, char *sub) {
    int c = 0;
    char substring[length];

    while (c < length) {
        substring[c] = encodedChar[position + c];
        c++;
    }
    substring[c] = '\0';

    strcpy(sub, substring);
}