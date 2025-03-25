# Первый график - количество сравнений от количества элементов
set terminal pngcairo size 1280, 800
set output 'comparisons_of_size.png'

set title "Количество сравнений от количества элементов"
set xlabel "Количество элементов"
set ylabel "Среднее количество сравнений"
set grid
set logscale x
set key left top
plot 'experiment_results.csv' using 1:2 with linespoints title "Среднее количество сравнений" lw 2 lc rgb "blue"

# Второй график - среднее время выполнения от количества элементов
set terminal pngcairo size 1280, 800
set output 'time_of_size.png'

set title "Среднее время выполнения от количества элементов"
set xlabel "Количество элементов"
set ylabel "Среднее время выполнения (с)"
set grid
set logscale x
set key left top
plot 'experiment_results.csv' using 1:3 with linespoints title "Среднее время выполнения" lw 2 lc rgb "red"
