#include "hrl.h"

#include "backend_vtable.h"

#include "backend/opengl33/gl33_backend.h"

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

//Generer un id pour les objets HRL
static HRL_id currentID = 0;
static HRL_id GenerateID()
{
	return currentID++;
}

//on nomme les structures static parceque elles ne sont visibles que dans l'implementation
typedef struct {
	HRL_uint type_;

	HRL_id material_;

	glm::vec3 position_;
	glm::vec3 rotation_;
	glm::vec3 scale_;
}static_HRL_Mesh;

typedef struct {
	HRL_uint type_;

	glm::vec3 position_;
	glm::vec3 rotation_;

	glm::vec3 color_;
	float intensity_;
	float attenuation_;
}static_HRL_Light;

typedef struct {
	HRL_uint type_;
	//stocke l'id généré par le backend
	HRL_BackendHandle backend_handle_;
}static_HRL_Texture;

typedef struct {
	HRL_id material_;
}static_HRL_PostProcess;

typedef struct {
	//le handle repr�sente l'id du shader program
	HRL_BackendHandle backend_handle_;
}static_HRL_Shader;

typedef struct {
	HRL_id shader_;

	std::unordered_map<const char*, int> intParams_;
	std::unordered_map<const char*, HRL_id> textureParams_;
	std::unordered_map<const char*, float> floatParams_;
	std::unordered_map<const char*, glm::vec2> vec2Params_;
	std::unordered_map<const char*, glm::vec3> vec3Params_;
	std::unordered_map<const char*, glm::vec4> vec4Params_;
}static_HRL_Material;

typedef struct {
	HRL_uint type_;

	glm::vec3 position_;
	glm::vec3 rotation_;

	float value_;
}static_HRL_Camera;

//vtable utilis�e pour appeller les fonctions, ne doit jamais etre modifi� apres Init()
static static_HRL_vtable g_Backend;

static std::unordered_map<HRL_id, static_HRL_Mesh*> meshes_;
static std::unordered_map<HRL_id, static_HRL_Light*> lights_;
static std::unordered_map<HRL_id, static_HRL_Texture*> textures_;
static std::unordered_map<HRL_id, static_HRL_PostProcess*> postprocesses_;
static std::unordered_map<HRL_id, static_HRL_Shader*> shaders_;
static std::unordered_map<HRL_id, static_HRL_Material*> materials_;

static static_HRL_Camera camera_;

static std::string lastErrorCode;

void HRL_Init(HRL_uint _api)
{
	switch (_api)
	{
	case HRL_OpenGL33 :
	{
		g_Backend = GetOpenGL33Backend();
		g_Backend.RHI_Init();
		break;
	}
	case HRL_OpenGL45 :
	{
		break;
	}
	case HRL_Vulkan :
	{
		break;
	}
	case HRL_DX11 :
	{
		break;
	}
	case HRL_DX12 :
	{
		break;
	}
	case HRL_Metal :
	{
		break;
	}
	case HRL_NVN :
	{
		break;
	}
	case HRL_GNM :
	{
		break;
	}
	default :
	{
		assert("HRL : Backend not supported");
		break;
	}
	}
}

void HRL_InitContext(HRL_uint _width, HRL_uint _height, void* _loader)
{
	g_Backend.RHI_InitContext(_width, _height, _loader);
}

void HRL_Shutdown()
{
	//supprimer tous les objets
	for (auto [id, mesh] : meshes_)
	{
		delete mesh;
	}
	meshes_.clear();

	for (auto [id, light] : lights_)
	{
		delete light;
	}
	lights_.clear();	
	
	for (auto [id, texture] : textures_)
	{
		g_Backend.RHI_DeleteTexture(texture->backend_handle_);
		delete texture;
	}
	textures_.clear();

	for (auto [id, post] : postprocesses_)
	{
		delete post;
	}
	postprocesses_.clear();

	for (auto [id, shader] : shaders_)
	{
		g_Backend.RHI_DeleteShader(shader->backend_handle_);
		delete shader;
	}
	shaders_.clear();

	for (auto [id, material] : materials_)
	{
		delete material;
	}
	materials_.clear();

	g_Backend.RHI_Shutdown();
}

void HRL_BeginFrame()
{
	g_Backend.RHI_BeginFrame();
}

void HRL_EndFrame()
{
	g_Backend.RHI_EndFrame();
}

const char* HRL_GetLastError()
{
	//on stocke dans une var statique pour eviter un use after free
	static std::string err;
	err = "HRL Error : " + lastErrorCode;
	return err.c_str();
}

HRL_id HRL_CreateMesh(HRL_uint _type)
{
	if (_type == HRL_Sprite || _type == HRL_2D_Mesh || _type == HRL_3D_Mesh)
	{
		auto* m = new static_HRL_Mesh();
		m->type_ = _type;
		//default values
		m->material_ = HRL_InvalidID;
		m->position_ = glm::vec3(0.f);
		m->rotation_ = glm::vec3(0.f);
		m->scale_ = glm::vec3(1.f);

		HRL_id newId = GenerateID();
		meshes_.emplace(newId, m);

		g_Backend.RHI_CreateMesh(_type);

		return newId;
	}
	else
	{
		lastErrorCode = "HRL_CreateMesh, type not supported";
		return HRL_InvalidID;
	}
}

