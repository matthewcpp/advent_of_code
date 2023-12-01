cmake_minimum_required(VERSION 3.0)

project(advent_of_code CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_CXX_STANDARD 17)

if (DEFINED BUILD_YEAR)
    if (BUILD_YEAR EQUAL "2022")
        add_subdirectory(2022)
    elseif(BUILD_YEAR EQUAL "2023")
        add_subdirectory(2023)
    endif()
else()
    add_subdirectory(2022)
    add_subdirectory(2023)
endif()