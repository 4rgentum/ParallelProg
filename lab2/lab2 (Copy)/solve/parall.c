#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    const int seeds[10] = {920214, 123456, 654321, 789101, 112131, 
                           141516, 171819, 202122, 232425, 262728};
    const int target = 16;          ///< Number to look for
    const int max_threads = 16;     ///< Maximum number of threads to test
    const int num_arrays = 10;      ///< Number of random arrays
    const int runs_per_array = 10;  ///< Number of runs per array

    int* array = 0;                 ///< Array for searching
    FILE* output_file = fopen("/home/dt/ParProg/lab2/solve/performance_data.txt", "w");

    if (!output_file) {
        printf("Failed to open file for writing.\n");
        return 1;
    }

    //fprintf(output_file, "# Threads\tTime (s)\tSpeedup\tEfficiency\n");

    double linear_avg_time = 0.0;   ///< Average time for linear search

    // Создание массива для хранения данных
    int** arrays = (int**)malloc(num_arrays * sizeof(int*));
    for (int i = 0; i < num_arrays; i++) {
        arrays[i] = (int*)malloc(count * sizeof(int));
        if (!arrays[i]) {
            printf("Memory allocation failed.\n");
            return 1;
        }

        // Генерация массива с использованием уникального seed
        srand(seeds[i]);
        for (int j = 0; j < count; j++) {
            arrays[i][j] = rand();
        }
    }

    // Линейный проход
    for (int arr = 0; arr < num_arrays; arr++) {
        double total_time = 0.0;

        for (int run = 0; run < runs_per_array; run++) {
            double start_time = omp_get_wtime();
            int index_linear = -1;

            for (int i = 0; i < count; i++) {
                if (arrays[arr][i] == target) {
                    index_linear = i;
                    break;
                }
            }

            total_time += (omp_get_wtime() - start_time);
        }

        linear_avg_time += total_time / runs_per_array;
    }

    linear_avg_time /= num_arrays;
    printf("Linear average time: %.6f seconds.\n", linear_avg_time);

    // Тестирование с потоками от 1 до max_threads
    for (int threads = 1; threads <= max_threads; threads++) {
        omp_set_num_threads(threads);

        double avg_time = 0.0;

        for (int arr = 0; arr < num_arrays; arr++) {
            double total_time = 0.0;

            for (int run = 0; run < runs_per_array; run++) {
                int index = -1;
                int found = 0;

                double start_time = omp_get_wtime();

                #pragma omp parallel
                {
                    int local_index = -1;

                    #pragma omp for
                    for (int i = 0; i < count; i++) {
                        if (found) continue;

                        if (arrays[arr][i] == target) {
                            local_index = i;

                            #pragma omp atomic write
                            found = 1;
                        }
                    }

                    // Обновление общего индекса
                    if (local_index != -1) {
                        #pragma omp critical
                        {
                            if (index == -1 || local_index < index) {
                                index = local_index;
                            }
                        }
                    }
                }

                total_time += (omp_get_wtime() - start_time);
            }

            avg_time += total_time / runs_per_array;
        }

        avg_time /= num_arrays;

        // Вычисление ускорения и эффективности
        double speedup = linear_avg_time / avg_time;
        double efficiency = speedup / threads;

        printf("Threads: %d, Avg Time: %.6f s, Speedup: %.2f, Efficiency: %.2f\n",
               threads, avg_time, speedup, efficiency);

        fprintf(output_file, "%d\t %.6f\t %.2f\t %.2f\n",
                threads, avg_time, speedup, efficiency);
    }

    // Освобождение памяти
    for (int i = 0; i < num_arrays; i++) {
        free(arrays[i]);
    }
    free(arrays);

    fclose(output_file);

    printf("Performance data saved to 'performance_data.txt'.\n");
    return 0;
}
