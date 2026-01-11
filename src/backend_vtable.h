#ifndef HRL_BACKEND_VTABLE
#define HRL_BACKEND_VTABLE

#include <cstdint>

//on peut inclure hrl.h car hrl.h n'inclura jamais ce fichier
#include "hrl.h"

typedef uint64_t HRL_BackendHandle;

typedef struct {
	void(*RHI_Init)();
	void(*RHI_InitContext)(HRL_uint _width, HRL_uint _height, void* _loader);
	void(*RHI_Shutdown)();

	void(*RHI_BeginFrame)();
	void(*RHI_EndFrame)();

	void(*RHI_CreateMesh)(HRL_uint _type);
	void(*RHI_DeleteMesh)(HRL_id _meshid);

	void(*RHI_CreateLight)(HRL_uint _type);
	void(*RHI_DeleteLight)(HRL_id _lightid);

	HRL_BackendHandle(*RHI_CreateTexture)(HRL_uint _type, const char* _fileContent, size_t _bufferSize);
	void(*RHI_DeleteTexture)(HRL_BackendHandle _textureHandle);

	void(*RHI_CreatePostProcess)(HRL_id _matid);
	void(*RHI_DeletePostProcess)(HRL_id _postid);

	HRL_BackendHandle(*RHI_CreateShader)(const char* _vertContent, size_t _vertSize, const char* _fragContent, size_t _fragSize);
	void(*RHI_DeleteShader)(HRL_BackendHandle _shaderHandle);

	void(*RHI_MaterialSetInt)(HRL_BackendHandle _shaderid, const char* _uniformName, int a);
	void(*RHI_MaterialSetTexture)(HRL_BackendHandle _shaderid, const char* _uniformName, HRL_BackendHandle _textureHandle);
	void(*RHI_MaterialSetBool)(HRL_BackendHandle _shaderid, const char* _uniformName, int a);
	void(*RHI_MaterialSetFloat)(HRL_BackendHandle _shaderid, const char* _uniformName, float a);
	void(*RHI_MaterialSetVec2)(HRL_BackendHandle _shaderid, const char* _uniformName, float x, float y);
	void(*RHI_MaterialSetVec3)(HRL_BackendHandle _shaderid, const char* _uniformName, float x, float y, float z);
	void(*RHI_MaterialSetVec4)(HRL_BackendHandle _shaderid, const char* _uniformName, float x, float y, float z, float w);
}static_HRL_vtable;

#endif