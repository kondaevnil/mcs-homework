cmake_minimum_required(VERSION 3.16)
project(lab-11_vector)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-11_vector
        include/my_vector.h
        include/my_vector.hpp
        include/my_vector_impl.hpp
        src/main.cpp)
