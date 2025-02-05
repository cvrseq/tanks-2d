
cmake_minimum_required(VERSION 3.10)

project(game_example)

set(CMAKE_CXX_STANDARD 11)

find_package(raylib REQUIRED)

add_executable(my_program main.c)

target_link_libraries(my_program raylib)