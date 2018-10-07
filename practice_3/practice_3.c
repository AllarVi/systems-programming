#include <stdio.h>
#include <stdlib.h>

// Incrementing int pointers shifts the pointer by 4 bytes, because int requires 4 bytes to store.

// Segmentation fault
// https://kb.iu.edu/d/aqsj

void printMemoryAddressOfInt(int *intPtr, char printPrefix[]);

void printMemoryAddressOfString(char (*strPtr)[]);

void foo(int argument);

void printStack(int *ptr);

int main() {
    printf("Practice 3! \n");

    // note that there is a difference between the lines:
    // char s[] = "foo:asdasdasd"; // is allocated on the stack
    // char *s = "bar:dsadsadas";  // is not allocated on the stack
    char string[] = "random";
    printMemoryAddressOfString(&string);

    int firstInt = 111;
    printMemoryAddressOfInt(&firstInt, "main()");

    int secondInt = 222;
    // printMemoryAddressOfInt(&secondInt, "main()");

    int thirdInt = 333;
    // printMemoryAddressOfInt(&thirdInt, "main()");

    foo(0);

    printStack(&firstInt);

    return 0;
}

void printStack(int *ptr) {
    int i = 0;
    while (i < 10) {
    // while (1) {
        printf("\nprintStack() memory: %p, decimal: %d, hexadecimal: %08x, char: %c \n", ptr, *ptr, *ptr, *ptr);
        ptr++;
        i++;
    }
}

void foo(int argument) {
    int fourthInt = 4;
    printMemoryAddressOfInt(&fourthInt, "foo()");

    int *fourthIntPtr = &fourthInt;

    int *beforeFourthIntPtr = fourthIntPtr;
    int *afterFourthIntPtr = fourthIntPtr;

    --beforeFourthIntPtr;
    ++afterFourthIntPtr;

    printf("Before int: %d - int: %x, address: %p \n", fourthInt, *beforeFourthIntPtr, beforeFourthIntPtr);

    printf("After int: %d - int: %x, address: %p \n", fourthInt, *afterFourthIntPtr, afterFourthIntPtr);

    int *tryToGetThirdInt = fourthIntPtr;

    for (int i = 0; i < 8; ++i) {
        ++tryToGetThirdInt;
    }

    printf("\nfoo() Third int: %x, address: %p \n", *tryToGetThirdInt, tryToGetThirdInt);

    ++tryToGetThirdInt;

    printf("\nfoo() Second int: %x, address: %p \n", *tryToGetThirdInt, tryToGetThirdInt);

    ++tryToGetThirdInt;

    printf("\nfoo() First int: %x, address: %p \n", *tryToGetThirdInt, tryToGetThirdInt);

    ++tryToGetThirdInt;

    // since signed char storage size = 1 byte
    char *charPtr = (char *) tryToGetThirdInt;
    ++charPtr; // will increment by one byte

    printf("\nfoo() String string: %s, address: %p \n", charPtr, charPtr);

    int fifthInt = 5;
    printMemoryAddressOfInt(&fifthInt, "foo()");
};

void printMemoryAddressOfInt(int *intPtr, char printPrefix[]) {
    printf("\n");
    printf("%s Int: %d, address: %p \n", printPrefix, *intPtr, intPtr);
};

void printMemoryAddressOfString(char (*strPtr)[]) {
    printf("\n");
    printf("String: %s, address: %p \n", *strPtr, strPtr);
};
