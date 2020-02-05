#include "ModuleResources.h"
#include "Application.h"
#include "ModuleFileSystem.h"
#include "ModuleGui.h"

#include "Importers.h"
#include "Resources.h"

#include "Assimp/include/cimport.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "mmgr/mmgr.h"

// --- Get Assimp LOGS and print them to console ---
void MyAssimpCallback(const char* msg, char* userData)
{
	CONSOLE_LOG("[Assimp]: %s", msg);
}

ModuleResources::ModuleResources(bool start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init(json file)
{
	// --- Stream LOG messages to MyAssimpCallback, that sends them to console ---
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	aiAttachLogStream(&stream);

	// --- Create importers ---
	importers.push_back(new ImporterFolder());
	importers.push_back(new ImporterScene());
	importers.push_back(new ImporterModel());
	importers.push_back(new ImporterMaterial());
	importers.push_back(new ImporterShader());
	importers.push_back(new ImporterMesh());
	importers.push_back(new ImporterMeta());

	return true;
}

bool ModuleResources::Start()
{
	// --- Import all resources in Assets at startup ---
	App->gui->CreateIcons();

	std::vector<std::string> filters;
	filters.push_back("fbx");
	//filters.push_back("vertex");
	//filters.push_back("VERTEX");

	AssetsFolder = SearchAssets(nullptr, ASSETS_FOLDER, filters);

	return true;
}

// ------------------------------ IMPORTING --------------------------------------------------------

// DEPRECATED --- Create all ResourceFolder type resources ---
void ModuleResources::ImportAllFolders(const char* directory)
{
	std::vector<std::string> dirs;

	App->fs->DiscoverDirectories(directory, dirs);

	std::string dir((directory) ? directory : "");

	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		ImportAllFolders((dir + (*it) + "/").c_str());
	}

	ImportFolder(dir.c_str());
}

// --- Sweep over all files in given directory, if those files pass the given filters, call Import and if it succeeds add them to the given resource folder ---
ResourceFolder* ModuleResources::SearchAssets(ResourceFolder* parent, const char* directory, std::vector<std::string>& filters)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;

	std::string dir((directory) ? directory : "");

	App->fs->DiscoverFiles(dir.c_str(), files, dirs);

	// --- Import folder ---
	ResourceFolder* folder = (ResourceFolder*)ImportFolder(dir.c_str());

	// --- If parent is not nullptr add ourselves as childs ---
	if (parent)
		folder->SetParent(parent);

	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		SearchAssets(folder,(dir + (*it) + "/").c_str(), filters);
	}

	// --- Now import all of its engine-supported files ---
	std::sort(files.begin(), files.end());

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		const std::string& str = *it;

		bool pass_filter = false;

		if (filters.size() > 0)
		{
			for (uint i = 0; i < filters.size(); ++i)
			{
				std::string extension = (str.substr(str.find_last_of(".") + 1));
				App->fs->NormalizePath(extension);

				if (extension == filters[i])
				{
					pass_filter = true;
					break;
				}
			}
		}
		else
		pass_filter = true;


		// --- If the given file has a compatible extension, try to import it ---
		if (pass_filter)
		{
			std::string path = directory;
			path.append((*it).data());
			Resource* resource = ImportAssets(path.data());

			// --- If resource is imported correctly, add it to the current folder ---
			if (resource)
			{
				folder->AddResource(resource);
			}
		}
	}

	return folder;
}

