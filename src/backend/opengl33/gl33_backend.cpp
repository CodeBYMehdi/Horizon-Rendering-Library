#include "gl33_backend.h"

#include "gl33_renderer.h"
#include "gl33_texture.h"

HRL_vtable GetOpenGL33Backend()
{
	HRL_vtable vtable;

	vtable.RHI_Init = GL33_Init;
	vtable.RHI_InitContext = GL33_InitContext;
	vtable.RHI_Shutdown = GL33_Shutdown;

	vtable.RHI_BeginFrame = GL33_BeginFrame;
	vtable.RHI_BindViewport = GL33_BindViewport;
	vtable.RHI_BindMaterial = GL33_BindMaterial;
	vtable.RHI_DrawMesh = GL33_DrawMesh;

	vtable.RHI_CreateTexture = GL33_CreateTexture;
	vtable.RHI_DeleteTexture = GL33_DeleteTexture;

	vtable.RHI_CreateShader = GL33_CreateShader;
	vtable.RHI_DeleteShader = GL33_DeleteShader;

	return vtable;
}