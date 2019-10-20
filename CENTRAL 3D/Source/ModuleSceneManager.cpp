#include "ModuleSceneManager.h"
#include "OpenGL.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentRenderer.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ModuleTextures.h"

#include "ImporterMaterial.h"

#include "par/par_shapes.h"

#include "mmgr/mmgr.h"

ModuleSceneManager::ModuleSceneManager(bool start_enabled)
{
}

ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Init(json file)
{


	return true;
}

bool ModuleSceneManager::Start()
{
	CheckersMaterial = CreateEmptyMaterial();
	CheckersMaterial->TextureID = App->textures->GetCheckerTextureID();


	GameObject* cube = CreateCube(1.0f, 1.0f, 1.0f, true);
	GameObject* sphere = CreateSphere(1.0f,25,25, true);

	cube->SetPosition(-3.0f, 0.0f, 0.0f);
	sphere->SetPosition(-4.5f, 0.5f, 0.5f);

	return true;
}

update_status ModuleSceneManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{
	// --- Clean Up object data ---
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i])
			delete game_objects[i];
	}

	for (uint i = 0; i < Materials.size(); ++i)
	{
		if (Materials[i])
			delete Materials[i];
	}

	CheckersMaterial = nullptr;

	return true;
}


void ModuleSceneManager::Draw() const
{

	// --- Draw Grid ---
	CreateGrid();

	// --- Activate wireframe mode ---
	if (App->renderer3D->wireframe)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// --- Draw Game Object Meshes ---
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		glPushMatrix();
		glMultMatrixf(game_objects[i]->GetLocalTransform().ptr());

		// --- Search for Renderer Component --- 
		ComponentRenderer* Renderer = (ComponentRenderer*)game_objects[i]->GetComponent(Component::ComponentType::Renderer);

		// --- If Found, draw the mesh ---
		if (Renderer)
		{
			Renderer->Draw();
		}

		glPopMatrix();
	}

	// --- DeActivate wireframe mode ---
	if (App->renderer3D->wireframe)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

}

uint ModuleSceneManager::GetNumGameObjects() const
{
	return game_objects.size();
}

uint ModuleSceneManager::GetSelectedGameObjects() 
{
	return SelectedGameObject;
}

std::vector<GameObject*>& ModuleSceneManager::GetGameObjects() 
{
	return game_objects;
}

void ModuleSceneManager::SetSelectedGameObject(uint index)
{
	SelectedGameObject = index;
}

void ModuleSceneManager::SetTextureToSelectedGO(uint id)
{
	ComponentMaterial* Material = (ComponentMaterial*)game_objects[SelectedGameObject]->GetComponent(Component::ComponentType::Material);

	if (Material)
	{
		//Material->FreeCurrentTexture();
		Material->TextureID = id;
	}
}

GameObject * ModuleSceneManager::CreateEmptyGameObject()
{
	// --- Create New Game Object Name ---
	std::string Name = "GameObject ";
	Name.append("(");
	Name.append(std::to_string(game_objects.size()));
	Name.append(")");

	// --- Create empty Game object to be filled out ---
	GameObject* new_object = new GameObject(Name.data());
	game_objects.push_back(new_object);

	return new_object;
}

ComponentMaterial * ModuleSceneManager::CreateEmptyMaterial()
{
	// --- Creating Empty material to be filled out ---
	ComponentMaterial* Material = new ComponentMaterial(Component::ComponentType::Material);

	Materials.push_back(Material);

	return Material;
}

