cmake_minimum_required(VERSION 3.12)
project(practice_5_threads C)

set(CMAKE_C_STANDARD 11)

add_executable(practice_5_threads main.c threaded_hello.c join.c)