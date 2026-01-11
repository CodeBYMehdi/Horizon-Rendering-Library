cmake_minimum_required(VERSION 3.31)
project(example)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(third_party)

add_executable(example main.cpp)

target_link_libraries(example PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libhrl.a
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
        opengl32
)