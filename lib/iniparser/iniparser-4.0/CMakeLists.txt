cmake_minimum_required(VERSION 2.8)
project(iniparser)

include_directories(src)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../../bin/lib/iniparser)
add_library(iniparser SHARED
        src/iniparser.c
        src/dictionary.c
        )
