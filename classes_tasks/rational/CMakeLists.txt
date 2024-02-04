cmake_minimum_required(VERSION 3.26)
project(rational)

set(CMAKE_CXX_STANDARD 17)

add_executable(rational main.cpp
        rational.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

target_link_libraries(rational GTest::gtest_main)
