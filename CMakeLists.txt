cmake_minimum_required(VERSION 3.4)
project(porr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${root_SOURCE_DIR}/lib)

set(SOURCE_FILES main.cpp)

add_executable(porr ${SOURCE_FILES})