cmake_minimum_required(VERSION 3.26)
project(Server_Using_MySQL)

set(CMAKE_CXX_STANDARD 14)

include_directories("C:/Program Files/MySQL/MySQL Server 8.2/include/")
include_directories("C:/Program Files/MySQL/MySQL Server 8.2/include/mysql")
link_directories("C:/Program Files/MySQL/MySQL Server 8.2/lib")
add_executable(Server_Using_MySQL
        serverMysql.cpp
)
# Link against ws2_32.lib
target_link_libraries(Server_Using_MySQL PRIVATE ws2_32.lib -lmysql)
