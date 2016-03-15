cmake_minimum_required(VERSION 2.8)
project(shadowsocks_manyuser_management_daemon)

add_subdirectory(lib)

include_directories(
        lib/iniparser
        lib/mysqlclient
)
link_directories(lib/iniparser)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(start_servers start_servers.c)
target_link_libraries(start_servers
        iniparser
        mysqlclient
        )
