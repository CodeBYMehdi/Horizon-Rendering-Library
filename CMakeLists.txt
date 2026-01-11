cmake_minimum_required(VERSION 3.31)
project(hrl)

#C++ 20 Minimum
set(CMAKE_CXX_STANDARD_REQUIRED 20)

#creer le fichier generated dans le fichier cmake
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated)

#function to generate a .cpp file with binary definition of a file
function(embed_binary INPUT_FILE OUTPUT_NAME)
    set(OUTPUT_CPP ${CMAKE_BINARY_DIR}/generated/${OUTPUT_NAME}.cpp)

    add_custom_command(
        OUTPUT ${OUTPUT_CPP}
        COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/embed.py
                ${INPUT_FILE} ${OUTPUT_NAME} > ${OUTPUT_CPP}
        DEPENDS ${INPUT_FILE}
        COMMENT "Embeding ${INPUT_FILE}"
    )

    set(EMBEDDED_SOURCES ${EMBEDDED_SOURCES} ${OUTPUT_CPP} PARENT_SCOPE)
endfunction()

#opengl ressources
embed_binary(${CMAKE_SOURCE_DIR}/src/shaders/opengl/sprite.vert.glsl gl_sprite_vert_shader)
embed_binary(${CMAKE_SOURCE_DIR}/src/shaders/opengl/sprite.frag.glsl gl_sprite_frag_shader)
embed_binary(${CMAKE_SOURCE_DIR}/src/shaders/opengl/ui.vert.glsl gl_ui_vert_shader)
embed_binary(${CMAKE_SOURCE_DIR}/src/shaders/opengl/ui.frag.glsl gl_ui_frag_shader)


include_directories(src)
include_directories(third_party)

add_library(hrl STATIC
        #third-party
        third_party/glad/glad.c

        src/hrl.cpp
        ${EMBEDDED_SOURCES}

        #opengl
        src/backend/opengl33/gl33_backend.cpp
        src/backend/opengl33/gl33_renderer.cpp
        src/backend/opengl33/gl33_shader.cpp
)


#link OpenGL
if(WIN32)
    target_link_libraries(hrl PRIVATE opengl32)
elseif(UNIX)
    target_link_libraries(hrl PRIVATE libGL.so)
endif()