cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(QuadUI)

set(QuadUI_VERSION_MAJOR 0)
set(QuadUI_VERSION_MINOR 0)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add subdirectories in library dependency order
add_subdirectory(src)
