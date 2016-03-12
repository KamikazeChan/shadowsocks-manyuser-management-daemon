cmake_minimum_required(VERSION 3.3)


project(iniparser)
add_library(iniparser SHARED
        dictionary.c
        iniparser.c
        )
