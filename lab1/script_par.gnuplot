# Настройки графика
set terminal pngcairo size 1280, 800
set output 'parallel_performance.png'

set title "Зависимость времени выполнения от количества потоков"
set xlabel "Количество потоков"
set ylabel "Время выполнения (с)"
set grid
set datafile separator whitespace
set key left top

# Настройка стилей точек и линий
set style data linespoints
set pointsize 1.5

# Загрузка данных из файла и построение графика
plot 'experiment_results.csv' using 1:2 with linespoints title "Среднее время выполнения" lw 2 lc rgb "blue"

# График: Ускорение от числа потоков
set output 'speedup_vs_threads.png'  # Имя выходного файла
set title "Ускорение от числа потоков"
set xlabel "Количество потоков"
set ylabel "Ускорение"
plot 'experiment_results.csv' using 1:3 with linespoints title "Ускорение" lw 2 lc rgb "blue"

# График: Эффективность от числа потоков
set output 'efficiency_vs_threads.png'  # Имя выходного файла
set title "Эффективность от числа потоков"
set xlabel "Количество потоков"
set ylabel "Эффективность"
plot 'experiment_results.csv' using 1:4 with linespoints title "Эффективность" lw 2 lc rgb "red"