// --- Identify resource by file extension, call relevant importer, prepare everything for its use ---
Resource* ModuleResources::ImportAssets(const char* path)
{
	// --- Identify resource type by file extension ---
	Resource::ResourceType type = GetResourceTypeFromPath(path);

	Resource* resource = nullptr;

	// --- Call relevant function depending on resource type ---

	switch (type)
	{
	case Resource::ResourceType::FOLDER:
		resource = ImportFolder(path);
		break;

	case Resource::ResourceType::SCENE:
		resource = ImportScene(path);
		break;

	case Resource::ResourceType::MODEL:
		resource = ImportModel(path);
		break;

	case Resource::ResourceType::MATERIAL:
		resource = ImportMaterial(path);
		break;

	case Resource::ResourceType::SHADER:
		resource = ImportShaderProgram(path);
		break;

	case Resource::ResourceType::TEXTURE:
		resource = ImportTexture(path);
		break;

	case Resource::ResourceType::MESH:
		resource = ImportMesh(path);
		break;

	case Resource::ResourceType::SHADER_OBJECT:
		resource = ImportShaderObject(path);
		break;

	//case Resource::ResourceType::META:

	//	break;

	case Resource::ResourceType::UNKNOWN:
		break;
	default:
		CONSOLE_LOG("![Warning]: Detected unsupported file type on: %s", path);
		break;
	}

	if (resource)
	{
		CONSOLE_LOG("Imported successfully: %s", path);
	}
	else
		CONSOLE_LOG("![Warning]: Could not import: %s", path);


	return resource;
}


Resource* ModuleResources::ImportFolder(const char* path)
{
	ImporterFolder* IFolder = GetImporter<ImporterFolder>();
	ImporterMeta* IMeta = GetImporter<ImporterMeta>();

	Resource* folder = nullptr;

	if (IFolder && IMeta)
	{
		// --- Eliminate last / so we can build the meta file name ---
		std::string new_path = path;
		new_path.pop_back();

		// --- If the resource is already in library, load from there ---
		if (IsFileImported(new_path.c_str()))
		{
			// --- Load meta first ---
			ResourceMeta* meta = (ResourceMeta*)IMeta->Load(new_path.c_str());

			if(meta)
			folder = IFolder->Load(meta->GetResourceFile());
		}
		// --- Else call relevant importer ---
		else
		{
			Importer::ImportData IData;
			IData.path = new_path.append("/").c_str();
			folder = IFolder->Import(IData);

			// --- Create meta ---
			ResourceMeta* meta = (ResourceMeta*)CreateResourceGivenUID(Resource::ResourceType::META, new_path, folder->GetUID());

			if (meta)
				IMeta->Save(meta);
		}
	}

	return folder;
}

Resource* ModuleResources::ImportScene(const char* path)
{
	ResourceScene* scene = nullptr;

	// --- If the resource is already in library, load from there ---
	if (IsFileImported(path))
	{
		//Loadfromlib
	}

	// --- Else call relevant importer ---
	else
		// Import

	return scene;
}

Resource* ModuleResources::ImportModel(const char* path)
{
	ImporterModel* IModel = GetImporter<ImporterModel>();
	ImporterMeta* IMeta = GetImporter<ImporterMeta>();

	Resource* model = nullptr;
	std::string file = "";

	App->fs->NormalizePath((char*)path, false);
	App->fs->SplitFilePath(path, nullptr, &file, nullptr);

	if (IModel && IMeta)
	{
		// --- If the resource is already in library, just create the resource with no data ---
		if (IsFileImported(path))
		{
			// --- Load meta first ---
			ResourceMeta* meta = (ResourceMeta*)IMeta->Load(path);
			
			if (meta)
			{
				std::string model_path = MODELS_FOLDER;
				model_path.append(std::to_string(meta->GetUID()));
				model_path.append(".model");
				model = IModel->Load(model_path.c_str());
			}
		}

		// --- Else call relevant importer ---
		else
		{
			// --- Duplicate File into Assets folder ---
			std::string relative_path = ASSETS_FOLDER;
			relative_path.append(file);

			if (!App->fs->Exists(relative_path.c_str()))
				App->fs->CopyFromOutsideFS(path, relative_path.c_str());

			Importer::ImportData IData;
			IData.path = relative_path.c_str();
			model = IModel->Import(IData);

			// --- Create meta ---
			if (model)
			{
				ResourceMeta* meta = (ResourceMeta*)CreateResourceGivenUID(Resource::ResourceType::META, path, model->GetUID());

				if (meta)
					IMeta->Save(meta);
			}
			else
			CONSOLE_LOG("|[error]: Failed to import model: %s", path);
		}
	}

	return model;
}

