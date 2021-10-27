cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(Caissa)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(Caissa ${SRC_FILES})
target_include_directories(Caissa PUBLIC ./include/)