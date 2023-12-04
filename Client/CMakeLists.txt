cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 17)

add_executable(Client
        client.cpp)
# Link against ws2_32.lib
target_link_libraries(Client PRIVATE ws2_32.lib)
