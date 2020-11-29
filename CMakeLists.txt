cmake_minimum_required(VERSION 3.0)
project(json_test C)

add_library(json json.c)
add_executable(json_test test.c)
target_link_libraries(json_test json)