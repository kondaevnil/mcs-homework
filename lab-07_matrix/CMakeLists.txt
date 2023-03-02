cmake_minimum_required(VERSION 3.16)
project(lab-07_matrix)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-07_matrix
        include/matrix.h
        src/main.cpp
        src/matrix.cpp)
