/**
 * Copyright (c) 2025-2026 Oscar Soirey
 * https://github.com/oscar-soirey/Horizon-Rendering-Library
 *
 * This project was developed by a single passionate developer
 * I've tried to make everything work smoothly, but there may still be bugs.
 * If you encounter any issues or have suggestions, please feel free to contact me at:
 * oscarsoirey.contact@gmail.com
 * Thank you for your support and understanding
 *
 * This code is the intellectual property of Oscar Soirey and is
 * licensed under the Apache License, Version 2.0. You may not use,
 * modify, or distribute this software except in compliance with the
 * License. A copy of the License can be obtained at:
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * By using or modifying this code, you agree to adhere to the terms
 * of the Apache 2.0 License.
 *
 *    ,--.  ,--.,------. ,--.
 *    |  '--'  ||  .--. '|  |
 *    |  .--.  ||  '--'.'|  |
 *    |  |  |  ||  |\  \ |  '--.
 *    `--'  `--'`--' '--'`-----'
 */

#ifndef HRL_IMPL
#define HRL_IMPL

#ifdef __cplusplus
 #include <cstdint>
 #include <iostream>
#else
 #include <stdint.h>
 #include <stdio.h>
#endif

//ajouter des debug views (genre activer que l'albedo ou que la normal, montrer la lumiere, les reflections, etc)
//suport multiviewport (avec HRL_CreateViewport et tout) (toujours 1 camera de rendu par viewport)

typedef uint32_t HRL_id;
typedef unsigned int HRL_uint;

//Bool
#define HRL_False ((int)0)
#define HRL_True  ((int)1)

//Errors
#define HRL_InvalidID										((HRL_id)-1)

//APIs
#define HRL_OpenGL33										0x0001
#define HRL_OpenGL45										0x0002
#define HRL_Vulkan											0x0003
#define HRL_DX11												0x0004
#define HRL_DX12												0x0005
#define HRL_Metal												0x0006
#define HRL_NVN													0x0007
#define HRL_GNM													0x0008

//Lights
#define HRL_PointLight									0x0011
#define HRL_DirectionalLight						0x0012
#define HRL_SpotLight										0x0013

//Meshes & sprite
#define HRL_Sprite											0x0021
#define HRL_2D_Mesh											0x0022
#define HRL_3D_Mesh											0x0023

//Debug geometry
#define HRL_DebugLine										0x0031
#define HRL_DebugBox										0x0032
#define HRL_DebugSphere									0x0033

//Textures
#define HRL_Tex_Albedo									0x0041
#define HRL_Tex_Normal									0x0042
#define HRL_Tex_Specular								0x0043
#define HRL_Tex_Roughness								0x0044
#define HRL_Tex_Metalic									0x0045
#define HRL_Tex_Alpha										0x0046
#define HRL_Tex_ShadowMap								0x0047
#define HRL_Tex_CubeMap									0x0048

#define HRL_Tex_Custom0									0x0049
#define HRL_Tex_Custom1									0x004A
#define HRL_Tex_Custom2									0x004B
#define HRL_Tex_Custom3									0x004C
#define HRL_Tex_Custom4									0x004D
#define HRL_Tex_Custom5									0x004E
#define HRL_Tex_Custom6									0x004F

//Camera
#define HRL_Ortho												0x0051
#define HRL_Perspective									0x0052


//Default Shaders (we reserve theses ID)
#define HRL_SpriteShader								(UINT32_MAX)
#define HRL_Mesh2DShader								(UINT32_MAX - 1)
#define HRL_Mesh3DShader								(UINT32_MAX - 2)


