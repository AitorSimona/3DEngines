#include "ImporterScene.h"
#include "Application.h"
#include "ModuleResourceManager.h"


#include "mmgr/mmgr.h"

ImporterScene::ImporterScene() : Importer(Importer::ImporterType::Scene)
{

}

ImporterScene::~ImporterScene()
{

}

// MYTODO: Give some use to return type (bool) in all functions (if load fails log...)

Resource* ImporterScene::Import(ImportData& IData) const
{
	//// --- Import scene from path ---
	//const aiScene* scene = aiImportFileEx(relative_path.data(), aiProcessPreset_TargetRealtime_MaxQuality | aiPostProcessSteps::aiProcess_FlipUVs, App->fs->GetAssimpIO());

	//GameObject* rootnode = nullptr;

	//if (scene != nullptr && scene->HasMeshes())
	//{
	//	rootnode = App->scene_manager->CreateEmptyGameObject();
	//	// --- Set root node name as file name with no extension ---
	//	rootnode->SetName(rootnodename.data());

	//	// --- Save Game objects to vector so we can save to lib later ---
	//	std::vector<GameObject*> scene_gos;
	//	scene_gos.push_back(rootnode);

	//	// --- Load all meshes ---
	//	LoadSceneMeshes(scene);

	//	// --- Use scene->mNumMeshes to iterate on scene->mMeshes array ---
	//	LoadNodes(scene->mRootNode,rootnode,scene, scene_gos, relative_path.data(), File_path);

	//	// --- Save to Own format files in Library ---
	//	std::string exported_file = SaveSceneToFile(scene_gos, rootnodename, MODEL);

	//	// --- Free everything ---
	//	FreeSceneMeshes();
	//	rootnode->RecursiveDelete();

	//	// --- Free scene ---
	//	aiReleaseImport(scene);

	//}
	//else
	//	CONSOLE_LOG("|[error]: Error loading FBX %s", &File_path);

	return nullptr;
}

