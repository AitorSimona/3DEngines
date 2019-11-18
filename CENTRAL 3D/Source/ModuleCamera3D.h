#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math.h"

class GameObject;
class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void FrameObject(GameObject* GO);

private:
	void CameraPan(float speed);
	void CameraZoom(float speed);
	void CameraLookAround(float speed, float3 reference);
public:
	float3 reference = { 0.0f,0.0f,0.0f };
	ComponentCamera* camera = nullptr;
};