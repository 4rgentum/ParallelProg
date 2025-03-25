cmake_minimum_required(VERSION 3.10)

project(lab2 C)

# Добавление поддержки OpenMP
find_package(OpenMP REQUIRED)

# Создание исполняемого файла
add_executable(lab2 parall.c)

# Линковка с OpenMP
target_link_libraries(lab2 PRIVATE OpenMP::OpenMP_C)