Resource* ImporterScene::Load(const char * path) const
{
	// --- Load Scene/model file ---
	json file = App->GetJLoader()->Load(path);


	//std::vector<GameObject*> objects;

	//for (json::iterator it = file.begin(); it != file.end(); ++it)
	//{
	//	// --- Create a Game Object for each node ---
	//	GameObject* new_go = App->scene_manager->CreateEmptyGameObject();

	//	// --- Retrieve GO's UID and name ---
	//	new_go->SetName(it.key().data());
	//	std::string uid = file[it.key()]["UID"];
	//	new_go->GetUID() = std::stoi(uid);

	//	// --- Iterate components ---
	//	json components = file[it.key()]["Components"];

	//	ComponentTransform* transform = transform = new_go->GetComponent<ComponentTransform>(Component::ComponentType::Transform);

	//	std::string posx = components["0"]["positionx"];
	//	std::string posy = components["0"]["positiony"];
	//	std::string posz = components["0"]["positionz"];

	//	std::string rotx = components["0"]["rotationx"];
	//	std::string roty = components["0"]["rotationy"];
	//	std::string rotz = components["0"]["rotationz"];

	//	std::string scalex = components["0"]["scalex"];
	//	std::string scaley = components["0"]["scaley"];
	//	std::string scalez = components["0"]["scalez"];

	//	for (json::iterator it2 = components.begin(); it2 != components.end(); ++it2)
	//	{
	//		// --- Determine ComponentType ---
	//		std::string val = it2.key();
	//		uint value = std::stoi(val);		
	//		Component::ComponentType type = (Component::ComponentType)value;

	//		// --- Create components to fill ---
	//		ComponentMesh* mesh = nullptr;
	//		ComponentMaterial* mat = nullptr;
	//		ComponentCamera* camera = nullptr;
	//		ResourceTexture* texture = nullptr;
	//		ResourceMesh* rmesh = nullptr;
	//		std::string tmp;

	//		// --- Get path to component file ---
	//		std::string component_path;

	//		if (type != Component::ComponentType::Transform && type != Component::ComponentType::Material)
	//		{
	//			std::string tmppath = components[val];
	//			component_path = tmppath;
	//		}

	//		std::string diffuse_uid;
	//		std::string mesh_uid;
	//		uint count;

	//		switch (type)
	//		{
	//			case Component::ComponentType::Transform:
	//				transform->SetPosition(std::stof(posx),std::stof(posy), std::stof(posz));
	//				transform->SetRotation(float3{std::stof(rotx), std::stof(roty), std::stof(rotz) });
	//				transform->Scale(std::stof(scalex), std::stof(scaley), std::stof(scalez));
	//				break;

	//			case Component::ComponentType::Renderer:
	//				new_go->AddComponent(Component::ComponentType::Renderer);
	//				break;

	//			case Component::ComponentType::Material:
	//				// --- Check if Library file exists ---
	//			    {
	//					std::string diffuse = components[val]["diffuse"];
	//					component_path = diffuse;
	//				}
	//				if (App->fs->Exists(component_path.data()))
	//				{

	//					mat = App->scene_manager->CreateEmptyMaterial();
	//					mat->resource_material = (ResourceMaterial*)App->resources->CreateResource(Resource::ResourceType::MATERIAL);
	//					texture = (ResourceTexture*) App->resources->GetResource(component_path.data());

	//					if (texture)
	//					{
	//						mat->resource_material->resource_diffuse = texture;
	//						texture->instances++;
	//					}
	//					else
	//					{
	//						IMaterial->Load(component_path.data(), *mat->resource_material);

	//						diffuse_uid = component_path;
	//						App->fs->SplitFilePath(component_path.data(), nullptr, &diffuse_uid);
	//						count = diffuse_uid.find_last_of(".");
	//						diffuse_uid = diffuse_uid.substr(0, count);
	//						mat->resource_material->resource_diffuse->SetUID(std::stoi(diffuse_uid));
	//					}
	//					std::string tmp = components[val]["shader"];
	//					std::map<std::string, ResourceShader*>::iterator it = App->resources->GetShaders()->find(tmp);

	//					ResourceShader* shad = nullptr;

	//					if(it != App->resources->GetShaders()->end())
	//					shad = it->second;

	//					if (shad)
	//					{
	//						mat->resource_material->shader = shad;


	//						// --- Load uniforms ---
	//						std::vector<Uniform*> uniforms;
	//						shad->GetAllUniforms(uniforms);

	//						uint shader = shad->ID;

	//						float tmp_float, tmp_float2, tmp_float3, tmp_float4 = 0.0f;
	//						int tmp_int, tmp_int2, tmp_int3, tmp_int4 = 0;
	//						std::string tmp;

	//						for (std::vector<Uniform*>::const_iterator iterator = uniforms.begin(); iterator != uniforms.end(); ++iterator)
	//						{

	//							switch ((*iterator)->type)
	//							{
	//							case GL_INT:
	//								{
	//								std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//								tmp = tmp1;
	//								}
	//								tmp_int = std::stoi(tmp);
	//								(*iterator)->value.intU = tmp_int;
	//								break;

	//							case GL_FLOAT:
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//									tmp = tmp1;
	//								}
	//								tmp_float = std::stof(tmp);
	//								(*iterator)->value.floatU = tmp_float;
	//								break;

	//							case GL_FLOAT_VEC2:
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//									tmp = tmp1;
	//								}
	//								tmp_float = std::stof(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["y"];
	//									tmp = tmp1;
	//								}
	//								tmp_float2 = std::stof(tmp);
	//								(*iterator)->value.vec2U = float2(tmp_float,tmp_float2);
	//								break;

	//							case GL_FLOAT_VEC3:
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//									tmp = tmp1;
	//								}
	//								tmp_float = std::stof(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["y"];
	//									tmp = tmp1;
	//								}
	//								tmp_float2 = std::stof(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["z"];
	//									tmp = tmp1;
	//								}
	//								tmp_float3 = std::stof(tmp);

	//								(*iterator)->value.vec3U = float3(tmp_float, tmp_float2, tmp_float3);
	//								break;

	//							case GL_FLOAT_VEC4:
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//									tmp = tmp1;
	//								}
	//								tmp_float = std::stof(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["y"];
	//									tmp = tmp1;
	//								}
	//								tmp_float2 = std::stof(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["z"];
	//									tmp = tmp1;
	//								}
	//								tmp_float3 = std::stof(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["w"];
	//									tmp = tmp1;
	//								}
	//								tmp_float4 = std::stof(tmp);

	//								(*iterator)->value.vec4U = float4(tmp_float, tmp_float2, tmp_float3, tmp_float4);
	//								break;

	//							case GL_INT_VEC2:
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//									tmp = tmp1;
	//								}
	//								tmp_int = std::stoi(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["y"];
	//									tmp = tmp1;
	//								}
	//								tmp_int2 = std::stoi(tmp);

	//								(*iterator)->value.vec2U = float2(tmp_int, tmp_int2);
	//								break;

	//							case GL_INT_VEC3:
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//									tmp = tmp1;
	//								}
	//								tmp_int = std::stoi(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["y"];
	//									tmp = tmp1;
	//								}
	//								tmp_int2 = std::stoi(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["z"];
	//									tmp = tmp1;
	//								}
	//								tmp_int3 = std::stoi(tmp);

	//								(*iterator)->value.vec3U = float3(tmp_int, tmp_int2, tmp_int3);
	//								break;

	//							case GL_INT_VEC4:
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["x"];
	//									tmp = tmp1;
	//								}
	//								tmp_int = std::stoi(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["y"];
	//									tmp = tmp1;
	//								}
	//								tmp_int2 = std::stoi(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["z"];
	//									tmp = tmp1;
	//								}
	//								tmp_int3 = std::stoi(tmp);
	//								{
	//									std::string tmp1 = components[val]["uniforms"][(*iterator)->name]["w"];
	//									tmp = tmp1;
	//								}
	//								tmp_int4 = std::stoi(tmp);

	//								(*iterator)->value.vec4U = float4(tmp_int, tmp_int2, tmp_int3, tmp_int4);
	//								break;

	//							default:
	//								continue;
	//								break;

	//							}

	//						}

	//						mat->resource_material->uniforms = uniforms;
	//					}

	//					new_go->SetMaterial(mat);
	//				}
	//				else
	//					CONSOLE_LOG("|[error]: Could not find %s", component_path.data());

	//				break;

	//			case Component::ComponentType::Mesh:

	//				// --- Check if Library file exists ---
	//				if (App->fs->Exists(component_path.data()))
	//				{
	//					rmesh = (ResourceMesh*)App->resources->GetResource(component_path.data());
	//					mesh = (ComponentMesh*)new_go->AddComponent(type);
	//					if (rmesh)
	//					{
	//						mesh->resource_mesh = rmesh;
	//						rmesh->instances++;
	//					}
	//					else
	//					{
	//						mesh_uid = component_path;
	//						App->fs->SplitFilePath(component_path.data(), nullptr, &mesh_uid);
	//						count = mesh_uid.find_last_of(".");
	//						mesh_uid = mesh_uid.substr(0, count);

	//						mesh->resource_mesh = (ResourceMesh*)App->resources->CreateResource(Resource::ResourceType::MESH);
	//						IMesh->Load(component_path.data(), *mesh->resource_mesh);
	//						mesh->resource_mesh->SetUID(std::stoi(mesh_uid));
	//					}
	//				}
	//				else
	//					CONSOLE_LOG("|[error]: Could not find %s", component_path.data());
	//				break;

	//			case Component::ComponentType::Camera:
	//				//camera->SetFOV();
	//				//camera->SetNearPlane();
	//				//camera->SetFarPlane();
	//				//camera->SetAspectRatio();
	//				break;

	//		}
	//	}

	//	//transform->update_transform = true;
	//	objects.push_back(new_go);
	//}

	//// --- Parent GO's ---
	//for (uint i = 0; i < objects.size(); ++i)
	//{
	//	std::string parent_uid = file[objects[i]->GetName()]["Parent"];
	//	uint p_uid = std::stoi(parent_uid);

	//	for (uint j = 0; j < objects.size(); ++j)
	//	{
	//		if (p_uid == objects[j]->GetUID())
	//		{
	//			objects[j]->AddChildGO(objects[i]);
	//			continue;
	//		}
	//	}
	//}

	//App->scene_manager->GetRootGO()->OnUpdateTransform();

	return nullptr;
}

