cmake_minimum_required(VERSION 3.24)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 main.cpp Menu.cpp Menu.h UserService.cpp UserService.h HashTable.cpp HashTable.h
        RandomCredentialsGenerator.cpp
        RandomCredentialsGenerator.h
        InputHelper.cpp
        InputHelper.h)
