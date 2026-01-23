#include "gl33_renderer.h"
#include "../../ressources/ressources.h"
#include "gl33_shader.h"
#include "gl33_texture.h"

#include "../../core/utils_functions.h"
#include "../../hrl.h"

//on va faire les implémentations des fonctions gl-api
#include "../../hrl_gl.h"

//GL et GLM
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Debug printf
#include <cstdio>

//Memory
#include <unordered_map>
#include <string>


#define Buffer_Num      3

#define Sprite_Buffer   0
#define Mesh2D_Buffer   1
#define Mesh3D_Buffer   2


//ordre d'importance du batching :
// 1 - Type de mesh, ex : sprite, mesh2D, etc
// 2 - Texture
// 3 - Material
// 4 - Shader


//OpenGL objects//
static GLuint vao[Buffer_Num];
static GLuint vbo[Buffer_Num];
static GLuint ebo[Buffer_Num];


//Ubo : Uniform partagés entre plusieurs shaders
#define Ubo_Num         2

#define LIGHT_UBO       0
#define CAMERA_UBO      1

static GLuint ubo[Ubo_Num];


#define MAX_LIGHTS      32


float sprite_vertices[] = {
  // pos      // uv
  0.f, 0.f,   0.f, 0.f,
  1.f, 0.f,   1.f, 0.f,
  1.f, 1.f,   1.f, 1.f,
  0.f, 1.f,   0.f, 1.f
};

//indices pour deux triangles
unsigned int sprite_indices[] = {
    0, 1, 2,
    2, 3, 0
};


//light
/**
 * La norme std 140 de opengl a respecter pour les ubo demande d'alligner les objets
 * sur des multiples de 16, donc on ajoute des paddings pour correspondre
 */
typedef struct {
  //16 bytes {
  //4 bytes
  uint32_t type;
  //4 bytes
  float intensity;
  //4 bytes
  float attenuation;
  //4 bytes
  float padding1;
  // }

  //16 bytes {
  //12 bytes
  glm::vec3 position;
  //4 bytes
  float padding2;
  // }

  //16 bytes {
  //12 bytes
  glm::vec3 rotation;
  //4 bytes
  float padding3;
  // }

  //16 bytes {
  //12 bytes
  glm::vec3 color;
  //4 bytes
  float padding4;
  // }
}GL_Light_t;


void check_errors(int line)
{
  GLenum err; \
  while ((err = glGetError()) != GL_NO_ERROR) \
    printf("OpenGL error: 0x%X after line %d\n", err, line);
}
#define GL33_CheckErrors() check_errors(__LINE__)


//shaders//
std::unordered_map<HRL_id, GL33_Shader*> shaders_;


//Textures//
static std::unordered_map<HRL_id, GL33_Texture*> textures_;


/** Backend Implementation */

void GL33_Init() {}

void GL33_InitContext(HRL_uint _width, HRL_uint _height, void* loader)
{
  // ici _handle = contexte déja créé / rendu attaché
  // GLAD doit charger les fonctions : 
  // tu passes une fonction de platform loader adaptée
  // ex: wglGetProcAddress sur Windows, glXGetProcAddress sur Linux

  if (!gladLoadGLLoader((GLADloadproc)loader))
  {
    SetErrorCode("Failed to init GLAD, (loader error)");
    return;
  }

  //on crée les vao
  glGenVertexArrays(Buffer_Num, vao);
  //on crée les vbo
  glGenBuffers(Buffer_Num, vbo);
  //on crée les ebo
  glGenBuffers(Buffer_Num, ebo);

  //on crée le ubo
  glGenBuffers(Ubo_Num, ubo);

  //ubo light
  glBindBuffer(GL_UNIFORM_BUFFER, ubo[LIGHT_UBO]);
  glBufferData(GL_UNIFORM_BUFFER, MAX_LIGHTS * sizeof(HRL_Light), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo[LIGHT_UBO]);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  //mettre ici le ubo camera
  //
  //

  //on va initialiser le vao et vbo Sprite
  glBindVertexArray(vao[Sprite_Buffer]);

  //on bind le vbo sprite
  glBindBuffer(GL_ARRAY_BUFFER, vbo[Sprite_Buffer]);

  //allouer la bonne taille mais sans valeur par défaut
  //16 car on passe 2 float et 2 coordonées UV par vertex (donc 16 en tout pour 4 vertex)
  glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), nullptr, GL_DYNAMIC_DRAW);

  //coordonées des points
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //coordonées de texture (UV)
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
  glEnableVertexAttribArray(1);

  //on bind le ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[Sprite_Buffer]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprite_indices), sprite_indices, GL_DYNAMIC_DRAW);

  //gerer les vao Mesh2D et Mesh3D ici
  //
  //


  //on crée les shaders
  //ajouter une gestion des erreurs
  auto* spriteShader = new GL33_Shader();
  spriteShader->GL33_Create(
    (const char*)res_sprite_vert_glsl,
    res_sprite_vert_glsl_len,
    (const char*)res_sprite_frag_glsl,
    res_sprite_frag_glsl_len);

  shaders_.emplace(HRL_SpriteShader, spriteShader);
}

