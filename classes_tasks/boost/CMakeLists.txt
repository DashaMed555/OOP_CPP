cmake_minimum_required(VERSION 3.26)
project(boost)

set(CMAKE_CXX_STANDARD 20)

add_executable(boost main.cpp)

set(BOOST_ROOT /usr/local/Cellar/boost/1.80.0)
set(BOOST_INCLUDEDIR /usr/local/Cellar/boost/1.80.0/include)
set(BOOST_LIBRARY_DIR /usr/local/Cellar/boost/1.80.0/lib)

find_package(Boost 1.80.0 COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
