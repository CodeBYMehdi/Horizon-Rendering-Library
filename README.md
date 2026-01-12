# HRL Engine

HRL est un moteur de rendu léger, multi-API, supportant OpenGL, Vulkan, DirectX, Metal et les consoles modernes (NVN, GNM). Il permet de gérer des meshes, lumières, textures, shaders, matériaux et viewports avec une API simple en C/C++.

---

## Table des matières

- [Installation](#installation)  
- [Initialisation](#initialisation)  
- [Frame](#frame)  
- [Window](#window)  
- [Gestion des erreurs](#gestion-des-erreurs)  
- [Meshes & Sprites](#meshes--sprites)  
- [Lights](#lights)  
- [Textures](#textures)  
- [Shaders & Materials](#shaders--materials)  
- [Post-process](#post-process)  
- [Viewports](#viewports)  
- [Caméras](#caméras)  

---

## Installation

Inclure `HRL.h` dans votre projet et définir `HRL_IMPL` dans **un seul fichier** avant l’inclusion pour générer l’implémentation :

```c
#define HRL_IMPL
#include "HRL.h"
