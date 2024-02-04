cmake_minimum_required(VERSION 3.26)
project(string_groups)

set(CMAKE_CXX_STANDARD 20)

add_executable(string_groups main.cpp
        test_runner.cpp
        test_runner.h
        tests.cpp
        tests.h
        profile.h
)
