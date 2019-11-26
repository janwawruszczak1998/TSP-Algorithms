cmake_minimum_required(VERSION 3.13)
project(PEA)

set(CMAKE_CXX_STANDARD 14)

add_executable(PEA main.cpp Graph.h Graph.cpp ExactAlgorithms.h ExactAlgorithms.cpp LocalSearchAlgorithms.cpp LocalSearchAlgorithms.h)