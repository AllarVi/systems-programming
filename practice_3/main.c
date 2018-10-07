#include <stdio.h>
#include <stdlib.h>

int const STACKSIZE = 10;

typedef struct {
    int size;
    int items[STACKSIZE];
} STACK;

void printStackItems(STACK *stackPtr);

void push(STACK *stackPtr, int newStackItem);

int pop(STACK *stackPtr);

int main() {
    printf("Stack example! \n");

    // Initialize the stack
    STACK myStack;
    STACK *stackPtr = &myStack;

    push(stackPtr, 5);
    push(stackPtr, 7);
    push(stackPtr, 22);

    printStackItems(stackPtr);

    pop(stackPtr);

    printStackItems(stackPtr);

    return 0;
}

void push(STACK *stackPtr, int newStackItem) {
    if (stackPtr->size == STACKSIZE) {
        fputs("Error: stackoverflow\n", stderr);
        abort();
    } else
        stackPtr->items[stackPtr->size++] = newStackItem;
};

int pop(STACK *stackPtr) {
    if (stackPtr->size == 0) {
        fputs("Error: stackunderflow\n", stderr);
        abort();
    } else
        return stackPtr->items[--stackPtr->size];
}

void printStackItems(STACK *stackPtr) {
    printf("\n");
    printf("Stack %p items:\n", stackPtr);
    int i;
    for (i = 0; i < (*stackPtr).size; i++) {
        printf("  %d: %d \n", i, (*stackPtr).items[i]);
    }
}

