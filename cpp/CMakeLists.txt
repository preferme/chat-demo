cmake_minimum_required(VERSION 3.17)
project(cpp)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(chat-shared)
add_subdirectory(chat-client)
add_subdirectory(chat-server)
