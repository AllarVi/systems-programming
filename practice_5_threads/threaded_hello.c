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

void *PrintHello(void *threadid) {
    printf("Hello World! It's me, thread #%ld!\n", (long) threadid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];

    int returnCode;

    long threadCount;
    for (threadCount = 0; threadCount < NUM_THREADS; threadCount++) {

        printf("In main: creating thread %ld\n", threadCount);
        returnCode = pthread_create(&threads[threadCount], NULL, PrintHello, (void *) threadCount);
        if (returnCode) {
            printf("ERROR; return code from pthread_create() is %d\n", returnCode);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}
