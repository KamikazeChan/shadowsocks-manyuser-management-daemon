cmake_minimum_required(VERSION 2.8)
project(start_servers)

include_directories(
        ../lib/iniparser/iniparser-4.0/src
        ../lib/mysql
)
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386")
    add_library(mysqlclient SHARED IMPORTED)
    set_property(TARGET mysqlclient PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../lib/mysql/X86/libmysqlclient.so)
    link_directories(../lib/mysql/X86)
elseif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
    add_library(mysqlclient SHARED IMPORTED)
    set_property(TARGET mysqlclient PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../lib/mysql/X86/libmysqlclient.so)
    link_directories(../lib/mysql/X86)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    add_library(mysqlclient SHARED IMPORTED)
    set_property(TARGET mysqlclient PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../lib/mysql/X86_64/libmysqlclient.so)
    link_directories(../lib/mysql/X86_64)
else()
    #其他架构的cpu请将需要的动态链接库文件放入系统lib文件夹并确定能被链接器搜索到
    message(STATUS "不受支持的cpu")
endif ()
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
add_executable(start_servers start_servers.c)
target_link_libraries(start_servers
        iniparser
        mysqlclient
        )
