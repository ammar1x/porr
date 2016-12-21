cmake_minimum_required(VERSION 3.4)
project(porr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${root_SOURCE_DIR}/lib)
include_directories(/usr/local/include)

#set(SOURCE_FILES main.cpp)

add_executable(porr ${SOURCE_FILES} main.cpp)
add_executable(test_bruteforce ${SOURCE_FILES} test_bruteforce.cpp)

target_link_libraries(test_bruteforce /usr/local/lib/libgtest.a)
