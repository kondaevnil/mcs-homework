cmake_minimum_required(VERSION 3.16)
project(lab-02_cstrings C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

add_executable(lab-02_cstrings
        include/str.h
        include/test_str.h
        src/str.c
        src/test.c
        src/test_str.c)
