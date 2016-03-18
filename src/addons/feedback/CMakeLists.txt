cmake_minimum_required(VERSION 2.8)
project(feedback)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../../bin/addons/feedback)
add_executable(feedback feedback.c)
