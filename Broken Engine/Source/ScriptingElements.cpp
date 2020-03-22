#include "ScriptingElements.h"
#include "Application.h"
#include "ModuleTimeManager.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ModuleSceneManager.h"
#include "ResourceScene.h"
#include "GameObject.h"
#include "Components.h"
#include "ModuleRenderer3D.h"

#include "../Game/Assets/Sounds/Wwise_IDs.h"
#include "ModuleAudio.h"

#include "ScriptData.h"
#include "LuaIncludes.h"

using namespace Broken;
ScriptingElements::ScriptingElements() {}

ScriptingElements::~ScriptingElements() {}

// Returns the UID of the GameObject if it is found
uint ScriptingElements::FindGameObject(const char* go_name)
{
	uint ret = 0;

	GameObject* go = App->scene_manager->currentScene->GetGOWithName(go_name);

	if (go != nullptr)
	{
		ret = go->GetUID();
	}
	else
	{
		ENGINE_CONSOLE_LOG("(SCRIPTING) Alert! Gameobject %s was not found! 0 will be returned", go_name);
	}
	return ret;
}

float ScriptingElements::GetGameObjectPos(uint gameobject_UID, lua_State* L)
{
	float ret = 0;
	float3 rot = float3(0.0f);

	GameObject* go = (*App->scene_manager->currentScene->NoStaticGameObjects.find(gameobject_UID)).second;
	if (go == nullptr)
	{
		go = (*App->scene_manager->currentScene->StaticGameObjects.find(gameobject_UID)).second;
	}

	ComponentTransform* transform;
	transform = go->GetComponent<ComponentTransform>();
	if (go != nullptr && transform != nullptr)
	{
		rot = transform->GetPosition();
		ret = 3;
	}
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");

	lua_pushnumber(L, rot.x);
	lua_pushnumber(L, rot.y);
	lua_pushnumber(L, rot.z);
	return 0.0f;
}

float ScriptingElements::GetGameObjectPosX(uint gameobject_UID)
{
	float ret = 0.0f;
	GameObject* GO = App->scene_manager->currentScene->GetGOWithUID(gameobject_UID);

	if (GO != nullptr)
	{
		ComponentTransform* transform;
		transform = GO->GetComponent<ComponentTransform>();

		if (transform)
		{
			ret = transform->GetGlobalPosition().x;
		}
	}

	return ret;
}

float ScriptingElements::GetGameObjectPosY(uint gameobject_UID)
{
	float ret = 0.0f;
	GameObject* GO = App->scene_manager->currentScene->GetGOWithUID(gameobject_UID);

	if (GO != nullptr)
	{
		ComponentTransform* transform;
		transform = GO->GetComponent<ComponentTransform>();

		if (transform)
		{
			ret = transform->GetGlobalPosition().y;
		}
	}

	return ret;
}

float ScriptingElements::GetGameObjectPosZ(uint gameobject_UID)
{
	float ret = 0.0f;
	GameObject* GO = App->scene_manager->currentScene->GetGOWithUID(gameobject_UID);

	if (GO != nullptr)
	{
		ComponentTransform* transform;
		transform = GO->GetComponent<ComponentTransform>();

		if (transform)
		{
			ret = transform->GetGlobalPosition().z;
		}
	}

	return ret;
}

void ScriptingElements::TranslateGameObject(uint gameobject_UID, float x, float y, float z)
{
	GameObject* go = (*App->scene_manager->currentScene->NoStaticGameObjects.find(gameobject_UID)).second;
	if (go == nullptr)
	{
		go = (*App->scene_manager->currentScene->StaticGameObjects.find(gameobject_UID)).second;
	}

	ComponentTransform* transform;
	transform = go->GetComponent<ComponentTransform>();

	if (transform)
	{
		float3 trans_pos = transform->GetPosition();

		trans_pos.x += x;
		trans_pos.y += y;
		trans_pos.z += z;

		transform->SetPosition(trans_pos.x, trans_pos.y, trans_pos.z);
	}
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
}

