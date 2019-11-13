cmake_minimum_required(VERSION 3.15)
project(sparse_matrix_mateonoel2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(mocker)
include_directories(tester)

add_executable(sparse_matrix_mateonoel2
        mocker/mocker.cpp
        mocker/mocker.h
        tester/tester.cpp
        tester/tester.h
        main.cpp
        matrix.h
        node.h)
