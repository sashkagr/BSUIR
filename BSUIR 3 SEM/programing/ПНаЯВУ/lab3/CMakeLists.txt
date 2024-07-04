cmake_minimum_required(VERSION 3.24)
project(lab3)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3 main.cpp Item.cpp Item.h Weapon.cpp Weapon.h Resource.cpp Resource.h Potion.cpp Potion.h Menu.cpp Menu.h InputHelper.cpp InputHelper.h
        GameService.cpp
        GameService.h
        OutputHelper.cpp
        OutputHelper.h
        Cryption.h
        Encryption.cpp
        Encryption.h
        Decryption.cpp
        Decryption.h
        Elections.cpp
        Elections.h
        Elections.h
)