Resource* ModuleResources::ImportMaterial(const char* path)
{
	ResourceMaterial* material = nullptr;

	// --- If the resource is already in library, load from there ---
	if (IsFileImported(path))
	{
		//Loadfromlib
	}

	// --- Else call relevant importer ---
	else
		// Import

	return material;
}

Resource* ModuleResources::ImportShaderProgram(const char* path)
{
	ResourceShaderProgram* shader = nullptr;

	// --- If the resource is already in library, load from there ---
	if (IsFileImported(path))
	{
		//Loadfromlib
	}

	// --- Else call relevant importer ---
	else
		// Import

	return shader;
}

Resource* ModuleResources::ImportMesh(const char* path)
{
	Resource* mesh = nullptr;
	ImporterMesh* IMesh = GetImporter<ImporterMesh>();

	// --- Load the mesh directly from the lib (only declaration)---
	if (path && IMesh)
	{
		mesh = IMesh->Load(path);
	}


	return mesh;
}

Resource* ModuleResources::ImportTexture(const char* path)
{
	ResourceTexture* texture = nullptr;

	// --- If the resource is already in library, load from there ---
	if (IsFileImported(path))
	{
		//Loadfromlib
	}

	// --- Else call relevant importer ---
	else
		// Import

	return texture;
}

Resource* ModuleResources::ImportShaderObject(const char* path)
{
	ResourceShaderObject* shader_object = nullptr;

	// --- If the resource is already in library, load from there ---
	if (IsFileImported(path))
	{
		//Loadfromlib
	}

	// --- Else call relevant importer ---
	else
		// Import

	return shader_object;
}


// ----------------------------------------------------

Resource* ModuleResources::GetResource(uint UID)
{
	Resource* resource = nullptr;
	Resource::ResourceType type = Resource::ResourceType::UNKNOWN;

	//std::map<uint, ResourceMeta*>::iterator it = metas.find(UID);
	std::map<uint, ResourceFolder*>::iterator it2 = folders.find(UID);
	std::map<uint, ResourceScene*>::iterator it3 = scenes.find(UID);
	std::map<uint, ResourceModel*>::iterator it4 = models.find(UID);
	std::map<uint, ResourceMaterial*>::iterator it5 = materials.find(UID);
	std::map<uint, ResourceShader*>::iterator it6 = shaders.find(UID);
	std::map<uint, ResourceMesh*>::iterator it7 = meshes.find(UID);
	std::map<uint, ResourceTexture*>::iterator it8 = textures.find(UID);
	std::map<uint, ResourceShaderObject*>::iterator it9 = shader_objects.find(UID);

	//if (it != metas.end())
	//	type = GetResourceTypeFromPath((*it).second->GetResourceFile());
	if (it2 != folders.end())
		type = GetResourceTypeFromPath((*it2).second->GetResourceFile());
	else if (it3 != scenes.end())
		type = GetResourceTypeFromPath((*it3).second->GetResourceFile());
	else if (it4 != models.end())
		type = GetResourceTypeFromPath((*it4).second->GetResourceFile());
	else if (it5 != materials.end())
		type = GetResourceTypeFromPath((*it5).second->GetResourceFile());
	else if (it6 != shaders.end())
		type = GetResourceTypeFromPath((*it6).second->GetResourceFile());
	else if (it7 != meshes.end())
		type = GetResourceTypeFromPath((*it7).second->GetResourceFile());
	else if (it8 != textures.end())
		type = GetResourceTypeFromPath((*it8).second->GetResourceFile());
	else if (it9 != shader_objects.end())
		type = GetResourceTypeFromPath((*it9).second->GetResourceFile());

	// --- Call relevant function depending on resource type ---

	switch (type)
	{
	case Resource::ResourceType::FOLDER:
		//resource = ImportFolder(path);
		break;

	case Resource::ResourceType::SCENE:
		//resource = ImportScene(path);
		break;

	case Resource::ResourceType::MODEL:
		resource = models.find(UID)->second;
		resource->LoadToMemory();

		GetImporter<ImporterModel>()->InstanceOnCurrentScene(resource->GetResourceFile());
		break;

	case Resource::ResourceType::MATERIAL:
		//resource = ImportMaterial(path);
		break;

	case Resource::ResourceType::SHADER:
		//resource = ImportShaderProgram(path);
		break;


	case Resource::ResourceType::MESH:
		resource = meshes.find(UID)->second;
		resource->LoadToMemory();
		break;

	case Resource::ResourceType::TEXTURE:
		resource = textures.find(UID)->second;
		resource->LoadToMemory();
		break;

	case Resource::ResourceType::SHADER_OBJECT:
		//resource = ImportShaderObject(path);
		break;

		//case Resource::ResourceType::META:

		//	break;

	case Resource::ResourceType::UNKNOWN:
		break;

	default:
		CONSOLE_LOG("![Warning]: Detected unsupported file type on: %i", UID);
		break;
	}

	if(!resource)
		CONSOLE_LOG("![Warning]: Could not load: %i", UID);


	return resource;
}

