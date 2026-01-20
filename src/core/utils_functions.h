/**
 * Fonctions utiles pour la communication entre l'API (l'utilisateur final) et le backend
 */

#ifndef HRL_UTILS_FUNCTIONS
#define HRL_UTILS_FUNCTIONS

#include "../hrl.h"

#include <string>
#include <glm/glm.hpp>

void SetErrorCode(const std::string& e);

HRL_id GenerateHRL_ID();

unsigned int GetWindowWidth();
unsigned int GetWindowHeight();

glm::vec3 GetForwardVector(glm::vec3 _rotation);
glm::vec3 GetRightVector(glm::vec3 _rotation);
glm::vec3 GetUpVector(glm::vec3 _rotation);

#endif