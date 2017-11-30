cmake_minimum_required(VERSION 3.8)
project(structures)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES main.c)
add_executable(structures ${SOURCE_FILES})