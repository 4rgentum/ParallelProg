#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void parallel_shell_sort(int *arr, size_t size, int threads) {
    for (size_t gap = size / 2; gap > 0; gap /= 2) {
        #pragma omp parallel for num_threads(threads)
        for (size_t start = 0; start < gap; start++) {
            for (size_t i = start + gap; i < size; i += gap) {
                int temp = arr[i];
                size_t j;
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                    arr[j] = arr[j - gap];
                }
                arr[j] = temp;
            }
        }
    }
}

int main() {
    const int count = 100000000;
    const int seed = 920214;

    int *array = (int *)malloc(count * sizeof(int));
    if (!array) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    srand(seed);
    for (size_t j = 0; j < count; j++) {
        array[j] = rand();
    }

    FILE *output = fopen("/home/dt/ParProg/lab6/omp_results.csv", "w");

    if (!output) {
        printf("Failed to open file for writing.\n");
        return 1;
    }

    fprintf(output, "# Threads\tAvgTime\tSpeedup\tEfficiency\n");

    double sequential_time = 0.0;

    for (int threads = 1; threads <= 16; threads++) {

        // Parallel sorting
        int *copy = (int *)malloc(count * sizeof(int));
        if (!copy) {
            printf("Memory allocation failed.\n");
            return 1; 
        }
        memcpy(copy, array, count * sizeof(int));

        double start_time = omp_get_wtime();
        parallel_shell_sort(copy, count, threads);
        double end_time = omp_get_wtime();

        double time = end_time - start_time;

        free(copy);

        if (threads == 1) {
            sequential_time = time;
        }

        double speedup = sequential_time / time;
        double efficiency = speedup / threads;

        // Write results
        fprintf(output, "%d\t%.6f\t%.6f\t%.6f\n", threads, time, speedup, efficiency);
    }

    fclose(output);

    free(array);

    return 0;
}