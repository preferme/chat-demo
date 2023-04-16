
include_directories(../chat-shared/include include)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INC include/*.h include/*.hpp)

add_executable(chat-client src/main.cpp ${INC} ${SRC})

target_link_libraries(chat-client chat-shared)