void GL33_Shutdown()
{
  glDeleteVertexArrays(Buffer_Num, vao);
  glDeleteBuffers(Buffer_Num, vbo);
  glDeleteBuffers(Buffer_Num, ebo);
}

void GL33_BeginFrame()
{
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

//passer plus tard a une structure RenderContext
static HRL_Viewport* currentViewport;
static HRL_Camera* currentCamera;
static GL33_Shader* currentShader;

void GL33_BindViewport(HRL_Viewport* viewport)
{
  glViewport((GLint)(viewport->x_*(float)GetWindowWidth()),
    //pour que 0 soit le haut et 1 le bas
    (GLint)((1 - viewport->y_ - viewport->height_)*(float)GetWindowHeight()),
    (GLint)(viewport->width_ * (float)GetWindowWidth()),
    (GLint)(viewport->height_ * (float)GetWindowHeight()));
  currentViewport = viewport;
  currentCamera = viewport->camera_;
}

void GL33_BindMaterial(HRL_Material* mat)
{
  auto it = shaders_.find(mat->shader_);
  if (it == shaders_.end())
  {
    SetErrorCode("Bind material error : shader doesn't exists");
    return;
  }
  else
  {
    auto* s = it->second;
    //on set CurrentShader pour spécifier que les prochains calls utiliseront ce shader
    currentShader = s;
    s->Use();

    //taille absolue du viewport width et height (on prend en compte la taille de la fenetre et la taille relative du viewport HRL)
    float viewportWidth  = (float)GetWindowWidth() * currentViewport->width_;
    float viewportHeight = (float)GetWindowHeight() * currentViewport->height_;

    //calcul du ratio largeur/hauteur du viewport
    float aspect = viewportWidth / viewportHeight;

    glm::mat4 proj;
    if (currentCamera->type_ == HRL_Perspective)
    {
      proj = glm::perspective(glm::radians(currentCamera->value_), aspect, currentCamera->near_plane_, currentCamera->far_plane_);
    }
    else
    {
      //on calcule la taille en hauteur d'abord, puis on fait le calcul de la largeur en fonction de la hauteur et de l'aspect
      float halfHeight = currentCamera->value_ * 0.5f;
      float halfWidth  = halfHeight * aspect;
      proj = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
                        currentCamera->near_plane_, currentCamera->far_plane_);
    }
    s->SetMat4("projection", proj);

    //position et vue de la camera
    glm::mat4 view = glm::lookAt(
      currentCamera->position_,
      currentCamera->position_ + GetForwardVector(currentCamera->rotation_),
      GetUpVector(currentCamera->rotation_)
    );
    s->SetMat4("view", view);

    //on passe tous les uniforms donnés par l'utilisateur
    for (auto [name, value] : mat->intParams_)
    {
      s->SetInt(name, value);
    }

    size_t index = 0;
    for (auto [name, value] : mat->textureParams_)
    {
      //on recherche la texture via son id HRL
      auto itTexture = textures_.find(value);
      if (itTexture == textures_.end())
      {
        SetErrorCode("Material set texture uniform : invalid texture (HRL_id)");
        return;
      }

      //la classe texture gere le bind sur le bon container de texture (GL_TEXTURE0, GL_TEXTURE1, ...).
      glActiveTexture(GL_TEXTURE0 + index);
      glBindTexture(GL_TEXTURE_2D, itTexture->second->GetGL_ID());

      s->SetInt(name, (int)index);

      index++;
    }

    for (auto [name, value] : mat->floatParams_)
    {
      s->SetFloat(name, value);
    }
    for (auto [name, value] : mat->vec2Params_)
    {
      s->SetVec2(name, value);
    }
    for (auto [name, value] : mat->vec3Params_)
    {
      s->SetVec3(name, value);
    }
    for (auto [name, value] : mat->vec4Params_)
    {
      s->SetVec4(name, value);
    }
  }
}

