cmake_minimum_required(VERSION 3.16)
project(lab-06_int-vector)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-06_int-vector
        include/my_vector.hpp
        src/main.cpp
        src/my_vector.cpp)
