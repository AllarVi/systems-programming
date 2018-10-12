#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS    2

pthread_mutex_t counterMutex;

long counter = 0; // Shared variable

struct thread_data {
    long *counter;
    char **args;
};

struct thread_data thread_data_array[NUM_THREADS];

void *threadWork(void *input) {
    struct thread_data *threadData = (struct thread_data *) input;
    printf("<Thread> Starting...\n");
    printf(" <Thread> input: %li \n", *threadData->counter);

    printf(" <Thread> Hello, : %s %s \n", threadData->args[1], threadData->args[2]);

    pthread_mutex_lock(&counterMutex);
    for (long i = 0; i < 1000000; i++) {
        (*threadData->counter)++;
    }
    pthread_mutex_unlock(&counterMutex);

    printf(" <Thread> output: %li \n", *threadData->counter);

    printf("<Thread> done.\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) { // an array of char pointers
    for (int i = 1; i < argc; i++) {
        printf("<Main> Args: %s\n", argv[i]);
    }

    pthread_t thread[NUM_THREADS];

    pthread_attr_t attr;
    pthread_attr_init(&attr); // Init thread detached attribute

    /*
     * Set thread detached attribute.
     * The purpose of PTHREAD_CREATE_JOINABLE is that it allows you to call pthread_join on the thread.
     */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&counterMutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data_array->counter = &counter;
        thread_data_array->args = argv;
    }

    int returnCode;
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("<Main> Creating thread: %d\n", i);
        returnCode = pthread_create(&thread[i], &attr, threadWork, (void *) &thread_data_array);

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

    printf("<Main> Counter value %li \n", counter);
    printf("<Main> Program completed. Exiting.\n");

    pthread_mutex_destroy(&counterMutex);
    pthread_exit(NULL);
}

