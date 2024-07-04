cmake_minimum_required(VERSION 3.24)
project(lab7)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab7 main.cpp Student.cpp Student.h BinFile.cpp BinFile.h TxtFile.cpp TxtFile.h OutFile.cpp OutFile.h)
