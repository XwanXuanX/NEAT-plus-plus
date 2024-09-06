cmake_minimum_required(VERSION 3.20)

# C++23 standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define a project
project(NEAT VERSION 1.0)
set(NEAT_VERSION 1.0)


# Function to prepend the subdirectory to source files in subdirectories
FUNCTION(PREPEND var )
   SET(listVar "")
   FOREACH(f ${${var}})
       LIST(APPEND listVar "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)


# Include source code and headers. This calls the CMakeLists.txt in each
# subdirectory.
add_subdirectory(src)
add_subdirectory(inc)

# Repo structure
include_directories(inc)
add_executable(neat 
        ${main_src}
        ${genotype_src}
        ${utility_src}
        ${prob_src}
        ${xor_game_src}
)
