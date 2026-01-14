#ifndef HRL_OBJECT_TYPES
#define HRL_OBJECT_TYPES

#include "../hrl.h"
#include <unordered_map>
#include <glm/glm.hpp>

typedef uint64_t HRL_BackendHandle;

typedef struct {
  HRL_uint type_;

  HRL_id material_;

  glm::vec3 position_;
  glm::vec3 rotation_;
  glm::vec3 scale_;
}HRL_Mesh;

typedef struct {
  HRL_uint type_;

  glm::vec3 position_;
  glm::vec3 rotation_;

  glm::vec3 color_;
  float intensity_;
  float attenuation_;
}HRL_Light;

typedef struct {
  HRL_id material_;
}HRL_PostProcess;

typedef struct {
  HRL_id shader_;

  std::unordered_map<const char*, int> intParams_;
  std::unordered_map<const char*, HRL_id> textureParams_;
  std::unordered_map<const char*, float> floatParams_;
  std::unordered_map<const char*, glm::vec2> vec2Params_;
  std::unordered_map<const char*, glm::vec3> vec3Params_;
  std::unordered_map<const char*, glm::vec4> vec4Params_;
}HRL_Material;

typedef struct {
  HRL_uint type_;

  glm::vec3 position_;
  glm::vec3 rotation_;

  float value_;
}HRL_Camera;

#endif