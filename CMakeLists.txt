cmake_minimum_required(VERSION 3.29)
project(MPI_tasks)

set(CMAKE_C_STANDARD 11)
find_package(MPI REQUIRED)
add_executable(Lab1 Lab1/Lab1.c)
add_executable(Lab2
        Lab2/Lab2_.c)
target_link_libraries(Lab1 PUBLIC m)
target_link_libraries(Lab1 PUBLIC MPI::MPI_CXX)
target_link_libraries(Lab2 PUBLIC m)
target_link_libraries(Lab2 PUBLIC MPI::MPI_CXX)