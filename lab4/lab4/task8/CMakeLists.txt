cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Lab4Task8 LANGUAGES C)

# Включение поддержки OpenMP
find_package(OpenMP REQUIRED)

# Добавление исполняемого файла
add_executable(lab4task8 lab4task8.c)

# Добавление флагов компиляции для OpenMP
target_link_libraries(lab4task8 PRIVATE OpenMP::OpenMP_C)
