#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_THREADS    2
pthread_mutex_t mutexCounter;

long counter = 0;

void *threadWork(void *input) {
    long *counter = (long *) input;
    printf("Thread starting...\n");

    long i;

    pthread_mutex_lock(&mutexCounter);
    for (i = 0; i < 100000; i++) {
        (*counter)++;
    }
    pthread_mutex_unlock(&mutexCounter);

    printf(" Thread input value %li \n", *counter);

    printf("Thread done.\n");
    pthread_exit((void *) input);
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;

    pthread_mutex_init(&mutexCounter, NULL);
    /* Initialize thread detached attribute */
    pthread_attr_init(&attr);
    /*
     * Set thread detached attribute.
     * The purpose of PTHREAD_CREATE_JOINABLE is that it allows you to call pthread_join on the thread.
     */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int returnCode;
    long i;
    for (i = 0; i < NUM_THREADS; i++) {
        printf("Main: creating thread %ld\n", i);
        returnCode = pthread_create(&thread[i], &attr, threadWork, (void *) &counter);
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
        // printf("Main: completed join with thread %ld having a status of %ld\n", i, (long) status);
    }

    printf("Main: counter value %li \n", counter);
    printf("Main: program completed. Exiting.\n");
    pthread_mutex_destroy(&mutexCounter);
    pthread_exit(NULL);
}

