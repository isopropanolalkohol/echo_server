cmake_minimum_required(VERSION 3.10)
project(echo_server C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES "src/*.cpp")

add_executable(echo_server ${SOURCES})


target_include_directories(echo_server PRIVATE include)
