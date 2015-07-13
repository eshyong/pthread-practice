#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_ITERATIONS 3000
#define MILLISECOND 1000

sem_t semaphore;
volatile int turn = 0;

struct pthread_args {
    int which;
    char to_print;
};

// This function will attempt to print arg as a char type.
// If arg is not of type char *, strange things will happen.
// Not sure how to enforce type arguments in C.
void *print_char(void *arg) {
    int i = 0;
    struct pthread_args *in_args = (struct pthread_args *)arg;
    while (i < NUM_ITERATIONS) {
        while (turn != in_args->which) {
            // Sleep for one millisecond while it's not our turn.
            usleep(MILLISECOND);
        }
        // Lock semaphore, so that only this function can print to the console.
        sem_wait(&semaphore);
        printf("%c ", in_args->to_print);

        // The turn variable should be shared between pthreads 0, 1, and 2.
        turn = (turn + 1) % 3;
        i += 1;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[3];
    int status;

    // Turn off newline buffering.
    setbuf(stdout, NULL);

    // Create a semaphore. All created threads will lock this when
    // they wish to print.
    // Don't allow process sharing (pshared = 0), initialize value to 1 so that
    // only one thread can lock the semaphore at a time.
    sem_init(&semaphore, 0, 1);

    // Create three threads that print "a", "b", and "c".
    struct pthread_args args_1 = ((struct pthread_args) {0, 'a'});
    status = pthread_create(&threads[0], NULL, print_char, (void *)&args_1);
    if (status != 0) {
        // TODO: If any one pthread_create call fails, there might be leftover
        // pthreads from previous calls. These if statements should call a
        // cleanup function and exit gracefully.
        exit(1);
    }
    struct pthread_args args_2 = ((struct pthread_args) {1, 'b'});
    status = pthread_create(&threads[0], NULL, print_char, (void *)&args_2);
    if (status != 0) {
        exit(1);
    }
    struct pthread_args args_3 = ((struct pthread_args) {2, 'c'});
    status = pthread_create(&threads[0], NULL, print_char, (void *)&args_3);
    if (status != 0) {
        exit(1);
    }

    // Wait for threads to finish.
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    printf("\n");

    // Release semaphore memory.
    sem_destroy(&semaphore);

    return 0;
} 
