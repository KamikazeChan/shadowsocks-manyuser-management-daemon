cmake_minimum_required(VERSION 3.3)
project(shadowsocks_manyuser_management_daemon)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/server.c src/server.c)
add_executable(shadowsocks_manyuser_management_daemon ${SOURCE_FILES})
