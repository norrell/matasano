cmake_minimum_required(VERSION 2.6)
project(matasano)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
set(CMAKE_CXX_STANDARD 11)
set(SOURCES main.cpp
            pkcs7.cpp
            base64.cpp
            IO.cpp
            hexbin.cpp
            vigenere.cpp
            aes128.cpp)

add_executable(matasano ${SOURCES})
