# Установка выходного формата
set terminal pngcairo size 800,600 enhanced font "Arial,12"
set output 'performance_plots.png'

# Настройки стилей
set style data linespoints
set grid

# Установка общих меток
set xlabel "Number of Threads"
set xtics 1
set key outside

# Первый график: Среднее время от числа потоков
set title "Average Time vs Number of Threads"
set ylabel "Average Time (s)"
plot "performance_data.txt" using 1:2 with linespoints title "Average Time"

# Сохранение графика
set output 'average_time.png'
replot

# Второй график: Ускорение от числа потоков
set title "Speedup vs Number of Threads"
set ylabel "Speedup"
plot "performance_data.txt" using 1:3 with linespoints title "Speedup"

# Сохранение графика
set output 'speedup.png'
replot

# Третий график: Эффективность от числа потоков
set title "Efficiency vs Number of Threads"
set ylabel "Efficiency"
plot "performance_data.txt" using 1:4 with linespoints title "Efficiency"

# Сохранение графика
set output 'efficiency.png'
replot