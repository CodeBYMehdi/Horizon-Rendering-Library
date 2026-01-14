#ifndef HRL_BACKEND_VTABLE
#define HRL_BACKEND_VTABLE

//on peut inclure hrl.h car hrl.h n'inclura jamais ce fichier
#include "../hrl.h"
#include "object_types.h"

typedef struct {
	//Init-Shutdown//
	void(*RHI_Init)();
	void(*RHI_InitContext)(HRL_uint width, HRL_uint height, void* loader);
	void(*RHI_Shutdown)();

	//Draw//
	void(*RHI_BeginFrame)();

	//Window//
	void(*RHI_WindowResizeCallback)(int width, int height);

	//Textures//
	HRL_id(*RHI_CreateTexture)(HRL_uint type, const char* imageContent, const size_t imageSize);
	void(*RHI_DeleteTexture)(HRL_id id);

	//Shaders//
	HRL_id(*RHI_CreateShader)(const char* vertContent, size_t vertSize, const char* fragContent, size_t fragSize);
	void(*RHI_DeleteShader)(HRL_id id);
}HRL_vtable;

#endif