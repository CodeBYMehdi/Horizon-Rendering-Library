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