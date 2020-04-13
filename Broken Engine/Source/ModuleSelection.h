#ifndef __SELECTION_H__
#define __SELECTION_H__

#include "Module.h"
#include "GameObject.h"
#include "Imgui/ImGuizmo/ImGuizmo.h"

//#include "Imgui/imgui.h"

/*
	I use this to not include the whole library of imgui
	ImGuiTreeNodeFlags_Selected 1
	ImGuiTreeNodeFlags_OpenOnArrow 128
	ImGuiTreeNodeFlags_SpanAvailWidth 2048
*/

/* CHANGE LIST
	- Added ModuleSelection to manage selection
	- **NEW** Visual scene rectangle selection 
	- **NEW** Script name in component title 
	- **NEW** Selection counter in inspector 
	- Selection can be both on hierarchy and scene:
		- Single -> mouse left click
		- Additive/Substractive -> mouse left click + CTRL
		- Multiple (weird on scene)-> mouse left click + SHIFT if there's at least one selected
	- Selected gameobjects can now:
		- Add same component
		- Be highlighted both scene and hierarchy
		- Be deleted at once
		- Paste previously copied components 
		- Delete same component
		- **NEW** Change parent to the dragged one 
		- **NEW** Guizmo transformation to all 
	- TODO
		- Scene visual rectangle selection (just fix one matrix)
		- Bugs when guizmo and parents (maybe is root obj)

*/



BE_BEGIN_NAMESPACE

class BROKEN_API ModuleSelection :	public Module
{
public:

	ModuleSelection(bool start_enabled = true);
	~ModuleSelection();
	bool Init(json& file);
	bool Start();
	bool CleanUp();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);


public:

	bool IsSelected(const GameObject* gameobject) const;

	void HandleSelection(GameObject* gameobject);

	GameObject* GetLastSelected();

	inline std::vector<GameObject*>* GetSelected() { return &root->childs; }

	// change update_root to false if you don't want the scene visual rectangle to clear
	void ClearSelection(bool update_root = true);

	// Component management
	void CopyComponentValues(Component* component);

	void PasteComponentValues(Component* component);

	void PasteComponentValuesToSelected();

	void DeleteComponentToSelected();

	bool ComponentCanBePasted() const;

	std::string component_name = "None";


	// Simple selection
	void Select(GameObject* gameobject);

	void UnSelect(GameObject* gameobject);

	// Initializer
	void SceneRectangleSelect(float3 start);

	// Values passed must be of the deltaMatrix of the guizmo
	void UseGuizmo(ImGuizmo::OPERATION guizmoOperation, ImGuizmo::MODE guizmoMode, float3 pos, float3 rot, float3 scale);

private:
	// Scene rectangle 
	void ApplyOBBTransformation();

	void SelectIfIntersects();

	void UpdateRoot();

	// Advanced selection
	void SelectLastTo(GameObject* gameobject);

	void SelectRecursive(GameObject* gameobject, GameObject* from, GameObject* to);

	bool ToggleSelect(GameObject* gameobject);


public:

	// Please DO NOT EDIT
	GameObject* root = nullptr;

private:

	// Shift selection
	bool start_selecting = false;
	bool stop_selecting = false;
	bool reverse_selecting = false;
	
	// Visual scene selection
	OBB aabb;
	AABB aabb_selection;
	bool is_rectangle_selection = false;
	float3 aabb_start = float3::zero;
	float3 aabb_end = float3::zero;

	// Guizmo 
	std::vector<float3> original_scales;

	// Component copy and paste
	json component_node;
	Component::ComponentType component_type = Component::ComponentType::Unknown;
};

BE_END_NAMESPACE
#endif