std::string ImporterScene::SaveSceneToFile(std::vector<GameObject*>& scene_gos, std::string& scene_name) const
{
	// --- Save Scene/Model to file ---

	json file;

	//for (int i = 0; i < scene_gos.size(); ++i)
	//{
	//	// --- Create GO Structure ---
	//	file[scene_gos[i]->GetName()];
	//	file[scene_gos[i]->GetName()]["UID"] = std::to_string(scene_gos[i]->GetUID());
	//	file[scene_gos[i]->GetName()]["Parent"] = std::to_string(scene_gos[i]->parent->GetUID());
	//	file[scene_gos[i]->GetName()]["Components"];

	//	for (int j = 0; j < scene_gos[i]->GetComponents().size(); ++j)
	//	{

	//		std::string component_path;
	//		ComponentTransform* transform = transform = scene_gos[i]->GetComponent<ComponentTransform>(Component::ComponentType::Transform);
	//		float3 position = transform->GetPosition();
	//		float3 rotation = transform->GetRotation();
	//		float3 scale = transform->GetScale();

	//		ComponentMesh* mesh = scene_gos[i]->GetComponent<ComponentMesh>(Component::ComponentType::Mesh);

	//		ComponentCamera* camera = scene_gos[i]->GetComponent<ComponentCamera>(Component::ComponentType::Camera);

	//		// --- Save Components to files ---

	//		switch (scene_gos[i]->GetComponents()[j]->GetType())
	//		{

	//			case Component::ComponentType::Transform:
	//				// --- Store path to component file ---
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())];

	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["positionx"] = std::to_string(position.x);
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["positiony"] = std::to_string(position.y);
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["positionz"] = std::to_string(position.z);

	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["rotationx"] = std::to_string(rotation.x);
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["rotationy"] = std::to_string(rotation.y);
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["rotationz"] = std::to_string(rotation.z);

	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["scalex"] = std::to_string(scale.x);
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["scaley"] = std::to_string(scale.y);
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["scalez"] = std::to_string(scale.z);
	//				
	//				break;
	//			case Component::ComponentType::Mesh:
	//				component_path = MESHES_FOLDER;
	//				component_path.append(std::to_string(mesh->resource_mesh->GetUID()));
	//				component_path.append(".mesh");

	//				if(!App->fs->Exists(component_path.data()))
	//				IMesh->Save(scene_gos[i]->GetComponent<ComponentMesh>(Component::ComponentType::Mesh)->resource_mesh, component_path.data());

	//				// --- Store path to component file ---
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())] = component_path;
	//				break;
	//			case Component::ComponentType::Renderer:
	//				// --- Store path to component file ---
	//				file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())] = component_path;
	//				break;

	//			case Component::ComponentType::Material:
	//				if (scene_gos[i]->GetComponent<ComponentMaterial>(Component::ComponentType::Material)->resource_material->resource_diffuse)
	//				{
	//					component_path = TEXTURES_FOLDER;
	//					component_path.append(std::to_string(scene_gos[i]->GetComponent<ComponentMaterial>(Component::ComponentType::Material)->resource_material->resource_diffuse->GetUID()));
	//					component_path.append(".dds");

	//					// --- Store path to component file ---
	//					file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())];
	//					file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["diffuse"] = component_path;
	//					component_path = ((scene_gos[i]->GetComponent<ComponentMaterial>(Component::ComponentType::Material)->resource_material->shader->name));
	//					file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["shader"] = component_path;
	//					file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"];

	//					std::vector<Uniform*>* uniforms = &((scene_gos[i]->GetComponent<ComponentMaterial>(Component::ComponentType::Material)->resource_material->uniforms));
	//					uint shader = ((scene_gos[i]->GetComponent<ComponentMaterial>(Component::ComponentType::Material)->resource_material->shader->ID));

	//					float* tmpf = new float[4];
	//					int* tmpi = new int[4];


	//					for (std::vector<Uniform*>::const_iterator iterator = uniforms->begin(); iterator != uniforms->end(); ++iterator)
	//					{
	//						file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name];
	//						file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["type"] = std::to_string((*iterator)->type);

	//						switch ((*iterator)->type)
	//						{
	//						case GL_INT:				
	//							glGetUniformiv(shader, (*iterator)->location, tmpi);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpi[0]);
	//							break;

	//						case GL_FLOAT:
	//							glGetUniformfv(shader, (*iterator)->location, tmpf);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpf[0]);
	//							break;

	//						case GL_FLOAT_VEC2:
	//							glGetUniformfv(shader, (*iterator)->location, tmpf);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpf[0]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["y"] = std::to_string(tmpf[1]);
	//							break;

	//						case GL_FLOAT_VEC3:
	//							glGetUniformfv(shader, (*iterator)->location, tmpf);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpf[0]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["y"] = std::to_string(tmpf[1]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["z"] = std::to_string(tmpf[2]);
	//							break;

	//						case GL_FLOAT_VEC4:
	//							glGetUniformfv(shader, (*iterator)->location, tmpf);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpf[0]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["y"] = std::to_string(tmpf[1]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["z"] = std::to_string(tmpf[2]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["w"] = std::to_string(tmpf[3]);
	//							break;

	//						case GL_INT_VEC2:
	//							glGetUniformiv(shader, (*iterator)->location, tmpi);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpi[0]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["y"] = std::to_string(tmpi[1]);
	//							break;

	//						case GL_INT_VEC3:
	//							glGetUniformiv(shader, (*iterator)->location, tmpi);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpi[0]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["y"] = std::to_string(tmpi[1]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["z"] = std::to_string(tmpi[2]);
	//							break;

	//						case GL_INT_VEC4:
	//							glGetUniformiv(shader, (*iterator)->location, tmpi);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["x"] = std::to_string(tmpi[0]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["y"] = std::to_string(tmpi[1]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["z"] = std::to_string(tmpi[2]);
	//							file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["uniforms"][(*iterator)->name]["w"] = std::to_string(tmpi[3]);
	//							break;

	//						default:
	//							continue;
	//							break;

	//						}

	//					}

	//					delete[] tmpf;
	//					delete[] tmpi;

	//				}
	//				break;

	//			case Component::ComponentType::Camera:
	//				//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["FOV"] = std::to_string(camera->GetFOV());
	//				//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["NEARPLANE"] = std::to_string(camera->GetNearPlane());
	//				//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["FARPLANE"] = std::to_string(camera->GetFarPlane());
	//				//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["ASPECTRATIO"] = std::to_string(camera->GetAspectRatio());
	//				break;
	//			
	//		}


	//	}
	//}
	//// --- Serialize JSON to string ---
	//std::string data;
	//data = App->GetJLoader()->Serialize(file);

	//// --- Set destination file given exportfile type ---
	//std::string path;
	//uint new_uid;
	//std::string filename = ASSETS_FOLDER;
	//filename.append(scene_name);
	//switch (exportedfile_type)
	//{
	//	case MODEL:
	//		path = MODELS_FOLDER;
	//		new_uid = App->GetRandom().Int();
	//		path.append(std::to_string(new_uid));
	//		path.append(".model");	
	//		filename.append(".fbx");
	//		App->resources->CreateMetaFromUID(new_uid, filename.data());
	//		break;

	//	case SCENE:
	//		path = SCENES_FOLDER;
	//		path.append(scene_name);
	//		path.append(".scene");
	//		break;
	//}

	//// --- Finally Save to file ---
	//char* buffer = (char*)data.data();
	//uint size = data.length();

	//App->fs->Save(path.data(), buffer, size);

	return scene_name;
}

