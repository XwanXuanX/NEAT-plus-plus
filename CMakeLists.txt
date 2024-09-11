cmake_minimum_required(VERSION 3.20)

# C++23 standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define a project
project(NEAT VERSION 1.0)

# Recursive call CMakeList in src dir
add_subdirectory(src)

# Include the include dir
include_directories(inc)

# Generate the Makefile for the executable
add_executable(neat ${neat_src})
