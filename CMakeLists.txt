cmake_minimum_required(VERSION 2.8) 
project(blit-test)

# Set some CMake properties:
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

# Build the main directory, always
add_subdirectory(src)
