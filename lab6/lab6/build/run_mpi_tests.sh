#!/bin/bash

# Убедитесь, что файл mpi_results.csv существует, если нет - создадим
RESULTS_FILE="/home/dt/ParProg/lab6/mpi_results.csv"
if [ ! -f "$RESULTS_FILE" ]; then
    echo -e "# NumProcesses\tAvgTime" > "$RESULTS_FILE"
fi

# Запуск mpirun для разных значений np
for np in {1..8}  # для np от 1 до 8 без oversubscribe
do
    echo "Running with $np processes..."
    mpirun -np $np ./lab6mpi
done

for np in {9..16}  # для np от 9 до 16 с oversubscribe
do
    echo "Running with $np processes (oversubscribe)..."
    mpirun --oversubscribe -np $np ./lab6mpi
done

# Чтение и обработка данных
LINEAR_TIME=0
OUTPUT_FILE="/home/dt/ParProg/lab6/mpi_results_with_speedup.csv"

# Записываем заголовок в новый файл
echo -e "# NumProcesses\tAvgTime\tSpeedup\tEfficiency" > "$OUTPUT_FILE"

# Читаем файл и вычисляем ускорение и эффективность
while IFS=$'\t' read -r np avg_time
do
    if [ "$np" == "# NumProcesses" ]; then
        continue  # Пропустить заголовок
    fi

    # Убираем лишние пробелы и проверяем, что avg_time — это число
    avg_time=$(echo $avg_time | tr -d '[:space:]')

    # Если avg_time — это число, продолжаем
    if [[ "$avg_time" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        # Для np=1, сохраняем время как линейное время
        if [ "$np" -eq 1 ]; then
            LINEAR_TIME=$avg_time
        fi

        # Если линейное время уже сохранено, рассчитываем ускорение и эффективность
        if [ "$LINEAR_TIME" != "0" ]; then
            # Форматируем скорость и эффективность с 6 знаками после запятой
            SPEEDUP=$(echo "$LINEAR_TIME / $avg_time" | bc -l)
            EFFICIENCY=$(echo "$SPEEDUP / $np" | bc -l)

            # Используем printf для форматирования чисел с 6 знаками после запятой
            FORMATTED_SPEEDUP=$(printf "%.6f" $SPEEDUP)
            FORMATTED_EFFICIENCY=$(printf "%.6f" $EFFICIENCY)

            # Записываем данные в новый файл
            echo -e "$np\t$avg_time\t$FORMATTED_SPEEDUP\t$FORMATTED_EFFICIENCY" >> "$OUTPUT_FILE"
        fi
    else
        echo "Skipping invalid data: $np\t$avg_time"
    fi
done < "$RESULTS_FILE"

echo "Ускорение и эффективность добавлены в файл: $OUTPUT_FILE"