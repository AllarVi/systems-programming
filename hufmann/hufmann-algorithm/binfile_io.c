//
// Created by Allar Viinamäe on 13/11/2018.
//

#include "binfile_io.h"
#include <stdlib.h>

typedef FILE BITFILE;
typedef FILE OUTPUT_BITFILE;

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

void forceFlush(struct OUTPUT_BITFILE *bitFile) {
    int bufferLength = (int) strlen(bitFile->buffer);
    int extraBitsNeeded = 8 - bufferLength;

    char *byteToWrite = (char *) malloc(9 * sizeof(char));

    for (int i = 0; i < bufferLength; i++) {
        byteToWrite[i] = bitFile->buffer[i];
    }

    for (int i = bufferLength; i < (bufferLength + extraBitsNeeded); i++) {
        byteToWrite[i] = '0';
    }

    byteToWrite[8] = '\0';

    printf("final byte to write: %s\n", byteToWrite);

    char charToWrite = strtol(byteToWrite, 0, 2);
    fputc(charToWrite, bitFile->filePtr);
}

int putBits(char *encodedChar, struct OUTPUT_BITFILE *bitFile) {
    int encodedCharLength = (int) strlen(encodedChar);
    int bufferLength = (int) strlen(bitFile->buffer);

    char *bitsToBuffer = (char *) malloc((encodedCharLength + bufferLength + 1) * sizeof(char));
    strcpy(bitsToBuffer, bitFile->buffer);
    strcat(bitsToBuffer, encodedChar);

    bitFile->buffer = bitsToBuffer; // Update buffer

    while (strlen(bitFile->buffer) >= 8) {
        // printf("%d bits in buffer\n", (int) strlen(bitFile->buffer));
        char *byteToWrite = (char *) malloc(9 * sizeof(char));
        substr(bitFile->buffer, 8, 0, byteToWrite);

        bufferLength = (int) strlen(bitFile->buffer);
        char *bitsLeftToBuffer = (char *) malloc((bufferLength - 7) * sizeof(char));

        substr(bitFile->buffer, bufferLength - 8, 8, bitsLeftToBuffer);

        // printf("Byte to write %s\n", byteToWrite);
        char charToWrite = strtol(byteToWrite, 0, 2);
        fputc(charToWrite, bitFile->filePtr);

        // printf("Bits left to buffer %s\n", bitsLeftToBuffer);

        bitFile->buffer = bitsLeftToBuffer;
    }
    int bitsLeftInBuffer = (int) strlen(bitFile->buffer);
    // printf("%d bits left in buffer\n", bitsLeftInBuffer);

    if (bitsLeftInBuffer > 0) {
        return 1;
    }
    return 0;
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