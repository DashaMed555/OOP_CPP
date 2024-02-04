cmake_minimum_required(VERSION 3.26)
project(ini)

set(CMAKE_CXX_STANDARD 20)

add_executable(ini
        ini.cpp
        json.cpp
        xml.cpp
        test_ini.cpp
        test_runner.cpp)
