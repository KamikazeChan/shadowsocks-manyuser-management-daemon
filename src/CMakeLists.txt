cmake_minimum_required(VERSION 2.8)

project(start_servers)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(start_server
        start_servers.c)
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(start_server
        iniparser
        mysqlclient
        )
