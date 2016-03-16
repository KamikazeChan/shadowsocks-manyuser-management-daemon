cmake_minimum_required(VERSION 2.8)
project(iniparser)

include_directories(src)
add_library(iniparser STATIC
        src/iniparser.c
        src/dictionary.c
        )
