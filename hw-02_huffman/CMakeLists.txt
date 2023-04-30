cmake_minimum_required(VERSION 3.16)
project(hw-02_huffman)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(hw-02_huffman
        include/huffman.h
        src/huffman.cpp
        src/main.cpp
        src/bit_manip.cpp
        include/bit_manip.h)
