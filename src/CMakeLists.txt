cmake_minimum_required(VERSION 2.8)
project(start_servers)

include_directories(
        ${PROJECT_SOURCE_DIR}/../lib/iniparser
        ${PROJECT_SOURCE_DIR}/../lib/mysqlclient
)
link_directories(${PROJECT_SOURCE_DIR}/../lib/iniparser)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
add_executable(start_servers start_servers.c)
target_link_libraries(start_servers
        iniparser
        mysqlclient
        )
