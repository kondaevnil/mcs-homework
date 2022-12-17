cmake_minimum_required(VERSION 3.16)
project(hw_01-bmp C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

add_executable(hw_01-bmp
        include/bmp.h
        src/bmp.c
        src/main.c)
