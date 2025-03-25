#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void shell_sort(int *arr, size_t size) {
    for (size_t gap = size / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < size; i++) {
            int temp = arr[i];
            size_t j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

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

void generate_arrays(int **arrays, size_t count, const int seeds[], size_t num_arrays) {
    for (int i = 0; i < num_arrays; i++) {
        srand(seeds[i]);
        for (size_t j = 0; j < count; j++) {
            arrays[i][j] = rand();
        }
    }
}

void create_special_array(int *arr, size_t size, int type, int seed) {
    srand(seed);
    int sorted_percentage = (type - 1) * 20 - 100;

    if (sorted_percentage < 0) {
        size_t sorted_count = (size_t)(((100 + sorted_percentage) / 100.0) * size);
        for (size_t i = 0; i < size - sorted_count; i++) {
            arr[i] = size - i;
        }
        for (size_t j = size - sorted_count; j < size; j++) {
            arr[j] = rand();
        }
    }
    else {
        size_t sorted_count = (size_t)((sorted_percentage / 100.0) * size);
        for (size_t i = 0; i < sorted_count; i++) {
            arr[i] = i;
        }
        if (sorted_percentage != 100) {
            for (size_t j = sorted_count; j < size; j++) {
                arr[j] = rand();
            }
        }
    }
}

int main() {
    const int count = 10000000;
    const int seeds[10] = {920214, 123456, 654321, 789101, 112131,
                           141516, 171819, 202122, 232425, 262728};
    const int num_arrays = 10;
    const int runs_per_array = 10;
    double *spec_time = (double *)malloc(10 * sizeof(double));

    int **arrays = (int **)malloc(num_arrays * sizeof(int *));
    if (!arrays) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    for (int i = 0; i < num_arrays; i++) {
        arrays[i] = (int *)malloc(count * sizeof(int));
        if (!arrays[i]) {
            printf("Memory allocation failed.\n");
            return 1;
        }
    }

    generate_arrays(arrays, count, seeds, num_arrays);

    FILE *output_random = fopen("/home/dt/ParProg/lab3/random_results.csv", "w");
    FILE *output_special = fopen("/home/dt/ParProg/lab3/special_results.csv", "w");
    
    if (!output_random || !output_special) {
        printf("Failed to open file for writing.\n");
        return 1;
    }

    fprintf(output_random, "Threads\tAvgTime\tSpeedup\tEfficiency\n");
    fprintf(output_special, "Threads\t-100\t-80\t-60\t-40\t-20\t0\t20\t40\t60\t80\t100\n");

    // Sequential sorting
    double sequential_time_random = 0.0;
    double sequential_time_special = 0.0;

    for (int arr_idx = 0; arr_idx < num_arrays; arr_idx++) {
        double total_time_per_array = 0.0;
        for (int run = 0; run < runs_per_array; run++) {
            int *copy = (int *)malloc(count * sizeof(int));
            if (!copy) {
                printf("Memory allocation failed.\n");
                return 1;
            }
            memcpy(copy, arrays[arr_idx], count * sizeof(int));

            double start_time = omp_get_wtime();
            shell_sort(copy, count); // Sequential sorting
            double end_time = omp_get_wtime();

            total_time_per_array += (end_time - start_time);
            free(copy);
        }
        sequential_time_random += total_time_per_array / runs_per_array;
    }

    sequential_time_random /= num_arrays;

    for (int threads = 1; threads <= 16; threads++) {
        double total_time_random = 0.0, total_time_special = 0.0;

        // Parallel sorting of random arrays
        for (int arr_idx = 0; arr_idx < num_arrays; arr_idx++) {
            double total_time_per_array = 0.0;
            for (int run = 0; run < runs_per_array; run++) {
                int *copy = (int *)malloc(count * sizeof(int));
                if (!copy) {
                    printf("Memory allocation failed.\n");
                    return 1;
                }
                memcpy(copy, arrays[arr_idx], count * sizeof(int));

                double start_time = omp_get_wtime();
                parallel_shell_sort(copy, count, threads);
                double end_time = omp_get_wtime();

                total_time_per_array += (end_time - start_time);
                free(copy);
            }

            double avg_time_per_array = total_time_per_array / runs_per_array;
            total_time_random += avg_time_per_array;
        }

        // Parallel sort for special type
        for (int type = 1; type <= 11; type++) {
            double total_time_per_special = 0.0;
            for (int i = 0; i < 10; i++) {
                int *special_array = (int *)malloc(count * sizeof(int));
                if (!special_array) {
                    printf("Memory allocation failed.\n");
                    return 1;
                }
                create_special_array(special_array, count, type, seeds[i]);

                double start_time = omp_get_wtime();
                parallel_shell_sort(special_array, count, threads);
                double end_time = omp_get_wtime();

                #pragma omp critical
                total_time_per_special += (end_time - start_time);

                free(special_array);
            }

            total_time_special = total_time_per_special / 10;
            spec_time[type - 1] = total_time_special;
        }

        double avg_time_random = total_time_random / num_arrays;
        double speedup_random = sequential_time_random / avg_time_random;
        double efficiency_random = speedup_random / threads;

        // Write results
        fprintf(output_random, "%d\t%.6f\t%.6f\t%.6f\n", threads, avg_time_random, speedup_random, efficiency_random);
        fprintf(output_special, "%d", threads);
        for (int i = 0; i < 11; i++) {
            fprintf(output_special, "\t%.6f", spec_time[i]);
        }
        fprintf(output_special, "\n");
    }

    fclose(output_random);
    fclose(output_special);

    for (int i = 0; i < num_arrays; i++) {
        free(arrays[i]);
    }
    free(arrays);

    free(spec_time);

    return 0;
}
