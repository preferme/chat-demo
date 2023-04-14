cmake_minimum_required(VERSION 3.16)
project(chat-test)

include_directories(../chat-shared/include include)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INC include/*.h include/*.hpp)

add_executable(chat-test src/main.cpp ${INC} ${SRC})

target_link_libraries(chat-test chat-shared pthread)
