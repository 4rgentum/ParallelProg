#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv)
{
    const int count = 100000000;     ///< Number of elements in each array
    const int seeds[10] = {920214, 123456, 654321, 789101, 112131, 
                           141516, 171819, 202122, 232425, 262728}; ///< Array seeds
    const int target = 16;          ///< Target value to search for
    const int max_threads = 16;     ///< Maximum number of threads to test
    const int num_arrays = 10;      ///< Number of random arrays to generate

    // Allocate memory for storing multiple arrays
    int** arrays = (int**)malloc(num_arrays * sizeof(int*));
    for (int i = 0; i < num_arrays; i++) {
        arrays[i] = (int*)malloc(count * sizeof(int));
        srand(seeds[i]); ///< Initialize random seed for array generation
        for (int j = 0; j < count; j++) {
            arrays[i][j] = rand(); ///< Fill array with random values
        }
    }

    // Scheduling policies and chunk sizes
    char* schedules[3] = {"static", "dynamic", "guided"}; ///< Schedule types
    int chunks[] = {5, 10, 25, 50};            ///< Different chunk sizes

    double linear_avg_time = 0.0; ///< Linear time with threads = 1

    // ** Unified Schedule Calculation for static, dynamic, and guided **
    for (int s = 0; s < 3; s++) {  // Loop over scheduling policies
        for (int chunk = 0; chunk < 4; chunk++) {  // Loop over chunk sizes
            char filename[100];
            sprintf(filename, "/home/dt/ParProg/lab4/task8/performance_%s_chunk_%d.txt", schedules[s], chunks[chunk]);

            FILE* file = fopen(filename, "w");
            if (!file) {
                printf("Failed to open file for %s schedule with chunk size %d\n", schedules[s], chunks[chunk]);
                return 1;
            }

            fprintf(file, "# Schedule: %s, Chunk Size: %d\n", schedules[s], chunks[chunk]);
            fprintf(file, "# Threads\tAvg Time (s)\tSpeedup\tEfficiency\n");

            // Test thread counts from 1 to max_threads
            for (int threads = 1; threads <= max_threads; threads++) {
                omp_set_num_threads(threads); ///< Set number of OpenMP threads

                double avg_time = 0.0; ///< Average time for parallel search

                for (int arr = 0; arr < num_arrays; arr++) { ///< Loop through arrays
                    int index = -1; ///< Global index of target value
                    int found = 0;  ///< Flag to indicate if target was found

                    double start_time = omp_get_wtime(); ///< Start time for parallel search

                    #pragma omp parallel
                    {
                        int local_index = -1; ///< Local index of the target value

                        // Use appropriate scheduling policy with chunk size
                        #pragma omp for schedule(runtime)
                        for (int i = 0; i < count; i++) {
                            if (found) continue; ///< Stop if target is already found

                            if (arrays[arr][i] == target) { ///< Check for target value
                                local_index = i;

                                #pragma omp atomic write
                                found = 1; ///< Set the global found flag
                            }
                        }

                        // Update the global index in a critical section
                        #pragma omp critical
                        {
                            if (local_index != -1 && (index == -1 || local_index < index)) {
                                index = local_index; ///< Update global index
                            }
                        }
                    }
                    avg_time += (omp_get_wtime() - start_time); ///< Total search time
                }
                avg_time /= num_arrays; ///< Average time for the current test

                // For one thread, save the time as the reference (linear_avg_time)
                if (threads == 1) {
                    linear_avg_time = avg_time;
                }

                // Compute speedup and efficiency
                double speedup = linear_avg_time / avg_time;
                double efficiency = speedup / threads;

                // Write results in tabular format
                fprintf(file, "%d\t%.6f\t%.2f\t%.2f\n",
                        threads, avg_time, speedup, efficiency);

                // Print results to console for real-time monitoring
                printf("Schedule: %s, Chunk: %d, Threads: %d, Avg Time: %.6f s, Speedup: %.2f, Efficiency: %.2f\n",
                       schedules[s], chunks[chunk], threads, avg_time, speedup, efficiency);
            }

            fclose(file);
            printf("Results for %s schedule, chunk size %d saved to %s\n", schedules[s], chunks[chunk], filename);
        }
    }

    // Free allocated memory
    for (int i = 0; i < num_arrays; i++) {
        free(arrays[i]);
    }
    free(arrays);

    printf("All results saved successfully.\n");
    return 0;
}