GameObject * ModuleSceneManager::CreateCube(float sizeX, float sizeY, float sizeZ, bool checkers)
{

	par_shapes_mesh * mesh = par_shapes_create_cube();
	GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

	if (mesh)
	{
		ComponentMesh* new_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);

		ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

		new_mesh->IndexDatatype = GL_UNSIGNED_SHORT;

		par_shapes_scale(mesh, sizeX, sizeY, sizeZ);

		new_mesh->IndicesSize = mesh->ntriangles * 3;
		new_mesh->VerticesSize = mesh->npoints;

		// --- Vertices ---

		new_mesh->Vertices = new float3[new_mesh->VerticesSize];

		for (uint i = 0; i < new_mesh->VerticesSize; ++i)
		{
			new_mesh->Vertices[i].x = mesh->points[3 * i];
			new_mesh->Vertices[i].y = mesh->points[(3 * i) + 1];
			new_mesh->Vertices[i].z = mesh->points[(3 * i) + 2];
		}

		glGenBuffers(1, (GLuint*)&new_mesh->VerticesID); // create buffer
		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->VerticesID); // start using created buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * new_mesh->VerticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

		// --- Indices ---

		new_mesh->Indices = new unsigned[new_mesh->IndicesSize];
		for (uint i = 0; i < uint(mesh->ntriangles) * 3; ++i)
		{
			new_mesh->Indices[i] = mesh->triangles[i];
		}

		glGenBuffers(1, (GLuint*)&new_mesh->IndicesID); // create buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->IndicesID); // start using created buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * new_mesh->IndicesSize, mesh->triangles, GL_STATIC_DRAW); // send vertices to VRAM
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

		// --- Texture Coords ---
	
		new_mesh->TexCoordsSize = new_mesh->VerticesSize*2;
	
		new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{
			    1, 0, 	0, 0, 	0, 1, 	1, 1,               // v0,v1,v2,v3 (front)
				1, 1,   1, 0,   0, 0,   0, 1,               // v4,v7,v6,v5 (back)
		};
	
		if (checkers)
			new_object->SetMaterial(CheckersMaterial);

		glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer


		par_shapes_free_mesh(mesh);

	}

	return new_object;
}

GameObject * ModuleSceneManager::CreateSphere(float Radius, int slices, int slacks, bool checkers)
{
	par_shapes_mesh * mesh = par_shapes_create_parametric_sphere(slices, slacks);

	GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

	if (mesh)
	{
		ComponentMesh* new_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);

		ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

		new_mesh->IndexDatatype = GL_UNSIGNED_SHORT;

		par_shapes_scale(mesh, Radius / 2, Radius / 2, Radius / 2);

		new_mesh->IndicesSize = mesh->ntriangles * 3;
		new_mesh->VerticesSize = mesh->npoints;

		// --- Vertices ---

		new_mesh->Vertices = new float3[new_mesh->VerticesSize];

		for (uint i = 0; i < new_mesh->VerticesSize; ++i)
		{
			new_mesh->Vertices[i].x = mesh->points[3*i];
			new_mesh->Vertices[i].y = mesh->points[(3*i) + 1];
			new_mesh->Vertices[i].z = mesh->points[(3*i) + 2];
		}

		glGenBuffers(1, (GLuint*)&new_mesh->VerticesID); // create buffer
		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->VerticesID); // start using created buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * new_mesh->VerticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

		// --- Indices ---

		new_mesh->Indices = new unsigned[new_mesh->IndicesSize];
		for (uint i = 0; i < uint(mesh->ntriangles) * 3; ++i)
		{
			new_mesh->Indices[i] = mesh->triangles[i];
		}

		glGenBuffers(1, (GLuint*)&new_mesh->IndicesID); // create buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->IndicesID); // start using created buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * new_mesh->IndicesSize, mesh->triangles, GL_STATIC_DRAW); // send vertices to VRAM
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

		// --- Texture Coords ---

		if (checkers)
			new_object->SetMaterial(CheckersMaterial);

		new_mesh->TexCoordsSize = new_mesh->VerticesSize * 2;
		new_mesh->TexCoords = new float[new_mesh->TexCoordsSize];

		for (uint i = 0; i < new_mesh->VerticesSize; ++i)
		{
			new_mesh->TexCoords[2 * i] = mesh->tcoords[2 * i];
			new_mesh->TexCoords[(2 * i) + 1] = mesh->tcoords[(2 * i) + 1];
		}

		glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer


		par_shapes_free_mesh(mesh);

	}

	return new_object;
}

void ModuleSceneManager::CreateGrid() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	float distance = 10.0f;

	for (int max_linesgrid = -distance; max_linesgrid < distance; max_linesgrid++)
	{
		glVertex3f((float)max_linesgrid, 0.0f, -distance);
		glVertex3f((float)max_linesgrid, 0.0f, distance);
		glVertex3f(-distance, 0.0f, (float)max_linesgrid);
		glVertex3f(distance, 0.0f, (float)max_linesgrid);
	}

	glVertex3f((float)-distance, 0.0f, distance);
	glVertex3f((float)distance, 0.0f, distance);
	glVertex3f((float)distance, 0.0f, -distance);
	glVertex3f((float)distance, 0.0f, distance);

	glLineWidth(1.0f);

	glEnd();
}
