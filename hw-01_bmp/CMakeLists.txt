cmake_minimum_required(VERSION 3.16)
project(hw-01_bmp C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(include)

add_executable(hw-01_bmp
        include/bmp.h
        src/bmp.c
        src/main.c)
