cmake_minimum_required(VERSION 3.29)
project(MPI_tasks)

set(CMAKE_C_STANDARD 11)
find_package(MPI REQUIRED)
add_executable(Lab1 Lab1/Lab1.c)
target_link_libraries(Lab1 PUBLIC m)
target_link_libraries(Lab1 PUBLIC MPI::MPI_CXX)
