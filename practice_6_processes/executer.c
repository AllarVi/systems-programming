#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE

#include <unistd.h>

int main(int arcg, char **argv) {
    char *stopperArgv[3] = {"stopper", "-s", NULL};

    execvp("./stopper", stopperArgv);

    return EXIT_SUCCESS;
} 
