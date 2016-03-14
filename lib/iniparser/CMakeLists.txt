cmake_minimum_required(VERSION 2.8)
project(iniparser)

set(iniparser_SRCS
        dictionary.c
        iniparser.c
        )
add_library(iniparser SHARED ${iniparser_SRCS})
