//
// Created by Allar Viinamäe on 09/11/2018.
//

#ifndef HUFMANN_ALGORITHM_MAP_H
#define HUFMANN_ALGORITHM_MAP_H

struct map *createMap(int size);

void put(struct map *myMap, int key, int val);

int get(struct map *myMap, int key);

#endif //HUFMANN_ALGORITHM_MAP_H


