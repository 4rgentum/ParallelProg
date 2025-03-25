cmake_minimum_required(VERSION 3.28)
project(Lab5 C)

# Укажите стандарт C
set(CMAKE_C_STANDARD 99)

# Найти библиотеку OpenMPI
find_package(MPI REQUIRED)

# Определить компилятор OpenMP
find_package(OpenMP REQUIRED)

# Добавить цели для двух программ
add_executable(lab5omp lab5OMP.c)
target_link_libraries(lab5omp OpenMP::OpenMP_C)

add_executable(lab5mpi lab5MPI.c)
target_link_libraries(lab5mpi MPI::MPI_C)
