cmake_minimum_required(VERSION 3.12)
project(hufmann-algorithm C)

set(CMAKE_C_STANDARD 99)

add_executable(hufmann-algorithm main.c map.c map.h tree.c tree.h)