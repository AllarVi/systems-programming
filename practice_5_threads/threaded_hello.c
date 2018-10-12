/******************************************************************************
* FILE: hello.c
* DESCRIPTION:
*   A "hello world" Pthreads program.  Demonstrates thread creation and
*   termination.
* AUTHOR: Blaise Barney
* LAST REVISED: 08/09/11
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS    5

/*struct thread_data{
   int  thread_id;
   int  sum;
   char *message;
};

struct thread_data thread_data_array[NUM_THREADS];*/

void *printHello(void *threadId) {
    printf("Hello World! It's me, thread #%ld!\n", (long) threadId);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];

    int returnCode;
    for (long i = 0; i < NUM_THREADS; i++) {
        printf("In main: creating thread %ld\n", i);
        returnCode = pthread_create(&threads[i], NULL, printHello, (void *) i);

        if (returnCode) {
            printf("ERROR; return code from pthread_create() is %d\n", returnCode);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}