// ------------------- RESOURCE HANDLING ----------------------------------------------------------

Resource * ModuleResources::CreateResource(Resource::ResourceType type, std::string source_file)
{
	// Note you CANNOT create a meta resource through this function, use CreateResourceGivenUID instead

	Resource* resource = nullptr;

	switch (type)
	{
	case Resource::ResourceType::FOLDER:
		resource = (Resource*)new ResourceFolder(App->GetRandom().Int(),source_file);
		folders[resource->GetUID()] = (ResourceFolder*)resource;
		break;

	case Resource::ResourceType::SCENE:
		resource = (Resource*)new ResourceScene(App->GetRandom().Int(), source_file);
		scenes[resource->GetUID()] = (ResourceScene*)resource;
		break;

	case Resource::ResourceType::MODEL:
		resource = (Resource*)new ResourceModel(App->GetRandom().Int(), source_file);
		models[resource->GetUID()] = (ResourceModel*)resource;
		break;

	case Resource::ResourceType::MATERIAL:
		resource = (Resource*)new ResourceMaterial(App->GetRandom().Int(), source_file);
		materials[resource->GetUID()] = (ResourceMaterial*)resource;
		break;

	case Resource::ResourceType::SHADER:
		resource = (Resource*)new ResourceShader(App->GetRandom().Int(), source_file);
		shaders[resource->GetUID()] = (ResourceShader*)resource;
		break;

	case Resource::ResourceType::MESH:
		resource = (Resource*)new ResourceMesh(App->GetRandom().Int(), source_file);
		meshes[resource->GetUID()] = (ResourceMesh*)resource;
		break;

	case Resource::ResourceType::TEXTURE:
		resource = (Resource*)new ResourceTexture(App->GetRandom().Int(), source_file);
		textures[resource->GetUID()] = (ResourceTexture*)resource;
		break;

	case Resource::ResourceType::SHADER_OBJECT:
		resource = (Resource*)new ResourceShaderObject(App->GetRandom().Int(), source_file);
		shader_objects[resource->GetUID()] = (ResourceShaderObject*)resource;
		break;

	case Resource::ResourceType::UNKNOWN:
		CONSOLE_LOG("![Warning]: Detected unsupported resource type");
		break;

	default:
		CONSOLE_LOG("![Warning]: Detected unsupported resource type");
		break;
	}

	return resource;
}

