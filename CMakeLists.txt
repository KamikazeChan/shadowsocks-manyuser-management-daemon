cmake_minimum_required(VERSION 3.3)
project(shadowsocks_manyuser_management_daemon)

set(start_servers_source_files
        src/start_servers.c
        src/iniparser.c
        src/dictionary.c
        )
add_executable(start_servers ${start_servers_source_files})
set(start_servers_link_libraries
        /usr/lib/x86_64-linux-gnu/libmysqlclient.so     #apt install libmysqlclient-dev
)
target_link_libraries(start_servers ${start_servers_link_libraries})
