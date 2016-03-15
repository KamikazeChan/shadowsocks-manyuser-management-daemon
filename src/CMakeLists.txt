cmake_minimum_required(VERSION 2.8)
project(start_servers)

include_directories(
        ../lib/iniparser
        ../lib/mysqlclient
)
link_directories(
        lib/iniparser
        /usr/lib64
        /usr/lib
        /usr/lib64/mysql
        /usr/lib/mysql
        /usr/lib/x86_64-linux-gnu/
        /usr/lib/x86-linux-gnu/
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
add_executable(start_servers start_servers.c)
target_link_libraries(start_servers
        iniparser
        mysqlclient
        )
