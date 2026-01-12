#ifndef HRL_GL33_RENDERER
#define HRL_GL33_RENDERER

#include "../../hrl.h"
#include "../../core/backend_vtable.h"

void GL33_Init();
void GL33_InitContext(HRL_uint _width, HRL_uint _height, void* loader);
void GL33_Shutdown();

void GL33_BeginFrame(HRL_Light* lights, size_t lightCount);
void GL33_BindMaterial(HRL_Material* material, HRL_Shader* shader);
void GL33_DrawMesh(HRL_Mesh* mesh);

void GL33_CreateMesh(HRL_uint _type, HRL_Mesh* _mesh);
void GL33_DeleteMesh(HRL_id _meshid);

void GL33_CreateTexture(HRL_uint type, const char* fileContent, size_t , HRL_Texture* texture);
void GL33_DeleteTexture(HRL_BackendHandle textureHandle);

HRL_BackendHandle GL33_CreateShader(const char* _vertContent, size_t _vertSize, const char* _fragContent, size_t _fragSize);
void GL33_DeleteShader(HRL_BackendHandle _handleid);

#endif