uint ScriptingElements::GetComponentFromGO(const char* component_name, const char* go_name)
{
	uint ret = 0;
	GameObject* go = nullptr;

	if (go_name == "NO_NAME")
		go = App->scripting->current_script->my_component->GetContainerGameObject();
	else
		go = App->scene_manager->currentScene->GetGOWithName(go_name);		

	if (go != nullptr)
	{
		Component* comp = nullptr;
		std::string name = component_name;

		if (name.compare("Animation") == 0)
			comp = (Component*)go->GetComponent<ComponentAnimation>();
		else if (name.compare("Audio") == 0)
			comp = (Component*)go->GetComponent<ComponentAudioSource>();
		else if (name.compare("Bone") == 0)
			comp = (Component*)go->GetComponent<ComponentBone>();
		else if (name.compare("Camera") == 0)
			comp = (Component*)go->GetComponent<ComponentCamera>();
		else if (name.compare("Collider") == 0)
			comp = (Component*)go->GetComponent<ComponentCollider>();
		else if (name.compare("Rigidbody") == 0)
			comp = (Component*)go->GetComponent<ComponentDynamicRigidBody>();
		else if (name.compare("Mesh") == 0)
			comp = (Component*)go->GetComponent<ComponentMesh>();
		else if (name.compare("Emitter") == 0)
			comp = (Component*)go->GetComponent<ComponentParticleEmitter>();

		if (comp != nullptr)
		{
			ret = comp->GetUID();
		}
		else
		{
			ENGINE_CONSOLE_LOG("(SCRIPTING) Alert! Component %s was not found inside Gameobject %s! 0 will be returned", component_name, go_name);
		}
	}
	else
	{
		ENGINE_CONSOLE_LOG("(SCRIPTING) Alert! Gameobject %s was not found! 0 will be returned", go_name);
	}
	
	return ret;
}

float ScriptingElements::GetPositionX() const
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		return transform->GetPosition().x;
	else
	{
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
		return 0.0f;
	}
}

float ScriptingElements::GetPositionY() const
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		return transform->GetPosition().y;
	else
	{
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
		return 0.0f;
	}
}

float ScriptingElements::GetPositionZ() const
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		return transform->GetPosition().z;
	else
	{
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
		return 0.0f;
	}
}

int ScriptingElements::GetPosition(lua_State* L)
{
	int ret = 0;
	float3 rot = float3(0.0f);

	ComponentTransform* transform;
	if ((transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>()))
	{
		rot = transform->GetPosition();
		ret = 3;
	}
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");

	lua_pushnumber(L, rot.x);
	lua_pushnumber(L, rot.y);
	lua_pushnumber(L, rot.z);
	return ret;
}

void ScriptingElements::Translate(float x, float y, float z, bool local)
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
	{
		float3 trans_pos = transform->GetPosition();

		trans_pos.x += x;
		trans_pos.y += y;
		trans_pos.z += z;

		transform->SetPosition(trans_pos.x, trans_pos.y, trans_pos.z);
	}
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
}

void ScriptingElements::SetPosition(float x, float y, float z, bool local)
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		transform->SetPosition(x, y, z);
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
}

void ScriptingElements::RotateObject(float x, float y, float z)
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();
	ComponentCollider* collider = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentCollider>();
	ComponentDynamicRigidBody* rb = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentDynamicRigidBody>();

	if (transform && rb && collider)
	{
		if (!rb->rigidBody)
			return;

		physx::PxTransform globalPos = rb->rigidBody->getGlobalPose();
		Quat quaternion = Quat::FromEulerXYZ(DEGTORAD * x, DEGTORAD * y, DEGTORAD * z);
		physx::PxQuat quat = physx::PxQuat(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
		globalPos = physx::PxTransform(globalPos.p, quat);

		collider->UpdateTransformByRigidBody(rb, transform, &globalPos);

	}
	else if (transform)
	{
		float3 rot = transform->GetRotation();
		rot = float3(x, y, z);
		transform->SetRotation(rot);
	}
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
}

void ScriptingElements::SetObjectRotation(float x, float y, float z)
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		transform->SetRotation({ x, y, z });
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
}

