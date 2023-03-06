
include_directories(include src)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INC include/*.h)

add_executable(client src/main.cpp ${INC} ${SRC})