Resource* ModuleResources::CreateResourceGivenUID(Resource::ResourceType type, std::string source_file, uint UID)
{
	Resource* resource = nullptr;

	switch (type)
	{
	case Resource::ResourceType::FOLDER:
		resource = (Resource*)new ResourceFolder(UID, source_file);
		folders[resource->GetUID()] = (ResourceFolder*)resource;
		break;

	case Resource::ResourceType::SCENE:
		resource = (Resource*)new ResourceScene(UID, source_file);
		scenes[resource->GetUID()] = (ResourceScene*)resource;
		break;

	case Resource::ResourceType::MODEL:
		resource = (Resource*)new ResourceModel(UID, source_file);
		models[resource->GetUID()] = (ResourceModel*)resource;
		break;

	case Resource::ResourceType::MATERIAL:
		resource = (Resource*)new ResourceMaterial(UID, source_file);
		materials[resource->GetUID()] = (ResourceMaterial*)resource;
		break;

	case Resource::ResourceType::SHADER:
		resource = (Resource*)new ResourceShader(UID, source_file);
		shaders[resource->GetUID()] = (ResourceShader*)resource;
		break;

	case Resource::ResourceType::MESH:
		resource = (Resource*)new ResourceMesh(UID, source_file);
		meshes[resource->GetUID()] = (ResourceMesh*)resource;
		break;

	case Resource::ResourceType::TEXTURE:
		resource = (Resource*)new ResourceTexture(UID, source_file);
		textures[resource->GetUID()] = (ResourceTexture*)resource;
		break;

	case Resource::ResourceType::SHADER_OBJECT:
		resource = (Resource*)new ResourceShaderObject(UID, source_file);
		shader_objects[resource->GetUID()] = (ResourceShaderObject*)resource;
		break;

	case Resource::ResourceType::META:
		resource = (Resource*)new ResourceMeta(UID, source_file);
		metas[resource->GetUID()] = (ResourceMeta*)resource;
		break;

	case Resource::ResourceType::UNKNOWN:
		CONSOLE_LOG("![Warning]: Detected unsupported resource type");
		break;

	default:
		CONSOLE_LOG("![Warning]: Detected unsupported resource type");
		break;
	}

	return resource;
}


Resource::ResourceType ModuleResources::GetResourceTypeFromPath(const char* path)
{
	static_assert(static_cast<int>(Resource::ResourceType::UNKNOWN) == 9, "Resource Creation Switch needs to be updated");

	std::string extension = "";
	App->fs->SplitFilePath(path, nullptr, nullptr, &extension);
	App->fs->NormalizePath(extension, true);


	Resource::ResourceType type = Resource::ResourceType::UNKNOWN;

	if (extension == "")
		type = Resource::ResourceType::FOLDER;

	else if (extension == "scene")
		type = Resource::ResourceType::SCENE;

	else if (extension == "fbx" || extension == "model")
		type = Resource::ResourceType::MODEL;

	else if (extension == "mat")
		type = Resource::ResourceType::MATERIAL;

	else if (extension == "shader")
		type = Resource::ResourceType::SHADER;

	else if (extension == "dds" || extension == "png" || extension == "jpg")
		type = Resource::ResourceType::TEXTURE;

	else if (extension == "mesh")
		type = Resource::ResourceType::MESH;

	else if (extension == "vertex" || extension == "fragment")
		type = Resource::ResourceType::SHADER_OBJECT;

	else if (extension == "meta")
		type = Resource::ResourceType::META;


	return type;
}

ResourceFolder* ModuleResources::GetAssetsFolder()
{
	return AssetsFolder;
}

const std::map<uint, ResourceFolder*>& ModuleResources::GetAllFolders() const
{
	return folders;
}

bool ModuleResources::IsFileImported(const char* file)
{
	bool ret = false;

	std::string path = file;

	path.append(".meta");

	// --- PhysFS only will return true if the file is inside one of the fs predefined folders! 
	//  using that on our advantage to know if a resource is imported or not ---
	ret = App->fs->Exists(path.data());

	return ret;
}