#ifdef __cplusplus
extern "C" {
#endif

	//HRL Init
	void HRL_Init(HRL_uint _api);
	void HRL_InitContext(HRL_uint _width, HRL_uint _height, void* _loader);
	void HRL_Shutdown();

	//HRL Frame
	void HRL_BeginFrame();
	void HRL_EndFrame();

	//Window
	/**
	 * Call this function when window is resized
	 */
	void HRL_WindowResizeCallback(int _width, int _height);


	//Errors
	const char* HRL_GetLastError();


	//HRL Meshes
	/**
	 * @param _type HRL_Sprite, HRL_2D_Mesh, HRL_3D_Mesh
	 * @return HRL_id of the new object
	 */
	HRL_id HRL_CreateMesh(HRL_uint _type);
	/**
	 * @param _meshid HRL_id of the mesh
	 */
	void HRL_DeleteMesh(HRL_id _meshid);

	/**
	 * @param _meshid HRL_id of the mesh
	 * @param _matid HRL_id of the mat
	 */
	void HRL_SetMeshMaterial(HRL_id _meshid, HRL_id _matid);
	void HRL_SetMeshLocation(HRL_id _meshid, float x, float y, float z);
	void HRL_SetMeshRotation(HRL_id _meshid, float roll, float pitch, float yaw);
	void HRL_SetMeshScale(HRL_id _meshid, float x, float y, float z);

	/**
	 * This will be used only when 2 sprites are collinding in Z axle
	 */
	void HRL_SetSpriteDrawOrder(HRL_id _meshid, float _draworder);


	//Lights
	/**
	 * @param _type HRL_PointLight, HRL_DirectionalLight, HRL_SpotLight
	 * @return HRL_id of the new object
	 */
	HRL_id HRL_CreateLight(HRL_uint _type);
	/**
	 * @param _meshid HRL_id of the light
	 */
	void HRL_DeleteLight(HRL_id _lightid);

	void HRL_SetLightColor(HRL_id _lightid, float x, float y, float z);
	void HRL_SetLightIntensity(HRL_id _lightid, float i);
	void HRL_SetLightAttenuation(HRL_id _lightid, float a);

	void HRL_SetLightLocation(HRL_id _lightid, float x, float y, float z);
	void HRL_SetLightRotation(HRL_id _lightid, float roll, float pitch, float yaw);


	//Textures
	/**
	 * @brief Supported extensions : png, jpeg, jpg, bmp, tga, gif (single image), hdr, psd (half)
	 * To have more information, you can visit the stb website
	 * @param _type HRL_Tex_Albedo, HRL_Tex_Normal, HRL_Tex_Specular,
	 * HRL_Tex_Roughness, HRL_Tex_Metalic, HRL_Tex_Alpha, HRL_Tex_ShadowMap
	 * @param _fileContent content of the file (supported extensions) (opened in binary mode)
	 * @return HRL_id of the new object
	 */
	HRL_id HRL_CreateTexture(HRL_uint _type, const char* _fileContent, size_t _bufferSize);
	/**
	 * @param _meshid HRL_id of the light
	 */
	void HRL_DeleteTexture(HRL_id _textureid);
	//ajouter des fonctions de controle des textures


	//Post Process
	HRL_id HRL_CreatePostProcess(HRL_id _matid);
	void HRL_DeletePostProcess(HRL_id _postid);


	//Shaders
	HRL_id HRL_CreateShader(const char* _vertContent, size_t _vertSize, const char* _fragContent, size_t _fragSize);
	void HRL_DeleteShader(HRL_id _shaderid);


	//Materials
	//Wich contains values used by the source shader
	/**
	 * @param _shaderContent Code of the fragment shader (opened in binary mode)
	 * @return HRL_id of the new object
	 */
	HRL_id HRL_CreateMaterial(HRL_id _shaderid);
	/**
	 * @param _meshid HRL_id of the light
	 */
	void HRL_DeleteMaterial(HRL_id _matid);

	void HRL_MaterialSetInt(HRL_id _matid, const char* _uniformName, int a);
	void HRL_MaterialSetTexture(HRL_id _matid, const char* _uniformName, HRL_id _textureid);
	void HRL_MaterialSetBool(HRL_id _matid, const char* _uniformName, int a);
	void HRL_MaterialSetFloat(HRL_id _matid, const char* _uniformName, float a);
	void HRL_MaterialSetVec2(HRL_id _matid, const char* _uniformName, float x, float y);
	void HRL_MaterialSetVec3(HRL_id _matid, const char* _uniformName, float x, float y, float z);
	void HRL_MaterialSetVec4(HRL_id _matid, const char* _uniformName, float x, float y, float z, float w);


	//Viewport
	/**
	 * @brief Allow to render world multiple times at the screen at same time.
	 * (Usefull for make splitscreen games for example).
	 * Normalized coordinates [0 - 1].
	 * [0,0] ----- [1,0]
	 * [0,1] ----- [1,1]
	 */
	HRL_id HRL_CreateViewport(HRL_id _cameraid, float x, float y, float _width, float _height);
	void HRL_DeleteViewport(HRL_id _viewportid);
	void HRL_SetViewportCamera(HRL_id _viewportid, HRL_id _camid);
	void HRL_SetViewportRect(HRL_id _viewportid, float x, float y, float _width, float _height);


	//Camera
	/**
	 * @brief Default type = ortho
	 * @param _viewportid Id of the viewport (wich includes the camera) (default viewport id = 0)
	 * @param _type HRL_Ortho, HRL_Perspective
	 */
	HRL_id HRL_CreateCamera(HRL_uint _type);
	void HRL_DeleteCamera(HRL_id _camid);

	void HRL_SetCameraType(HRL_id _camid, HRL_uint _type);
	void HRL_SetCameraOrthoVertical(HRL_id _camid, float _height);
	void HRL_SetCameraPerspectiveFov(HRL_id _camid, float _fov);

	void HRL_SetCameraNearPlane(HRL_id _camid, float _nearPlane);
	void HRL_SetCameraFarPlane(HRL_id _camid, float _farPlane);

	void HRL_SetCameraPosition(HRL_id _camid, float x, float y, float z);
	/**
	 * Backend information :
	 * Roll : X, Pitch : Y, Yaw : Z
	 */
	void HRL_SetCameraRotation(HRL_id _camid, float roll, float pitch, float yaw);


#ifdef __cplusplus
} //extern "C"
#endif


//macros
#ifdef __cplusplus  // C++
 #define HRL_CheckErrors() \
	std::cout << HRL_GetLastError() << ", " << __FILE__ << ", " << __LINE__ << std::endl

#else // C
#define HRL_CheckErrors() \
	printf("%s, %s, %d\n", HRL_GetLastError(), __FILE__, __LINE__)

#endif


#endif //HRL_IMPL