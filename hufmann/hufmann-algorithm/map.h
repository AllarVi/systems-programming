//
// Created by Allar Viinamäe on 09/11/2018.
//

#ifndef HUFMANN_ALGORITHM_MAP_H
#define HUFMANN_ALGORITHM_MAP_H

#include <stdlib.h>

struct map {
    int size;
    struct node **list; // list of nodes
};

struct node {
    int key;
    int val;
    struct node *next;
};

struct map *createMap(int size);

void put(struct map *myMap, int key, int val);

int get(struct map *myMap, int key);

int getNumOfDiffChars(const struct map *fileCharFreq);

#endif //HUFMANN_ALGORITHM_MAP_H


