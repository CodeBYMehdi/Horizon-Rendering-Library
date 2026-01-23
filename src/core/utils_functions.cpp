#include "utils_functions.h"

#include "../hrl.h"


//on récupere la variable d'erreur déclarée dans hrl.cpp
extern std::string lastErrorCode;
void SetErrorCode(const std::string& e)
{
	lastErrorCode = e;
}


//Generer un id pour les objets HRL
static HRL_id currentID = 0;
HRL_id GenerateHRL_ID()
{
	return currentID++;
}


//Window
extern unsigned int window_width_;
unsigned int GetWindowWidth()
{
	return window_width_;
}
extern unsigned int window_height_;
unsigned int GetWindowHeight()
{
	return window_height_;
}


glm::vec3 GetForwardVector(glm::vec3 _rot)
{
  glm::vec3 forward;
  forward.x = cos(glm::radians(_rot.x)) * cos(glm::radians(_rot.y));
  forward.y = sin(glm::radians(_rot.x));
  forward.z = cos(glm::radians(_rot.x)) * sin(glm::radians(_rot.y));
  return glm::normalize(forward);
}
glm::vec3 GetRightVector(glm::vec3 _rot)
{
  return glm::normalize(glm::cross(
    glm::vec3(0.0f, 1.0f, 0.0f),
    GetForwardVector(_rot)
  ));
}
glm::vec3 GetUpVector(glm::vec3 _rot)
{
  return glm::normalize(glm::cross(
    GetForwardVector(_rot),
    GetRightVector(_rot)
  ));
}


extern HRL_uint textureMinFilter;
HRL_uint GetTextureMinFilter()
{
  return textureMinFilter;
}
extern HRL_uint textureMagFilter;
HRL_uint GetTextureMagFilter()
{
  return textureMagFilter;
}