#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    const int count = 100000000;     ///< Number of array elements
    const int random_seed = 920215; ///< RNG seed
    const int num_experiments = 10; ///< Number of experiments

    int *array = NULL;              ///< The array we need to find the max in
    int local_max = -1;             ///< The maximal element in the local process
    int global_max = -1;            ///< The global maximal element after reduction
    int rank, size;                ///< MPI rank and size

    double total_time = 0.0;        ///< Total time for averaging
    double start_time, end_time;    ///< Timing variables
    double avg_time;                ///< Average time

    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Initialize the RNG (only for the root process) */
    if (rank == 0) {
        srand(random_seed);
        array = (int*)malloc(count * sizeof(int));

        /* Generate the random array (only on root process) */
        for (int i = 0; i < count; i++) {
            array[i] = rand();
        }
    }

    /* Divide the work among processes */
    int local_count = count / size;
    int *local_array = (int*)malloc(local_count * sizeof(int));

    /* Repeat experiments */
    for (int exp = 0; exp < num_experiments; exp++) {
        local_max = -1; // Reset local max before each experiment

        /* Start timing */
        start_time = MPI_Wtime();

        /* Scatter the data to all processes */
        MPI_Scatter(array, local_count, MPI_INT, local_array, local_count, MPI_INT, 0, MPI_COMM_WORLD);

        /* Find the local maximum */
        for (int i = 0; i < local_count; i++) {
            if (local_array[i] > local_max) {
                local_max = local_array[i];
            }
        }

        /* Reduce all local maxima to global maximum */
        MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

        /* End timing */
        end_time = MPI_Wtime();

        /* Add the current experiment time to the total time */
        total_time += (end_time - start_time);
    }

    /* Calculate and print the average time */
    avg_time = total_time / num_experiments;

    /* Open file to store results */
    FILE *file = NULL;
    if (rank == 0) {
        file = fopen("/home/dt/ParProg/lab5/solve/mpi_results.csv", "a");
        if (file == NULL) {
            perror("Unable to open file");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        /* Write the results for the current experiment */
        fprintf(file, "%d\t%f\n", size, avg_time);
        fclose(file);
    }

    /* Cleanup */
    if (rank == 0) {
        free(array);
    }
    free(local_array);

    /* Finalize MPI */
    MPI_Finalize();
    return 0;
}
