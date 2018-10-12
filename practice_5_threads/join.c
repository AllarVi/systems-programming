#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS    2

pthread_mutex_t counterMutex;

long counter = 0; // Shared variable

void *threadWork(void *input) {
    long *counter = (long *) input;
    printf("<Thread> Starting...\n");
    printf(" <Thread> input: %li \n", *counter);

    pthread_mutex_lock(&counterMutex);
    for (long i = 0; i < 1000000; i++) {
        (*counter)++;
    }
    pthread_mutex_unlock(&counterMutex);

    printf(" <Thread> output: %li \n", *counter);

    printf("<Thread> done.\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];

    pthread_attr_t attr;
    pthread_attr_init(&attr); // Init thread detached attribute

    /*
     * Set thread detached attribute.
     * The purpose of PTHREAD_CREATE_JOINABLE is that it allows you to call pthread_join on the thread.
     */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&counterMutex, NULL);

    int returnCode;
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("<Main> Creating thread %d\n", i);
        returnCode = pthread_create(&thread[i], &attr, threadWork, (void *) &counter);

        if (returnCode) {
            printf("ERROR; return code from pthread_create() is %d\n", returnCode);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr); // Free the attribute when no longer needed

    // Wait for the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        returnCode = pthread_join(thread[i], NULL);

        if (returnCode) {
            printf("ERROR; return code from pthread_join() is %d\n", returnCode);
            exit(-1);
        }
    }

    printf("Main: counter value %li \n", counter);
    printf("Main: program completed. Exiting.\n");

    pthread_mutex_destroy(&counterMutex);
    pthread_exit(NULL);
}

