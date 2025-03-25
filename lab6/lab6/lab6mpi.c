#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void shell_sort(int *arr, int size) {
    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void merge(int *array, int start, int mid, int end) {
    int left_size = mid - start;
    int right_size = end - mid;

    int *left = (int *)malloc(left_size * sizeof(int));
    int *right = (int *)malloc(right_size * sizeof(int));

    for (int i = 0; i < left_size; i++) left[i] = array[start + i];
    for (int i = 0; i < right_size; i++) right[i] = array[mid + i];

    int i = 0, j = 0, k = start;
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            array[k++] = left[i++];
        } else {
            array[k++] = right[j++];
        }
    }

    while (i < left_size) array[k++] = left[i++];
    while (j < right_size) array[k++] = right[j++];

    free(left);
    free(right);
}

int main(int argc, char **argv) {
    const int count = 100000000; // Number of elements in the array
    const int seed = 920214;

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *array = NULL;
    if (rank == 0) {
        array = (int *)malloc(count * sizeof(int));
        srand(seed);
        for (int i = 0; i < count; i++) {
            array[i] = rand();
        }
    }

    int local_count = count / size;
    int *local_array = (int *)malloc(local_count * sizeof(int));

    double start_time = MPI_Wtime();

    MPI_Scatter(array, local_count, MPI_INT, local_array, local_count, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process sorts its portion of the array
    shell_sort(local_array, local_count);

    // Gather sorted subarrays at rank 0
    MPI_Gather(local_array, local_count, MPI_INT, array, local_count, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Merge sorted subarrays
        for (int step = 1; step < size; step *= 2) {
            for (int i = 0; i < size; i += 2 * step) {
                int start = i * local_count;
                int mid = start + step * local_count;
                int end = mid + step * local_count;

                if (mid > count) mid = count;
                if (end > count) end = count;

                merge(array, start, mid, end);
            }
        }

        // Write results to a file
        FILE *file = fopen("/home/dt/ParProg/lab6/mpi_results.csv", "a");
        if (!file) {
            perror("Unable to open file");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        double end_time = MPI_Wtime();
        fprintf(file, "%d\t%f\n", size, end_time - start_time);
        fclose(file);
        free(array);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
