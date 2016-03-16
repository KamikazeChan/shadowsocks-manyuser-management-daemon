cmake_minimum_required(VERSION 2.8)
project(start_servers)

include_directories(
        ../lib/iniparser/iniparser-4.0/src
        ../lib/mysql/include
)
link_directories(
        lib/iniparser
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
add_executable(start_servers start_servers.c)
target_link_libraries(start_servers
        iniparser
        mysqlclient
        )
