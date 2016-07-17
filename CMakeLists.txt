cmake_minimum_required(VERSION 2.6)
project(matasano)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
set(CMAKE_CXX_STANDARD 11)
set(SOURCES main.cpp
            set1/singlebyte_xor.cpp
            set1/utils.cpp
            set1/detect_singlebyte_xor.cpp)

add_executable(matasano ${SOURCES})
