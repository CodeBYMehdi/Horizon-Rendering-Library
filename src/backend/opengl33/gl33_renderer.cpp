#include "gl33_renderer.h"
#include "gl33_ressources.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../../hrl.h"

//debug printf
#include <cstdio>

#include <unordered_map>
#include <string>

#include "gl33_shader.h"


#define Buffer_Num      3

#define Sprite_Buffer   0
#define Mesh2D_Buffer   1
#define Mesh3D_Buffer   2


static GLuint vao[Buffer_Num];
static GLuint vbo[Buffer_Num];
static GLuint ebo[Buffer_Num];


//indices pour deux triangles
unsigned int sprite_indices[] = {
    0, 1, 2,
    2, 3, 0
};


std::unordered_map<HRL_id, GL33_Shader*> shaders_;
static GL33_Shader* sprite_shader_;
static GL33_Shader* mesh2D_shader_;
static GL33_Shader* mesh3D_shader_;


//plus tard, faire du batching par material et du batching par texture atlas aussi



/** API */

void GL33_Init() {}

void GL33_InitContext(HRL_uint _width, HRL_uint _height, void* loader)
{
  // ici _handle = contexte d�j� cr�� / rendu attach�
  // GLAD doit charger les fonctions : 
  // tu passes une fonction de platform loader adapt�e
  // ex: wglGetProcAddress sur Windows, glXGetProcAddress sur Linux

  if (!gladLoadGLLoader((GLADloadproc)loader))
  {
    printf("Failed to init GLAD\n");
    return;
  }

  // setup viewport par d�faut
  glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);

  printf("GL33 Context created: %ux%u\n", _width, _height);

  //on crée les vao
  glGenVertexArrays(Buffer_Num, vao);
  //on crée les vbo
  glGenBuffers(Buffer_Num, vbo);
  //on crée les ebo
  glGenBuffers(Buffer_Num, ebo);

  //on va initialiser le vao et vbo Sprite
  glBindVertexArray(vao[Sprite_Buffer]);

  //on bind le vbo sprite
  glBindBuffer(GL_ARRAY_BUFFER, vbo[Sprite_Buffer]);

  //allouer la bonne taille mais sans valeur par d�faut
  //16 car on passe 2 float et 2 coordon�es UV par vertex (donc 16 en tout pour 4 vertex)
  glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), nullptr, GL_STATIC_DRAW);

  //coordon�es des points
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //coordon�es de texture (UV)
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
  glEnableVertexAttribArray(1);

  //on bind le ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[Sprite_Buffer]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprite_indices), sprite_indices, GL_STATIC_DRAW);

  //gerer les vao Mesh2D et Mesh3D ici

  //on unbind le vao
  glBindVertexArray(0);


  //on crée les shaders
  sprite_shader_ = new GL33_Shader((const char*)gl_sprite_vert_shader, gl_sprite_vert_shader_len, (const char*)gl_sprite_frag_shader, gl_sprite_frag_shader_len);
}

void GL33_Shutdown()
{
  glDeleteVertexArrays(Buffer_Num, vao);
  glDeleteBuffers(Buffer_Num, vbo);
  glDeleteBuffers(Buffer_Num, ebo);
}

void GL33_BeginFrame()
{

}

void GL33_EndFrame()
{

}

void GL33_CreateMesh(HRL_uint _type)
{

}

void GL33_DeleteMesh(HRL_id _meshid)
{

}

HRL_BackendHandle GL33_CreateShader(const char *_vertContent, size_t _vertSize, const char *_fragContent, size_t _fragSize)
{
  auto* s = new GL33_Shader(_vertContent, _vertSize, _fragContent, _fragSize);
}
