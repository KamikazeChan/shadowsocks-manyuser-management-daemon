cmake_minimum_required(VERSION 2.8)
project(iniparser)

set(iniparser_SRCS
        dictionary.c
        iniparser.c
        )
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin/lib/iniparser)
add_library(iniparser SHARED ${iniparser_SRCS})
