cmake_minimum_required(VERSION 2.8)
project(shadowsocks_manyuser_management_daemon)

add_subdirectory(lib)

include_directories(lib/iniparser)
include_directories(lib/mysqlclient)
link_directories(lib/iniparser)
link_directories(lib/mysqlclient)
add_executable(start_servers start_servers.c)
target_link_libraries(start_servers
        iniparser
        libmysqlclient.so
        )
