#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv) 
{
    const int count = 100000000;     // Number of array elements
    const int threads_max = 16;      // Maximum number of parallel threads to use
    const int num_arrays = 10;       // Number of experiments per thread count

    int** arrays;                   // Array of pointers for random number arrays (now dynamic)
    int  max;                        // Maximal element
    double start_time, end_time;     // Timing variables
    double total_time;               // Sum of times for averaging
    double time_seq;                 // Time for sequential equal for 1 thread
    double avg_time;                 // Average time for thread
    double speedup, efficiency;      // Speedup and efficiency variables

    /* Define 10 fixed random seeds */
    const int seeds[10] = {123456, 789012, 345678, 901234, 567890, 
                           112233, 445566, 778899, 990011, 223344};

    /* Open file to write results for Gnuplot */
    FILE *fp = fopen("/home/dt/ParProg/lab5/solve/omp_results.csv", "w");
    if (!fp) {
        perror("Unable to open file");
        return 1;
    }

    fprintf(fp, "# threads \t Avg Time (s) \t Speedup \t Efficiency\n");

    /* Allocate memory for the array of pointers dynamically */
    arrays = (int**)malloc(num_arrays * sizeof(int*));

    /* Generate 10 different arrays with fixed random seeds */
    for (int arr = 0; arr < num_arrays; arr++) {
        /* Allocate memory for each array dynamically */
        arrays[arr] = (int*)malloc(count * sizeof(int));

        /* Set a fixed seed for each array */
        srand(seeds[arr]);

        /* Fill the array with random values */
        for(int i = 0; i < count; i++) {
            arrays[arr][i] = rand();
        }
    }

    /* Perform experiments for different number of threads */
    for(int threads = 1; threads <= threads_max; threads++) {

        total_time = 0.0; // Reset total time for each thread count

        for (int arr = 0; arr < num_arrays; arr++) {

            max = -1; // Reset max value before each experiment

            /* Start timing */
            start_time = omp_get_wtime();

            /* Parallel computation using pre-generated array */
            #pragma omp parallel num_threads(threads) shared(arrays, count, arr) reduction(max: max) default(none)
            {
                #pragma omp for
                for(int i = 0; i < count; i++) {
                    if(arrays[arr][i] > max) { max = arrays[arr][i]; }
                }
            }

            /* End timing */
            end_time = omp_get_wtime();
            /* Add the current array's time */
            total_time += (end_time - start_time);
        }

        /* Compute and output the average time for the current threads */
        avg_time = total_time / num_arrays;
        printf("Average for threads %d: Avg Time = %f\n", threads, avg_time);

        if (threads == 1) {
            time_seq = avg_time;
        }

        speedup  = time_seq / avg_time;
        efficiency = speedup / threads;

        /* Write results to CSV file */
        fprintf(fp, "%d\t%f\t%f\t%f\n", threads, avg_time, speedup, efficiency);
    }

    /* Free all allocated arrays after all experiments */
    for (int arr = 0; arr < num_arrays; arr++) {
        free(arrays[arr]);
    }

    /* Free the array of pointers */
    free(arrays);
    
    /* Close the file */
    fclose(fp);

    return 0;
}
