#ifndef HRL_OBJECT_TYPES
#define HRL_OBJECT_TYPES

#include "../hrl.h"
#include <unordered_map>
#include <glm/glm.hpp>

typedef uint64_t HRL_BackendHandle;

//MESH
typedef struct {
  HRL_uint type_;

  HRL_id material_;

  float draw_order_;

  glm::vec3 position_;
  glm::vec3 rotation_;
  glm::vec3 scale_;
}HRL_Mesh;

//LIGHT
typedef struct {
  HRL_uint type_;
  float intensity_;
  float attenuation_;
  /**
   * On utilise des vec4 pour eviter de poser des problemes de paddings pour certains backends
   */
  glm::vec3 position_;
  glm::vec3 rotation_;

  glm::vec3 color_;
}HRL_Light;

//POST PROCESS
typedef struct {
  HRL_id material_;
}HRL_PostProcess;

//MATERIAL
typedef struct {
  HRL_id shader_;

  std::unordered_map<const char*, int> intParams_;
  std::unordered_map<const char*, HRL_id> textureParams_;
  std::unordered_map<const char*, float> floatParams_;
  std::unordered_map<const char*, glm::vec2> vec2Params_;
  std::unordered_map<const char*, glm::vec3> vec3Params_;
  std::unordered_map<const char*, glm::vec4> vec4Params_;
}HRL_Material;

//CAMERA
typedef struct {
  HRL_uint type_;

  glm::vec3 position_;
  glm::vec3 rotation_;

  float value_;
  float near_plane_;
  float far_plane_;
}HRL_Camera;

//VIEWPORT
typedef struct {
  HRL_Camera* camera_;

  float x_;
  float y_;
  float width_;
  float height_;
}HRL_Viewport;

#endif