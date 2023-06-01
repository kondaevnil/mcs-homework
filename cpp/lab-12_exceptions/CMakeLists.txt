cmake_minimum_required(VERSION 3.16)
project(lab-12_exceptions)

set(CMAKE_CXX_STANDARD 17)

include_directories("include")

add_executable(lab-12_exceptions
        "include/matrix.h"
        src/main.cpp
        src/matrix.cpp)
