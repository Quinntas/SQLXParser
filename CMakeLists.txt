cmake_minimum_required(VERSION 3.26)
project(SQLXParser C)

set(CMAKE_C_STANDARD 23)

add_executable(SQLXParser main.c
        SQLXParser.h)
