cmake_minimum_required(VERSION 3.30)
project(shimmer)

set(CMAKE_CXX_STANDARD 20)

add_executable(shimmer src/main.cpp
        src/stb_image.h
        src/stb_image.cpp)
