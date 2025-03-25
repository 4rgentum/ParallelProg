#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    const int random_seed = 920215; ///< RNG seed

    int *array = NULL;              ///< The array we need to find the max in
    int local_max = -1;             ///< The maximal element in the local process
    int global_max = -1;            ///< The global maximal element after reduction
    int rank, size;                ///< MPI rank and size

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

    /* Scatter the data to all processes */
    MPI_Scatter(array, local_count, MPI_INT, local_array, local_count, MPI_INT, 0, MPI_COMM_WORLD);

    /* Find the local maximum */
    for (int i = 0; i < local_count; i++) {
        if (local_array[i] > local_max) {
            local_max = local_array[i];
        }
    }

    /* Print the local maximum for each process */
    printf("Process %d: Local max = %d\n", rank, local_max);

    /* Reduce all local maxima to global maximum */
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    /* Only the root process prints the global maximum */
    if (rank == 0) {
        printf("Max is: %d;\n", global_max);
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
