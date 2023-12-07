cmake_minimum_required(VERSION 3.27)

project(AOC VERSION 2023 LANGUAGES CXX)

add_library(aoc)
target_sources(aoc PUBLIC include/aoc/support.hpp PRIVATE include/aoc/support.cpp)
target_compile_features(aoc PUBLIC cxx_std_23)
target_include_directories(aoc PUBLIC include)

add_subdirectory(external/ctre)

add_subdirectory(day1)
add_subdirectory(day2)
