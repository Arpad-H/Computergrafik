cmake_minimum_required(VERSION 3.26)
project(CGG)

set(CMAKE_CXX_STANDARD 20)

add_executable(CGG main.cpp
        vector.h
        vector.cpp
        test1.cpp
        color.cpp
        test2.cpp
        rgbimage.cpp
        test3.cpp)
