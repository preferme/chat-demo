
include_directories(include)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INC include/*.h)

#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(chat-shared SHARED ${INC} ${SRC})
