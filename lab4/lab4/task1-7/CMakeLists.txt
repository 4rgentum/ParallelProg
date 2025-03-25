cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Lab4_OpenMP_Tasks VERSION 1.0 LANGUAGES C)

# Включение поддержки OpenMP
find_package(OpenMP REQUIRED)

# Добавление исполняемого файла
add_executable(lab4task1-7 lab4task1-7.c)

# Линковка с OpenMP
if(OpenMP_C_FOUND)
    target_link_libraries(lab4task1-7 PRIVATE OpenMP::OpenMP_C)
endif()

# Параметры компиляции для OpenMP
target_compile_options(lab4task1-7 PRIVATE ${OpenMP_C_FLAGS})
