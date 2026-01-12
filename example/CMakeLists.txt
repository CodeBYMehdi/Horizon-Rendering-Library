cmake_minimum_required(VERSION 3.31)
project(example)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)  #nécessaire pour hrl (C++)

include_directories(src)
include_directories(third_party)

#Crée l'exécutable
add_executable(example main.c)

#Force l'utilisation du linker C++ pour lier l'exécutable C avec la lib C++
set_target_properties(example PROPERTIES LINKER_LANGUAGE CXX)

#Lie les libs
target_link_libraries(example PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libhrl.a
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
        opengl32
)