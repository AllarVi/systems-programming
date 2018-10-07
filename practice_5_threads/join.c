/*****************************************************************************
* FILE: join.c
* DESCRIPTION:
*   This example demonstrates how to "wait" for thread completions by using
*   the Pthread join routine.  Threads are explicitly created in a joinable
*   state for portability reasons. Use of the pthread_exit status argument is
*   also shown. Compare to detached.c
* AUTHOR: 8/98 Blaise Barney
* LAST REVISED:  01/30/09
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_THREADS    2

void *threadWork(void *t) {
    long tid = (long) t;
    printf("Thread %ld starting...\n", tid);

    printf("Thread %ld done. Result = %s\n", tid, "Hello, World!");
    pthread_exit((void *) t);
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int returnCode;

    /* Initialize thread detached attribute */
    pthread_attr_init(&attr);
    /* Set thread detached attribute
    The purpose of "PTHREAD_CREATE_JOINABLE" is that it allows you to call pthread_join on the thread */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    long i;
    for (i = 0; i < NUM_THREADS; i++) {
        printf("Main: creating thread %ld\n", i);
        returnCode = pthread_create(&thread[i], &attr, threadWork, (void *) i);
        if (returnCode) {
            printf("ERROR; return code from pthread_create() is %d\n", returnCode);
            exit(-1);
        }
    }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);

    void *status;
    for (i = 0; i < NUM_THREADS; i++) {
        returnCode = pthread_join(thread[i], &status);
        if (returnCode) {
            printf("ERROR; return code from pthread_join() is %d\n", returnCode);
            exit(-1);
        }
        printf("Main: completed join with thread %ld having a status of %ld\n", i, (long) status);
    }

    printf("Main: program completed. Exiting.\n");
    pthread_exit(NULL);
}

