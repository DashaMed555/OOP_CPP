cmake_minimum_required(VERSION 3.26)
project(0a_modules)

set(CMAKE_CXX_STANDARD 20)

add_executable(0a_modules main.cpp
        module1.cpp
        module2.cpp
        module3.cpp)
