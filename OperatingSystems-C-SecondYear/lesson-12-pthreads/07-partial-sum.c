// File: parallel_sum.c
// Compile with: gcc -Wall -pthread parallel_sum.c -o parallel_sum

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 20

int arr[ARRAY_SIZE];
int partial_sums[NUM_THREADS]; // each thread stores its partial sum

// Thread function: Each thread computes the sum of a portion of the array.
void* compute_partial_sum(void *arg) {
    int thread_id = *(int *)arg;
    int start = thread_id * (ARRAY_SIZE / NUM_THREADS);
    int end = start + (ARRAY_SIZE / NUM_THREADS);
    int sum = 0;

    printf("Thread %d: processing elements from index %d to %d (values: ", thread_id, start, end - 1);
    for (int i = start; i < end; i++) {
        printf("%d", arr[i]);
        if (i < end - 1) printf(", ");
        sum += arr[i];
    }
    printf(")\n");

    partial_sums[thread_id] = sum;
    printf("Thread %d: computed partial sum = %d\n", thread_id, sum);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int status;
    int total_sum = 0;

    // Initialize the array with values 1 to ARRAY_SIZE.
    printf("Initializing array with values: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = i + 1;
        printf("%d", arr[i]);
        if (i < ARRAY_SIZE - 1) printf(", ");
    }
    printf("\n\n");

    // Create threads: Each thread computes a part of the sum.
    printf("Creating %d threads to compute partial sums...\n\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        status = pthread_create(&threads[i], NULL, compute_partial_sum, (void *)&thread_ids[i]);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed for thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Main thread: collected partial sum from thread %d = %d\n", i, partial_sums[i]);
    }

    // Calculate the total sum.
    for (int i = 0; i < NUM_THREADS; i++) {
        total_sum += partial_sums[i];
    }

    printf("\nTotal sum of the array = %d\n", total_sum);
    return EXIT_SUCCESS;
}
