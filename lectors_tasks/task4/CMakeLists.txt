cmake_minimum_required(VERSION 3.26)
project(task4)

set(CMAKE_CXX_STANDARD 20)

add_executable(task4 main.cpp
        parser.cpp
        parser.h
        converters.cpp
        converters.h
        wav_processing.cpp
        wav_processing.h
)
