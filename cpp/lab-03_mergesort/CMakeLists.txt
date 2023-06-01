cmake_minimum_required(VERSION 3.16)
project(lab-03_mergesort C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(lab-03_mergesort
        include/mergesort.h
        src/main.c
        src/mergesort.c)
