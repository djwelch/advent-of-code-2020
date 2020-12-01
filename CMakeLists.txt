cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(advent-of-code)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -Weverything -Wno-vla")
add_executable(day-01 src/day-01.c)
