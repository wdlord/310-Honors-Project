cmake_minimum_required(VERSION 3.17)
project(HonorsProject)

set(CMAKE_CXX_STANDARD 14)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/firstNames.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lastNames.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(HonorsProject main.cpp SpecialTree.h)