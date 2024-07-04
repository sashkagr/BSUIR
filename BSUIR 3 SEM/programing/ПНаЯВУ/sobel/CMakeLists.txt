cmake_minimum_required(VERSION 3.24)
project(sobel)

set(CMAKE_CXX_STANDARD 17)

add_executable(sobel main.cpp sobel.cpp sobel.h macros.h file_operations.cpp file_operations.h)
