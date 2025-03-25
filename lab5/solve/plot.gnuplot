set terminal pngcairo size 1600, 900

# Первый холст: Среднее время от числа потоков/процессов
set output 'average_time.png'
set title "Среднее время выполнения"
set xlabel "Число потоков/процессов"
set ylabel "Среднее время (с)"
set grid
plot \
    "omp_results.csv" using 1:2 with linespoints title "OpenMP", \
    "mpi_results_with_speedup.csv" using 1:2 with linespoints title "MPI"

# Второй холст: Ускорение от числа потоков/процессов
set output 'speedup.png'
set title "Ускорение"
set xlabel "Число потоков/процессов"
set ylabel "Ускорение"
set grid
plot \
    "omp_results.csv" using 1:3 with linespoints title "OpenMP", \
    "mpi_results_with_speedup.csv" using 1:3 with linespoints title "MPI"

# Третий холст: Эффективность от числа потоков/процессов
set output 'efficiency.png'
set title "Эффективность"
set xlabel "Число потоков/процессов"
set ylabel "Эффективность"
set grid
plot \
    "omp_results.csv" using 1:4 with linespoints title "OpenMP", \
    "mpi_results_with_speedup.csv" using 1:4 with linespoints title "MPI"
