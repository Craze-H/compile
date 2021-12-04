cmake_minimum_required(VERSION 3.20)
project(compile)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
add_definitions(-DLOCAL)
add_executable(compile
        getword.h
        test.cpp)
