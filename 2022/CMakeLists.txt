cmake_minimum_required(VERSION 3.0)

project(advent_of_code_2022 CXX)

#define input directory for all targets
add_compile_definitions(INPUT_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/input/")

add_executable(1-1 1-1.cpp)
add_executable(1-2 1-2.cpp)
add_executable(2-1 2-1.cpp)
add_executable(2-2 2-2.cpp)
add_executable(3-1 3-1.cpp)
add_executable(3-2 3-2.cpp)
add_executable(4-1 4-1.cpp)
add_executable(4-2 4-2.cpp)
add_executable(5-1 5-1.cpp)
add_executable(5-2 5-2.cpp)