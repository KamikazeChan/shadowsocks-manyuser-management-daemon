cmake_minimum_required(VERSION 3.3)

project(iniparser)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(iniparser SHARED
        dictionary.c
        iniparser.c
        )

