cmake_minimum_required(VERSION 3.4)
project(porr)


set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(/usr/local/include)

find_package(OpenCL REQUIRED)
find_package(MPI)
find_package(GTest)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

# debug configuration
#MESSAGE(WARNING CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
#MESSAGE(WARNING "IS MPI FOUND? " ${MPI_CXX_FOUND})
#MESSAGE(WARNING "GTestFound? " ${GTEST_LIBRARIES} )

#set(SOURCE_FILES main.cpp)
include_directories(lib)
include_directories(${MPI_CXX_INCLUDE_PATH})

link_libraries(${OpenCL_LIBRARIES})
# google test
link_libraries(gtest)
# mpi
link_libraries(${MPI_CXX_LIBRARIES})
# google logging

link_libraries(glog)

# google benchmark
link_libraries(benchmark)


file(COPY conf DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY data DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY scripts DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
MESSAGE(WARNING "YOU " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(bruteForce ${SOURCE_FILES} BruteForceMain.cpp)
add_executable(test_bruteforce ${SOURCE_FILES} test_bruteforce.cpp)
add_executable(test_opencl ${SOURCE_FILES} test_opencl.cpp)
add_executable(test_mpi ${SOURCE_FILES} test_mpi.cpp)
add_executable(test_cpp ${SOURCE_FILES} test_cpp.cpp)
add_executable(benchmark_brute ${SOURCE_FILES} benchmark_brute.cpp)



