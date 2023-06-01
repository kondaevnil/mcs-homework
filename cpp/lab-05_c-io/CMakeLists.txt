cmake_minimum_required(VERSION 3.16)
project(lab-05_c-io C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(lab-05_c-io
        include/clist.h
        include/point_list.h
        src/clist.c
        src/main.c
        src/point_list.c)
