#include <stdio.h>
#include <omp.h>
#include <time.h>

void compute_with_locks(); // Объявление функции для пункта 7

int main() {
    // 1. Определение версии OpenMP и даты стандарта
    #ifdef _OPENMP
        printf("1. OpenMP Version: %d\n", _OPENMP);
        printf("   Date of standard: %d-%d-%d\n", 
               _OPENMP / 10000, 
               (_OPENMP % 10000) / 100, 
               _OPENMP % 100);
    #else
        printf("1. OpenMP is not supported on this system.\n");
    #endif

    // 2. Определение числа процессоров и максимального числа потоков
    int num_procs = omp_get_num_procs();
    int max_threads = omp_get_max_threads();
    printf("\n2. Number of available processors: %d\n", num_procs);
    printf("   Maximum number of threads: %d\n", max_threads);

    // 3. Опция dynamic и её состояние
    int dynamic_state = omp_get_dynamic();
    printf("\n3. Dynamic threads option:\n");
    printf("   Dynamic threads enabled: %s\n", dynamic_state ? "Yes" : "No");

    // 4. Разрешение таймера
    double timer_resolution = omp_get_wtick();
    printf("\n4. Timer resolution: %.12f seconds\n", timer_resolution);

    // 5. Вложенные параллельные области
    int nested_state = omp_get_nested();
    int max_active_levels = omp_get_max_active_levels();
    printf("\n5. Nested parallelism state:\n");
    printf("   Nested parallelism enabled: %s\n", nested_state ? "Yes" : "No");
    printf("   Maximum active nested levels: %d\n", max_active_levels);

    // 6. Текущие настройки распределения нагрузки
    omp_sched_t schedule_type;
    int chunk_size;
    omp_get_schedule(&schedule_type, &chunk_size);
    printf("\n6. Current schedule settings:\n");
    printf("   Schedule type: ");
    switch (schedule_type) {
        case omp_sched_static:  printf("Static\n"); break;
        case omp_sched_dynamic: printf("Dynamic\n"); break;
        case omp_sched_guided:  printf("Guided\n"); break;
        case omp_sched_auto:    printf("Auto\n"); break;
        default:                printf("Unknown\n"); break;
    }
    printf("   Chunk size: %d\n", chunk_size);

    // 7. Пример с блокировками
    printf("\n7. Example of computation using explicit locks:\n");
    compute_with_locks();

    return 0;
}

void compute_with_locks() {
    omp_lock_t lock;
    omp_init_lock(&lock);

    int shared_resource = 0;

    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();
        omp_set_lock(&lock); // Захватываем блокировку
        printf("Thread %d: Lock acquired, updating shared resource.\n", thread_id);
        shared_resource += 1; // Обновление общего ресурса
        printf("Thread %d: Updated shared resource to %d.\n", thread_id, shared_resource);
        omp_unset_lock(&lock); // Освобождаем блокировку
    }

    omp_destroy_lock(&lock);
    printf("Final value of shared resource: %d\n", shared_resource);
}
