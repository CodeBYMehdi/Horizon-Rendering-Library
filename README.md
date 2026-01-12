# HRL Engine

HRL is a lightweight, multi-API rendering engine supporting OpenGL, Vulkan, DirectX, Metal, and modern consoles (NVN, GNM).  
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

---

## Initialization

void HRL_Init(HRL_uint api);
apis: HRL_OpenGL33, HRL_OpenGL45, HRL_Vulkan, HRL_DX11, HRL_DX12, HRL_Metal, HRL_NVN, HRL_GNM

void HRL_InitContext(HRL_uint width, HRL_uint height, void* loader);
width / height: window or rendering context size
loader: function to retrieve API function pointers (e.g., wglGetProcAddress, vkGetInstanceProcAddr)

void HRL_Shutdown();
