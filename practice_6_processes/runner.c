#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>

int main(int arcg, char **argv) {
    char *stopperArgv[3] = {"stopper", "-s", NULL};

    // On success, the PID of the child process is returned in the parent,
    // and 0 is returned in the child.
    pid_t pid = fork(); // duplicates the calling process

    if (pid == 0) {
        // this is executed in the child process
        execvp("./stopper", stopperArgv);
    }

    int waitpid_status;

    waitpid(pid, &waitpid_status, 0); // parent waits for the child to end

    printf("runner: Stopped");

    return EXIT_SUCCESS;
} 
