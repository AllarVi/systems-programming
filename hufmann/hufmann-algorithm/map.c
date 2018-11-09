//
// Inspiration from: http://www.kaushikbaruah.com/posts/data-structure-in-c-hashmap/
//

#include "map.h"

#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    int val;
    struct node *next;
};

struct map {
    int size;
    struct node **list; // list of nodes
};

struct map *createMap(int size) {
    struct map *newMap = (struct map *) malloc(sizeof(struct map));
    newMap->size = size;
    newMap->list = (struct node **) malloc(sizeof(struct node *) * size);

    for (int i = 0; i < size; i++)
        newMap->list[i] = NULL;

    return newMap;
}

int getHashCode(struct map *m, int key) {
    // key as ascii value of char
    if (key < 0)
        return -(key % m->size);

    // if key = 'a' (97) and size = 2916
    // then hash == 97
    // Note: if key > size, then map overwriting occurs
    // Return the position of key in map
    return key % m->size;
}

void put(struct map *myMap, int key, int val) {
    int keyPosition = getHashCode(myMap, key);

    // gets old node at map position, initially NULL
    struct node *oldNode = myMap->list[keyPosition];

    struct node *temp = oldNode;
    while (temp) {
        if (temp->key == key) {
            temp->val = val; // update old value
            return;
        }
        temp = temp->next; // doesn't reach, goes to next leaf
    }

    // allocating space in memory with the size of struct node
    // casting that space into struct node
    // returning pointer to new node
    struct node *newNode = (struct node *) malloc(sizeof(struct node));

    // reach here in case no previous node found
    newNode->key = key;
    newNode->val = val;
    newNode->next = oldNode;

    // put new node into map
    myMap->list[keyPosition] = newNode;
}

int get(struct map *myMap, int key) {
    int keyPosition = getHashCode(myMap, key);

    struct node *oldNode = myMap->list[keyPosition];

    while (oldNode) {
        if (oldNode->key == key) {
            return oldNode->val; // return value
        }
        oldNode = oldNode->next;
    }

    return -1;
}
