set terminal pngcairo enhanced font 'Arial,12' size 1280,960

### Настройки общего вида
set grid
set key outside
set style data linespoints

### 1. Среднее время от числа потоков
set title "Average Time vs Threads"
set xlabel "Threads"
set ylabel "Average Time (seconds)"
set output 'avg_time_vs_threads.png'
plot 'random_results.csv' using 1:2 title "AvgTime" with linespoints lw 2 lc rgb "#1f77b4"

### 2. Ускорение от числа потоков
set title "Speedup vs Threads"
set xlabel "Threads"
set ylabel "Speedup"
set output 'speedup_vs_threads.png'
plot 'random_results.csv' using 1:3 title "Speedup" with linespoints lw 2 lc rgb "#ff7f0e"

### 3. Эффективность от числа потоков
set title "Efficiency vs Threads"
set xlabel "Threads"
set ylabel "Efficiency"
set output 'efficiency_vs_threads.png'
plot 'random_results.csv' using 1:4 title "Efficiency" with linespoints lw 2 lc rgb "#2ca02c"

### 4. Время выполнения от процента отсортированности для каждого потока
set title "Execution Time vs Sortedness Percentage (for each thread count)"
set xlabel "Percentage of Sortedness (-100% to 100%)"
set ylabel "Execution Time (seconds)"
set output 'time_vs_sortedness.png'

# Настройка стиля линий и легенды
set key outside right top
set grid
set style data linespoints

# Массив степеней отсортированности
set xtics ("-100" 1, "-80" 2, "-60" 3, "-40" 4, "-20" 5, "0" 6, "20" 7, "40" 8, "60" 9, "80" 10, "100" 11)

# Построение графиков для каждого потока
plot for [col=1:16] 'special_results.csv' using 0:col with linespoints lw 2 pt col title sprintf("Потоков: %d", col)

unset output
reset
