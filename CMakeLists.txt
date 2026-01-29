cmake_minimum_required(VERSION 3.20)
project(SPOTIFY_AUTO_SHUFFLER VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(src/include)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(main ${SOURCES})
