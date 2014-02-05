# generated based on http://cmake.org/cmake/help/cmake_tutorial.html
cmake_minimum_required (VERSION 2.8.12.1)
project (prebowt)

# change to Clang
set (CMAKE_CXX_COMPILER "/usr/bin/clang++")

# The version number.
set (prebowt_VERSION_MAJOR 1)
set (prebowt_VERSION_MINOR 0)

#Sends the -std=c++11 flag to the gcc compiler
add_definitions(-std=c++11)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/prebowtconfig.hpp.in"
  "${PROJECT_BINARY_DIR}/include/prebowtconfig.hpp"
  )

# Have CMake tell us what it's doing
set (CMAKE_VERBOSE_MAKEFILE true)

# add the binary tree to the search path for include files
# so that we will find prebowtconfig.hpp
include_directories("${PROJECT_BINARY_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include")

# add the project executable
add_executable(prebowt src/dtree.cpp)
