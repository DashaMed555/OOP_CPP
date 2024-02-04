cmake_minimum_required(VERSION 3.26)
project(0a)

set(CMAKE_CXX_STANDARD 17)

add_executable(0a main.cpp
        module1.cpp
        module1.h
        module2.cpp
        module2.h
        module3.cpp
        module3.h)
