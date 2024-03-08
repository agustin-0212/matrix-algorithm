#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAX_SIZE 3500

typedef struct {
    int startRow;
    int endRow;
    int N;
    int (*matrixA)[MAX_SIZE];
    int (*matrixB)[MAX_SIZE];
    int (*result)[MAX_SIZE];
} ThreadArgs;

void *multiplyMatrices(void *args) {
    ThreadArgs *targs = (ThreadArgs *)args;

    for (int i = targs->startRow; i < targs->endRow; ++i) {
        for (int j = 0; j < targs->N; ++j) {
            targs->result[i][j] = 0;
            for (int k = 0; k < targs->N; ++k) {
                targs->result[i][j] += targs->matrixA[i][k] * targs->matrixB[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

void printMatrix(int (*matrix)[MAX_SIZE], int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <matrix_size> <num_threads>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int numThreads = atoi(argv[2]);

    if (N <= 0 || numThreads <= 0 || N > MAX_SIZE) {
        printf("Invalid input. N and num_threads should be greater than 0 and less than or equal to %d.\n", MAX_SIZE);
        return 1;
    }

    srand(time(NULL));

    // Allocate memory for matrices
    int (*matrixA)[MAX_SIZE] = malloc(N * sizeof(*matrixA));
    int (*matrixB)[MAX_SIZE] = malloc(N * sizeof(*matrixB));
    int (*result)[MAX_SIZE] = malloc(N * sizeof(*result));

    // Fill matrices A and B with random numbers
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrixA[i][j] = rand() % 9 + 1;  // Random number between 1 and 9
            matrixB[i][j] = rand() % 9 + 1;  // Random number between 1 and 9
        }
    }

    clock_t begin = clock();


    // Create threads
    pthread_t threads[numThreads];
    ThreadArgs threadArgs[numThreads];
    int rowsPerThread = N / numThreads;
    for (int i = 0; i < numThreads; ++i) {
        threadArgs[i].startRow = i * rowsPerThread;
        threadArgs[i].endRow = (i == numThreads - 1) ? N : (i + 1) * rowsPerThread;
        threadArgs[i].N = N;
        threadArgs[i].matrixA = matrixA;
        threadArgs[i].matrixB = matrixB;
        threadArgs[i].result = result;
        // printf("thread %d started with %d rows\n", i, rowsPerThread); 
        pthread_create(&threads[i], NULL, multiplyMatrices, &threadArgs[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    double time_spent = ((double)(end - begin) / CLOCKS_PER_SEC)*1000;

    printf("%f;%d;%d\n", time_spent, N, numThreads);

    free(matrixA);
    free(matrixB);
    free(result);

    return 0;
}
