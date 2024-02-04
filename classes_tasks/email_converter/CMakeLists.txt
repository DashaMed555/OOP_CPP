cmake_minimum_required(VERSION 3.26)
project(email_converter)

set(CMAKE_CXX_STANDARD 20)

add_executable(email_converter main.cpp
        test_runner.cpp)
