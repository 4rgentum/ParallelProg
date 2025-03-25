cmake_minimum_required(VERSION 3.28)

# Project name
project(lab6 C)

# Set the C standard
set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

# Add the source file
add_executable(lab6omp lab6omp.c)
target_link_libraries(lab6omp OpenMP::OpenMP_C) 

add_executable(lab6mpi lab6mpi.c)
target_link_libraries(lab6mpi MPI::MPI_C)