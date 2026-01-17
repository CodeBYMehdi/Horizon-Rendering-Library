/**
 * Fonctions utiles pour la communication entre l'API (l'utilisateur final) et le backend
 */

#ifndef HRL_UTILS_FUNCTIONS
#define HRL_UTILS_FUNCTIONS

#include "../hrl.h"

#include <string>

void SetErrorCode(const std::string& e);

HRL_id GenerateHRL_ID();

unsigned int GetWindowWidth();
unsigned int GetWindowHeight();

#endif