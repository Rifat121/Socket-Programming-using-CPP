cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Server
        server.cpp
        sqlite3.c)
