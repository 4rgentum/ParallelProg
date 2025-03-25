cmake_minimum_required(VERSION 3.10)

# Название проекта
project(lab2 C)

# Установка стандартов языка C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Добавление исполняемого файла
add_executable(lab2 lab2.c)