void ScriptingElements::LookAt(float spotX, float spotY, float spotZ, bool local)
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();
	ComponentCollider* collider = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentCollider>();
	ComponentDynamicRigidBody* rb = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentDynamicRigidBody>();

	if (transform)
	{
		float3 zaxis = float3(transform->GetGlobalPosition() - float3(spotX, spotY, spotZ)).Normalized();
		float3 xaxis = float3(zaxis.Cross(float3(0, 1, 0))).Normalized();
		float3 yaxis = xaxis.Cross(zaxis);
		zaxis = zaxis.Neg();

		float4x4 m = {
		   float4(xaxis.x, xaxis.y, xaxis.z, -Dot(xaxis, transform->GetGlobalPosition())),
		   float4(yaxis.x, yaxis.y, yaxis.z, -Dot(yaxis, transform->GetGlobalPosition())),
		   float4(zaxis.x, zaxis.y, zaxis.z, -Dot(zaxis, transform->GetGlobalPosition())),
		   float4(0, 0, 0, 1)
		};
		m.Transpose();

		float3 pos, scale;
		Quat rot;

		m.Decompose(pos, rot, scale);

		rot = rot.Inverted();


		if (rb && collider)
		{
			if (!rb->rigidBody)
				return;

			physx::PxTransform globalPos = rb->rigidBody->getGlobalPose();
			physx::PxQuat quat = physx::PxQuat(rot.x, rot.y, rot.z, rot.w);
			globalPos = physx::PxTransform(globalPos.p, quat);

			collider->UpdateTransformByRigidBody(rb, transform, &globalPos);
		}
		else
			transform->SetRotation(rot);

	}
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
}

int ScriptingElements::GetRotation(bool local, lua_State* L) const
{
	int ret = 0;
	float3 rot = float3(0.0f);
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
	{
		rot = transform->GetRotation();
		ret = 3;
	}
	else
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");

	lua_pushnumber(L, rot.x);
	lua_pushnumber(L, rot.y);
	lua_pushnumber(L, rot.z);
	return ret;
}

float ScriptingElements::GetRotationX() const
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		return transform->GetRotation().x;
	else
	{
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
		return 0.0f;
	}
}

float ScriptingElements::GetRotationY() const
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		return transform->GetRotation().y;
	else
	{
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
		return 0.0f;
	}
}

float ScriptingElements::GetRotationZ() const
{
	ComponentTransform* transform = App->scripting->current_script->my_component->GetContainerGameObject()->GetComponent<ComponentTransform>();

	if (transform)
		return transform->GetRotation().z;
	else
	{
		ENGINE_CONSOLE_LOG("Object or its transformation component are null");
		return 0.0f;
	}
}

// CAMERAS -----------------------------------------------------------
int ScriptingElements::GetPosInFrustum(float x, float y, float z, float fovratio1, float fovratio2)
{
	ComponentCamera* cam = App->renderer3D->active_camera;

	int camlevel = 0;

	if (cam)
	{
		// --- Create subdivisions of the frustum ---
		Frustum sub1 = cam->frustum;
		Frustum sub2 = cam->frustum;

		sub1.SetVerticalFovAndAspectRatio(cam->GetFOV() * DEGTORAD * fovratio1, cam->frustum.AspectRatio());
		sub2.SetVerticalFovAndAspectRatio(cam->GetFOV() * DEGTORAD * fovratio2, cam->frustum.AspectRatio());

		// --- Check in which subdivision we are ---
		if ((int)cam->frustum.Contains({ x, y, z }) == true)
		{
			camlevel = 1;

			if ((int)sub1.Contains({ x, y, z }) == true)
			{
				camlevel = 2;

				if ((int)sub2.Contains({ x, y, z }) == true)
				{
					camlevel = 3;
				}
			}
		}
	}
	else
		ENGINE_CONSOLE_LOG("[Script]: Current Active camera is NULL");

	return camlevel;
}

luabridge::LuaRef ScriptingElements::GetScript(uint go_UID, lua_State* L)
{
	luabridge::LuaRef ret = 0;

	GameObject* go = App->scene_manager->currentScene->GetGOWithUID(go_UID);

	if (go != nullptr)
	{
		ComponentScript* component_script = go->GetComponent<ComponentScript>();
		ScriptInstance* script = App->scripting->GetScriptInstanceFromComponent(component_script);

		ret = script->my_table_class;
	}
	return ret;
}