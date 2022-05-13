cmake_minimum_required (VERSION 3.2)

project (perf-stat)

set(CMAKE_CXX_STANDARD 11)

add_executable(perf-stat perf-stat.cpp)
