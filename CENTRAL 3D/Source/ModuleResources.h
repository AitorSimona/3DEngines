#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"
#include "Globals.h"
#include "Resource.h"

//class ResourceShader;
//
//struct ResourceMeta
//{
//	uint Date = 0;
//	uint UID = 0;
//};

class ModuleResources : public Module
{
public:

	// --- Basic ---
	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Init(json file);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	// --- Importing ---
	void SearchAssets(const char* directory, std::vector<std::string>& filters);
	void ImportAssets(const char* path);
	Resource::ResourceType GetResourceTypeFromPath(const char* path);


	/*void CreateMetaFromUID(uint UID, const char* filename);

	bool IsFileImported(const char* file);
	Resource* GetResource(const char* original_file);
	Resource::ResourceType GetResourceTypeFromPath(const char* path);
	uint GetUIDFromMeta(const char* file);
	uint GetModDateFromMeta(const char* file);

	std::map<std::string, ResourceShader*>* GetShaders();
	void SaveAllShaders();


	Resource* CreateResource(Resource::ResourceType type);
	void AddResource(Resource* res);
	void AddShader(ResourceShader* shader);*/


private:

	//// --- Available shaders ---
	//std::map<std::string, ResourceShader*> shaders;

	// --- Resources in memory ---
	std::map<uint, Resource*> resources;

	//// --- Resources in library ---
	//std::map<uint, ResourceMeta> LoadedResources;
};

#endif