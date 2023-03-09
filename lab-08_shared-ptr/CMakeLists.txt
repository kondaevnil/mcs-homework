cmake_minimum_required(VERSION 3.16)
project(lab-08_shared-ptr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-08_shared-ptr
        include/matrix.hpp
        include/shared_ptr.hpp
        src/main.cpp
        src/matrix.cpp
        src/shared_ptr.cpp)
