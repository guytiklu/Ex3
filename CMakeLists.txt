cmake_minimum_required(VERSION 3.15)
project(Ex3)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(Ex3 main.cpp ex1.cpp ex1.h cmake-build-debug/Expression.cpp cmake-build-debug/Expression.h Expression.cpp Expression.h)