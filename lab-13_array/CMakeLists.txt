cmake_minimum_required(VERSION 3.16)
project(lab_13_array)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab_13_array
        include/my_array.h
        src/main.cpp)
