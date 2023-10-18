cmake_minimum_required(VERSION 3.26)
project(CGG)

set(CMAKE_CXX_STANDARD 20)

add_executable(CGG
        CG_Helpers/vector.h
        CG_Helpers/vector.cpp
        CG_Helpers/test1.cpp
        CG_Helpers/rgbimage.cpp
        CG_Helpers/color.cpp
        CG_Helpers/test2.cpp
        CG_Helpers/test3.cpp
        CGUtilities.cpp
        main.cpp
        SimpleRayTracer.cpp)
