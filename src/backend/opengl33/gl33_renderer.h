#ifndef HRL_GL33_RENDERER
#define HRL_GL33_RENDERER

#include "../../hrl.h"
#include "../../backend_vtable.h"

void GL33_Init();
void GL33_InitContext(HRL_uint _width, HRL_uint _height, void* loader);
void GL33_Shutdown();

void GL33_BeginFrame();
void GL33_EndFrame();

void GL33_CreateMesh(HRL_uint _type);
void GL33_DeleteMesh(HRL_id _meshid);


HRL_BackendHandle GL33_CreateShader(const char* _vertContent, size_t _vertSize, const char* _fragContent, size_t _fragSize);
void GL33_DeleteShader(HRL_BackendHandle _handleid);

#endif