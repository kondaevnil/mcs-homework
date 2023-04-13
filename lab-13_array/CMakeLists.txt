cmake_minimum_required(VERSION 3.16)
project(lab-13_array)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-13_array
        include/my_array.h
        src/main.cpp)
