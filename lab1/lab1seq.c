#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv) {
    const long long sizes[] = {1000, 10000, 100000, 1000000, 10000000, 100000000}; // Array sizes
    const int experiments = 10;      // Number of experiments per size
    const int random_seed = 920215;  // RNG seed

    long long comparisons;           // Comparison counter
    long long total_comparisons;     // Sum of comparisons for averaging
    double total_time;               // Sum of times for averaging
    int* array = 0;                  // Array for random numbers
    int max = -1;                    // Maximal element
    double start_time, end_time;     // Timing variables

    /* Initialize the RNG */
    srand(random_seed);

    // Открываем файл для записи данных для gnuplot
    FILE *fp = fopen("experiment_results.csv", "w");
    fprintf(fp, "Size,AvgComparisons,AvgTime\n");

    printf("Size\tExperiment\tTime (s)\tComparisons\tMax Value\n");

    /* Loop over different array sizes */
    for(int s = 0; s < sizeof(sizes)/sizeof(sizes[0]); s++) {
        long long size = sizes[s];

        total_comparisons = 0; // Initialize the total comparisons for averaging
        total_time = 0.0;      // Initialize the total time for averaging

        for (int exp = 0; exp < experiments; exp++) {

            /* Allocate and fill the array with random values */
            array = (int*)malloc(size * sizeof(int));
            for(int i = 0; i < size; i++) { array[i] = rand(); }

            comparisons = 0;   // Reset comparisons count for each experiment
            max = -1;          // Reset max value before each experiment

            /* Start timing */
            start_time = omp_get_wtime();

            /* Sequential computation */
            for(int i = 0; i < size; i++) {
                comparisons++;
                if(array[i] > max) { max = array[i]; }
            }

            /* End timing */
            end_time = omp_get_wtime();

            /* Add the current experiment's time and comparisons to totals */
            total_time += (end_time - start_time);
            total_comparisons += comparisons;

            /* Output the results for the current experiment */
            printf("%lld\t%d\t%f\t%lld\t%d\n", size, exp + 1, end_time - start_time, comparisons, max);

            free(array); // Free the allocated array
        }

        /* Compute and output the average time and comparisons for the current size */
        printf("Average for size %lld: Avg Time = %f s, Avg Comparisons = %lld\n", 
                size, total_time / experiments, total_comparisons / experiments);

        // Записываем данные в CSV для Gnuplot
        fprintf(fp, "%lld %lld %f\n", size, total_comparisons / experiments, total_time / experiments);
    }

    fclose(fp); // Закрываем файл
    return 0;
}
