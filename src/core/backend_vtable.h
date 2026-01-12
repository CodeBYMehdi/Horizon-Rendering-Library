#ifndef HRL_BACKEND_VTABLE
#define HRL_BACKEND_VTABLE

//on peut inclure hrl.h car hrl.h n'inclura jamais ce fichier
#include "../hrl.h"
#include "object_types.h"

typedef struct {
	void(*RHI_Init)();
	void(*RHI_InitContext)(HRL_uint width, HRL_uint height, void* loader);
	void(*RHI_Shutdown)();

	void(*RHI_BeginFrame)();
	void(*RHI_EndFrame)();

	void(*RHI_WindowResizeCallback)(int width, int height);

	void(*RHI_DrawMesh)(

	void(*RHI_CreateLight)(HRL_uint type, HRL_Light* light);
	void(*RHI_DeleteLight)(HRL_id lightid);

	void(*RHI_CreateTexture)(HRL_uint type, const char* fileContent, size_t , HRL_Texture* texture);
	void(*RHI_DeleteTexture)(HRL_BackendHandle textureHandle);

	void(*RHI_CreatePostProcess)(HRL_id matid, HRL_PostProcess* post);
	void(*RHI_DeletePostProcess)(HRL_id postid);

	HRL_BackendHandle(*RHI_CreateShader)(const char* vertContent, size_t vertSize, const char* fragContent, size_t fragSize);
	void(*RHI_DeleteShader)(HRL_BackendHandle shaderHandle);
}HRL_vtable;

#endif