void GL33_DrawMesh(HRL_Mesh* mesh)
{
  glm::mat4 model = glm::mat4(1.f);
  model = glm::translate(model, mesh->position_);

  //rotation autour de X
  model = glm::rotate(model, mesh->rotation_.x, glm::vec3(1.f, 0.f, 0.f));
  //Y
  model = glm::rotate(model, mesh->rotation_.y, glm::vec3(0.f, 1.f, 0.f));
  //Z
  model = glm::rotate(model, mesh->rotation_.z, glm::vec3(0.f, 0.f, 1.f));

  model = glm::scale(model, mesh->scale_);
  currentShader->SetMat4("model", model);

  //on bind le bon vao (pas besoin de bind de vbo ou ebo car ils sont configurés pour le vao)
  glBindVertexArray(vao[Sprite_Buffer]);

  //on bind le vbo et on lui envoie les données du mesh
  glBindBuffer(GL_ARRAY_BUFFER, vbo[Sprite_Buffer]);

  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite_vertices), sprite_vertices);

  //desactiver si 2D, activer si 3D
  glDisable(GL_DEPTH_TEST);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


void GL33_UpdateLights(const std::vector<HRL_Light*>& _lights)
{
  //on commence par creer le tableau de GL_Lights_t
  GL_Light_t gpuLights[MAX_LIGHTS];
  size_t count = 0;

  //on rempli le tableau
  for (const auto& light: _lights)
  {
    if (count >= MAX_LIGHTS)
    {
      break;
    }

    gpuLights[count].type = light->type_;
    gpuLights[count].intensity = light->intensity_;
    gpuLights[count].attenuation = light->attenuation_;
    gpuLights[count].padding1 = 0.f;

    gpuLights[count].position = light->position_;
    gpuLights[count].padding2 = 0.f;

    gpuLights[count].rotation = light->rotation_;
    gpuLights[count].padding3 = 0.f;

    gpuLights[count].color = light->color_;
    gpuLights[count].padding4 = 0.f;

    ++count;
  }

  //on passe les données à opengl
  glBindBuffer(GL_UNIFORM_BUFFER, ubo[LIGHT_UBO]);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, count * sizeof(GL_Light_t), gpuLights);
  GL33_CheckErrors();
}


HRL_id GL33_CreateTexture(const char* _imageContent, const size_t _imageSize)
{
  //on crée la texture et on récupere le code d'erreur
  auto* t = new GL33_Texture();
  int error = t->GL33_Create(_imageContent, _imageSize);

  //si il n'y a pas d'erreur, on génere un ID et on push la texture dans la liste des textures, sinon on retourne invalid
  //la classe texture s'occupe des codes d'erreurs HRL, pas besoin de le faire ici.
  if (error == 0)
  {
    HRL_id id = GenerateHRL_ID();
    textures_.emplace(id, t);
    return id;
  }
  else
  {
    return HRL_InvalidID;
  }
}

void GL33_DeleteTexture(HRL_id _id)
{
  auto it = textures_.find(_id);
  if (it == textures_.end())
  {
    SetErrorCode("DeleteTexture error : Texture ID doesn't exists");
    return;
  }
  else
  {
    delete it->second;
    textures_.erase(it);
  }
}



HRL_id GL33_CreateShader(const char *_vertContent, size_t _vertSize, const char *_fragContent, size_t _fragSize)
{
  //on crée le shader et on recupere le code d'erreur
  auto* s = new GL33_Shader();
  int error = s->GL33_Create(_vertContent, _vertSize, _fragContent, _fragSize);

  //si il n'y a pas d'erreur, on génere un ID et on push le shader dans la liste des shaders, sinon on retourne invalid
  //la classe shader s'occupe des codes d'erreurs HRL, pas besoin de le faire ici.
  if (error == 0)
  {
    HRL_id id = GenerateHRL_ID();
    shaders_.emplace(id, s);
    return id;
  }
  else
  {
    return HRL_InvalidID;
  }
}

void GL33_DeleteShader(HRL_id _id)
{
  auto it = shaders_.find(_id);
  if (it == shaders_.end())
  {
    SetErrorCode("DeleteShader error : Shader ID doesn't exists");
    return;
  }
  else
  {
    delete it->second;
    shaders_.erase(it);
  }
}








///////// HRL_GL (hrl_gl.h) /////////

unsigned int HRL_GL_GetTextureGL_ID(HRL_id _textureid)
{
  auto it = textures_.find(_textureid);
  if (it == textures_.end())
  {
    SetErrorCode("DeleteTexture error : Texture ID doesn't exists");
    return GL_INVALID_VALUE;
  }

  return it->second->GetGL_ID();
}


unsigned int HRL_GL_GetShaderGL_ID(HRL_id _shaderid)
{
  auto it = shaders_.find(_shaderid);
  if (it == shaders_.end())
  {
    SetErrorCode("DeleteShader error : Shader ID doesn't exists");
    return GL_INVALID_VALUE;
  }

  return it->second->GetId();
}