void HRL_DeleteMesh(HRL_id _meshid)
{
	auto it = meshes_.find(_meshid);
	if (it == meshes_.end())
	{
		lastErrorCode = "HRL_DeleteMesh, invalid ID";
		return;
	}
	else
	{
		g_Backend.RHI_DeleteMesh(_meshid);
		delete it->second;
		meshes_.erase(_meshid);
	}
}

void HRL_SetMeshMaterial(HRL_id _meshid, HRL_id _matid)
{
	auto it = meshes_.find(_meshid);
	if (it == meshes_.end())
	{
		lastErrorCode = "HRL_SetMeshMaterial, invalid ID";
		return;
	}
	else
	{
		it->second->material_ = _matid;
	}
}

void HRL_SetMeshLocation(HRL_id _meshid, float x, float y, float z)
{
	auto it = meshes_.find(_meshid);
	if (it == meshes_.end())
	{
		lastErrorCode = "HRL_SetMeshLocation, invalid ID";
		return;
	}
	else
	{
		it->second->position_ = glm::vec3(x, y, z);
	}
}

void HRL_SetMeshRotation(HRL_id _meshid, float yaw, float pitch, float roll)
{
	auto it = meshes_.find(_meshid);
	if (it == meshes_.end())
	{
		lastErrorCode = "HRL_SetMeshRotation, invalid ID";
		return;
	}
	else
	{
		it->second->rotation_ = glm::vec3(yaw, pitch, roll);
	}
}

void HRL_SetMeshScale(HRL_id _meshid, float x, float y, float z)
{
	auto it = meshes_.find(_meshid);
	if (it == meshes_.end())
	{
		lastErrorCode = "HRL_SetMeshScale, invalid ID";
		return;
	}
	else
	{
		it->second->scale_ = glm::vec3(x, y, z);
	}
}

HRL_id HRL_CreateLight(HRL_uint _type)
{
	if (_type == HRL_PointLight || _type == HRL_DirectionalLight || _type == HRL_SpotLight)
	{
		auto* l = new static_HRL_Light();
		l->type_ = _type;

		//default values
		l->position_ = glm::vec3(0.f);
		l->rotation_ = glm::vec3(0.f);
		l->color_ = glm::vec3(1.f);
		l->intensity_ = 100.f;
		l->attenuation_ = 100.f;

		int newId = GenerateID();
		lights_.emplace(newId, l);

		g_Backend.RHI_CreateLight(_type);

		return newId;
	}
	else
	{
		lastErrorCode = "HRL_CreateLight, invalid type";
		return HRL_InvalidID;
	}
}

void HRL_DeleteLight(HRL_id _lightid)
{
	auto it = lights_.find(_lightid);
	if (it == lights_.end())
	{
		lastErrorCode = "HRL_DeleteLight, invalid ID";
		return;
	}
	else
	{
		g_Backend.RHI_DeleteLight(_lightid);
		delete it->second;
		lights_.erase(_lightid);
	}
}

void HRL_SetLightColor(HRL_id _lightid, float x, float y, float z)
{
	auto it = lights_.find(_lightid);
	if (it == lights_.end())
	{
		lastErrorCode = "HRL_SetLightColor, invalid ID";
		return;
	}
	else
	{
		it->second->color_ = glm::vec3(x, y, z);
	}
}

void HRL_SetLightIntensity(HRL_id _lightid, float i)
{
	auto it = lights_.find(_lightid);
	if (it == lights_.end())
	{
		lastErrorCode = "HRL_SetLightIntensity, invalid ID";
		return;
	}
	else
	{
		it->second->intensity_ = i;
	}
}

void HRL_SetLightAttenuation(HRL_id _lightid, float a)
{
	auto it = lights_.find(_lightid);
	if (it == lights_.end())
	{
		lastErrorCode = "HRL_SetLightAttenuation, invalid ID";
		return;
	}
	else
	{
		it->second->attenuation_ = a;
	}
}

void HRL_SetLightLocation(HRL_id _lightid, float x, float y, float z)
{
	auto it = lights_.find(_lightid);
	if (it == lights_.end())
	{
		lastErrorCode = "HRL_SetLightLocation, invalid ID";
		return;
	}
	else
	{
		it->second->position_ = glm::vec3(x, y, z);
	}
}

void HRL_SetLightRotation(HRL_id _lightid, float yaw, float pitch, float roll)
{
	auto it = lights_.find(_lightid);
	if (it == lights_.end())
	{
		lastErrorCode = "HRL_SetLightRotation, invalid ID";
		return;
	}
	else
	{
		it->second->rotation_ = glm::vec3(yaw, pitch, roll);
	}
}

