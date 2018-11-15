//
// Created by Allar Viinamäe on 13/11/2018.
//

#include <stdio.h>
#include <string.h>

#ifndef HUFMANN_ALGORITHM_BINFILE_IO_H
#define HUFMANN_ALGORITHM_BINFILE_IO_H

struct BITFILE {
    unsigned int *buffer;
    int *counter;
    FILE *filePtr;
};

struct OUTPUT_BITFILE {
    char *buffer;
    FILE *filePtr;
};

struct BITFILE *bitOpen(FILE *f);        /*initialise bit input or output*/

int getBit(struct BITFILE *bitFile);     /*get one bit from bf->buffer */

int putBits(char *encodedChar, struct OUTPUT_BITFILE *bitFile);

void forceFlush(struct OUTPUT_BITFILE *bitFile);

void closeOutput(struct BITFILE *bf);        /*finish writing. Flush the last bits to bf->file*/

#endif //HUFMANN_ALGORITHM_BINFILE_IO_H
