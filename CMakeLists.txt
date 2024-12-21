cmake_minimum_required(VERSION 3.16)

project(rcon LANGUAGES C CXX VERSION 2.0.0)

option(BUILD_CLI "Build the cli tool" ON)
option(BUILD_QT "Build the Qt interface" ON)

add_subdirectory(lib/)

if(BUILD_CLI)
    add_subdirectory(cli/)
endif()

if(BUILD_QT)
    add_subdirectory(qt/)
endif()