HRL_id HRL_CreateTexture(HRL_uint _type, const char* _fileContent, size_t _bufferSize)
{
	auto* t = new static_HRL_Texture();

	HRL_id newId = GenerateID();
	textures_.emplace(newId, t);

	HRL_BackendHandle handle = g_Backend.RHI_CreateTexture(_type, _fileContent, _bufferSize);
	t->backend_handle_ = handle;
	
	return newId;
}

void HRL_DeleteTexture(HRL_id _textureid)
{
	auto it = textures_.find(_textureid);
	if (it == textures_.end())
	{
		lastErrorCode = "HRL_DeleteTexture, invalid ID";
		return;
	}
	else
	{
		g_Backend.RHI_DeleteTexture(it->second->backend_handle_);
		delete it->second;
		textures_.erase(it);
	}
}

HRL_id HRL_CreatePostProcess(HRL_id _matid)
{
	return HRL_InvalidID;
}

void HRL_DeletePostProcess(HRL_id _postid)
{

}

HRL_id HRL_CreateShader(const char *_vertContent, size_t _vertSize, const char *_fragContent, size_t _fragSize)
{
	auto* m = new static_HRL_Shader();

	HRL_id newId = GenerateID();
	shaders_.emplace(newId, m);

	HRL_BackendHandle handle = g_Backend.RHI_CreateShader(_vertContent, _vertSize, _fragContent, _fragSize);
	m->backend_handle_ = handle;

	return newId;
}

void HRL_DeleteShader(HRL_id _shaderid)
{

}


HRL_id HRL_CreateMaterial(HRL_id _shaderid)
{
	auto* m = new static_HRL_Material();
	m->shader_ = _shaderid;

	HRL_id newId = GenerateID();
	materials_.emplace(newId, m);

	return newId;
}

void HRL_DeleteMaterial(HRL_id _matid)
{
	auto it = materials_.find(_matid);
	if (it == materials_.end())
	{
		lastErrorCode = "HRL_DeleteMaterial, invalid ID";
		return;
	}
	else
	{
		delete it->second;
		materials_.erase(it);
	}
}

void HRL_MaterialSetInt(HRL_id _matid, const char* _uniformName, int a)
{
	auto it = materials_.find(_matid);
	if (it == materials_.end())
	{
		lastErrorCode = "HRL_MaterialSetInt, invalid ID";
		return;
	}
	else
	{
		//si la clée n'existe pas, elle est créée
		it->second->intParams_[_uniformName] = a;
	}
}

void HRL_MaterialSetTexture(HRL_id _matid, const char* _uniformName, HRL_id _textureid)
{
	//on v�rifie que le mat existe
	auto it_mat = materials_.find(_matid);
	if (it_mat == materials_.end())
	{
		lastErrorCode = "HRL_MaterialSetTexture, invalid material ID";
		return;
	}
	//puis on v�rifie que la texture existe
	auto it_tex = textures_.find(_textureid);
	if (it_tex == textures_.end())
	{
		lastErrorCode = "HRL_MaterialSetTexture, invalid texture ID";
		return;
	}

	it_mat->second->textureParams_[_uniformName] = _textureid;
}

void HRL_MaterialSetBool(HRL_id _matid, const char* _uniformName, int a)
{
	auto it = materials_.find(_matid);
	if (it == materials_.end())
	{
		lastErrorCode = "HRL_MaterialSetBool, invalid ID";
		return;
	}
	else
	{
		it->second->intParams_[_uniformName] = a;
	}
}

void HRL_MaterialSetFloat(HRL_id _matid, const char* _uniformName, float a)
{
	auto it = materials_.find(_matid);
	if (it == materials_.end())
	{
		lastErrorCode = "HRL_MaterialSetFloat, invalid ID";
		return;
	}
	else
	{
		it->second->floatParams_[_uniformName] = a;
	}
}

void HRL_MaterialSetVec2(HRL_id _matid, const char* _uniformName, float x, float y)
{
	auto it = materials_.find(_matid);
	if (it == materials_.end())
	{
		lastErrorCode = "HRL_MaterialSetVec2, invalid ID";
		return;
	}
	else
	{
		it->second->vec2Params_[_uniformName] = glm::vec2(x, y);
	}
}

void HRL_MaterialSetVec3(HRL_id _matid, const char* _uniformName, float x, float y, float z)
{
	auto it = materials_.find(_matid);
	if (it == materials_.end())
	{
		lastErrorCode = "HRL_MaterialSetVec3, invalid ID";
		return;
	}
	else
	{
		it->second->vec3Params_[_uniformName] = glm::vec3(x, y, z);
	}
}

void HRL_MaterialSetVec4(HRL_id _matid, const char* _uniformName, float x, float y, float z, float w)
{
	auto it = materials_.find(_matid);
	if (it == materials_.end())
	{
		lastErrorCode = "HRL_MaterialSetVec4, invalid ID";
		return;
	}
	else
	{
		it->second->vec4Params_[_uniformName] = glm::vec4(x, y, z, w);
	}
}