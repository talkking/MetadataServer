cmake_minimum_required(VERSION 3.0)

project(metadata_management)

set(CMAKE_CXX_FLAGS "-std=c++11")

INCLUDE_DIRECTORIES(
include
)

set(src src)

add_subdirectory(${src}/lib)

add_executable(client ${src}/client.cpp)

add_executable(server ${src}/server.cpp)

target_link_libraries(client communication)

target_link_libraries(server filesystem communication)