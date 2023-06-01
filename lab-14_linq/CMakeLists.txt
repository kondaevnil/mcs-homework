cmake_minimum_required(VERSION 3.16)
project(lab-14_linq)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-14_linq
        include/linq.hpp
        src/test.cpp tests/smoke_test.cpp tests/minimal_smoke_test.cpp)
