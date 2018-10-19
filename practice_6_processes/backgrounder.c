#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>

int main(int arcg, char **argv) {
    char *stopperArgv[3] = {"stopper", "-s", NULL};

    pid_t pid = fork();

    if (pid == 0) {
        execvp("./stopper", stopperArgv);
    }

    //int waitpid_status;

    // waitpid(pid, &waitpid_status, 0); // parent waits for the child to end

    printf("backgrounder: my PID: %d and my child PID: %D", getpid(), pid);

    return EXIT_SUCCESS;
} 
