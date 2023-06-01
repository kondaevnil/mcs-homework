cmake_minimum_required(VERSION 3.16)
project(lab-10_cppio)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-10_cppio
        include/bin_manip.h
        include/employees.h
        src/bin_manip.cpp
        src/employees.cpp
        src/main.cpp)
