# HRL Engine

HRL is a hight quality, multi-API rendering engine made for professional use. It supports OpenGL, Vulkan, DirectX, Metal, and modern consoles (NVN, GNM).
It provides a simple C/C++ API to manage meshes, lights, textures, shaders, materials, viewports, and cameras.

---

## Table of Contents

- [Installation](#installation)  
- [Initialization](#initialization)  
- [Frame](#frame)  
- [Window](#window)  
- [Error Handling](#error-handling)  
- [Meshes & Sprites](#meshes--sprites)  
- [Lights](#lights)  
- [Textures](#textures)  
- [Shaders & Materials](#shaders--materials)  
- [Post-process](#post-process)  
- [Viewports](#viewports)  
- [Cameras](#cameras)  
- [Constants & IDs](#constants--ids)  

---

## Installation

Include `HRL.h` in your file:

```c
#include "HRL.h"
```

## Initialization

```c
void HRL_Init(HRL_uint api);
```
apis: HRL_OpenGL33, HRL_OpenGL45, HRL_Vulkan, HRL_DX11, HRL_DX12, HRL_Metal, HRL_NVN, HRL_GNM

```c
void HRL_InitContext(HRL_uint width, HRL_uint height, void* loader);
```
width / height: window or rendering context size
loader: function to retrieve API function pointers (e.g., wglGetProcAddress, vkGetInstanceProcAddr)

```c
void HRL_Shutdown();
```

## Frame

```c
void HRL_BeginFrame();
```
Refresh and clear the viewport (needs to be called before EndFrame();

```c
void HRL_EndFrame();
```
Render the scene.


## Window

HRL doesn't manage any window. You need to inform HRL of what is happening with the window

```c
void HRL_WindowResizeCallback(int width, int height);
```
Call when the window is resized to update viewport and projection matrices.


## Error Handling

```c
const char* HRL_GetLastError();
```
Returns the last error message as a string.


## Meshes & Sprites

```c
HRL_id HRL_CreateMesh(HRL_uint type);
void HRL_DeleteMesh(HRL_id meshid);
void HRL_SetMeshMaterial(HRL_id meshid, HRL_id matid);
void HRL_SetMeshLocation(HRL_id meshid, float x, float y, float z);
void HRL_SetMeshRotation(HRL_id meshid, float yaw, float pitch, float roll);
void HRL_SetMeshScale(HRL_id meshid, float x, float y, float z);
```

Supported mesh types:








