cmake_minimum_required(VERSION 3.16)
project(lab-09_figures)

#[[set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")]]
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(lab-09_figures
        include/Circle.hpp
        include/Figure.hpp
        include/Rectangle.hpp
        include/Scheme.hpp
        src/main.cpp
        src/Circle.cpp
        src/Figure.cpp
        src/Rectangle.cpp
        src/Scheme.cpp)
