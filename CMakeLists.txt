cmake_minimum_required(VERSION 3.4)
project(porr)


set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCL REQUIRED)
find_package(MPI)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3" )



include_directories(lib)
include_directories(${MPI_CXX_INCLUDE_PATH})



file(COPY conf DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY data DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY scripts DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(bruteForce ${SOURCE_FILES} BruteForceMain.cpp)
target_link_libraries(bruteForce ${OpenCL_LIBRARIES} ${MPI_CXX_LIBRARIES})