void ModuleResources::ONResourceDestroyed(Resource* resource)
{
	static_assert(static_cast<int>(Resource::ResourceType::UNKNOWN) == 9, "Resource Destruction Switch needs to be updated");

	switch (resource->GetType())
	{
	case Resource::ResourceType::FOLDER:
		folders.erase(resource->GetUID());
		break;

	case Resource::ResourceType::SCENE:
		scenes.erase(resource->GetUID());
		break;

	case Resource::ResourceType::MODEL:
		models.erase(resource->GetUID());
		break;

	case Resource::ResourceType::MATERIAL:
		materials.erase(resource->GetUID());
		break;

	case Resource::ResourceType::SHADER:
		shaders.erase(resource->GetUID());
		break;

	case Resource::ResourceType::MESH:
		meshes.erase(resource->GetUID());
		break;

	case Resource::ResourceType::TEXTURE:
		textures.erase(resource->GetUID());
		break;

	case Resource::ResourceType::SHADER_OBJECT:
		shader_objects.erase(resource->GetUID());
		break;

	case Resource::ResourceType::META:
		metas.erase(resource->GetUID());
		break;

	case Resource::ResourceType::UNKNOWN:
		CONSOLE_LOG("![Warning]: Detected unsupported resource type");
		break;

	default:
		CONSOLE_LOG("![Warning]: Detected unsupported resource type");
		break;
	}

}

// ----------------------------------------------------

update_status ModuleResources::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	static_assert(static_cast<int>(Resource::ResourceType::UNKNOWN) == 9, "Resource Clean Up needs to be updated");

	// Since std map invalidates iterator on pair delete, a reverse iterator is very useful
	// We eliminate a pair and automatically our iterator points at a new element

	// --- Delete resources ---
	for (std::map<uint, ResourceFolder*>::reverse_iterator it = folders.rbegin(); it != folders.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	folders.clear();

	for (std::map<uint, ResourceScene*>::reverse_iterator it = scenes.rbegin(); it != scenes.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	scenes.clear();

	for (std::map<uint, ResourceModel*>::reverse_iterator it = models.rbegin(); it != models.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	models.clear();

	for (std::map<uint, ResourceMaterial*>::reverse_iterator it = materials.rbegin(); it != materials.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	materials.clear();

	for (std::map<uint, ResourceShader*>::reverse_iterator it = shaders.rbegin(); it != shaders.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	shaders.clear();

	for (std::map<uint, ResourceMesh*>::reverse_iterator it = meshes.rbegin(); it != meshes.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	meshes.clear();

	for (std::map<uint, ResourceTexture*>::reverse_iterator it = textures.rbegin(); it != textures.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	textures.clear();

	for (std::map<uint, ResourceShaderObject*>::reverse_iterator it = shader_objects.rbegin(); it != shader_objects.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	shader_objects.clear();

	for (std::map<uint, ResourceMeta*>::reverse_iterator it = metas.rbegin(); it != metas.rend();)
	{
		it->second->FreeMemory();
		delete it->second;
	}

	metas.clear();

	// --- Delete importers ---
	for (uint i = 0; i < importers.size(); ++i)
	{
		delete importers[i];
	}

	importers.clear();

	// --- Detach assimp log stream ---
	aiDetachAllLogStreams();

	return true;
}


//
//Resource * ModuleResources::GetResource(const char * original_file)
//{
//	// --- If resource is loaded into memory, return pointer to it, else load it ---
//
//	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
//	{
//		std::string tmp = it->second->GetOriginalFile();
//		if (tmp.compare(original_file) == 0)
//			return it->second;
//	}
//
//	return nullptr;
//}
//
//

//
//uint ModuleResources::GetModDateFromMeta(const char * file)
//{
//	std::string path = file;
//	path.append(".meta");
//	uint DATE = 0;
//
//	if (App->fs->Exists(path.data()))
//	{
//		json file = App->GetJLoader()->Load(path.data());
//		std::string date = file["DATE"];
//		DATE = std::stoi(date);
//	}
//
//	return DATE;
//}
//
//std::map<std::string, ResourceShader*>* ModuleResources::GetShaders()
//{
//	return &shaders;
//}
//
//void ModuleResources::SaveAllShaders()
//{
//	for (std::map<std::string, ResourceShader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
//	{
//		if(it->first != "Standard" && it->first != "LinePoint" && it->first != "ZDrawer" && it->first != "OutlineShader")
//		it->second->Save();
//	}
//}
//
