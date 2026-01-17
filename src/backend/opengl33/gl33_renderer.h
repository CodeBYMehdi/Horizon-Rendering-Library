#ifndef HRL_GL33_RENDERER
#define HRL_GL33_RENDERER

#include "../../hrl.h"
#include "../../core/backend_vtable.h"

void GL33_Init();
void GL33_InitContext(HRL_uint _width, HRL_uint _height, void* loader);
void GL33_Shutdown();

void GL33_BeginFrame();
void GL33_BindMaterial(HRL_Material* mat);

HRL_id GL33_CreateTexture(HRL_uint _type, const char* _imageContent, const size_t _imageSize);
void GL33_DeleteTexture(HRL_id _id);

HRL_id GL33_CreateShader(const char* _vertContent, size_t _vertSize, const char* _fragContent, size_t _fragSize);
void GL33_DeleteShader(HRL_id _id);

#endif