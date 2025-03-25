cmake_minimum_required(VERSION 3.10)

# Project name
project(lab3 C)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the source file
add_executable(lab3 lab3.c)

# Enable OpenMP if available
find_package(OpenMP)
if(OpenMP_C_FOUND)
    target_link_libraries(lab3 PUBLIC OpenMP::OpenMP_C)
endif()