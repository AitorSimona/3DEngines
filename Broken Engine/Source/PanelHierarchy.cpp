#include "PanelHierarchy.h"
#include "Imgui/imgui.h"

#include "EngineApplication.h"
#include "PanelProject.h"

using namespace Broken;

PanelHierarchy::PanelHierarchy(char * name) : Broken::Panel(name) {}

PanelHierarchy::~PanelHierarchy() {}

bool PanelHierarchy::Draw()
{
	ImGui::SetCurrentContext(EngineApp->gui->getImgUICtx());

	ImGuiWindowFlags settingsFlags = 0;
	settingsFlags = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin(name, &enabled, settingsFlags))
	{
		ImGui::BeginMenuBar();
		ImGui::Image((ImTextureID)EngineApp->gui->sceneTexID, ImVec2(15, 15), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text(EngineApp->scene_manager->currentScene->GetName());
		ImGui::EndMenuBar();

		DrawRecursive(EngineApp->scene_manager->GetRootGO());

		// Deselect the current GameObject when clicking in an empty space of the hierarchy
		if (ImGui::InvisibleButton("##Deselect", { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - ImGui::GetCursorPosY() }))
			EngineApp->scene_manager->SetSelectedGameObject(nullptr);

		// Allow creating GameObjects and UI Elements from the hierarchy
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN
			&& ImGui::GetMousePos().x < ImGui::GetWindowWidth() + ImGui::GetWindowPos().x && ImGui::GetMousePos().x > ImGui::GetWindowPos().x
			&& ImGui::GetMousePos().y < ImGui::GetWindowHeight() + ImGui::GetWindowPos().y && ImGui::GetMousePos().y > ImGui::GetWindowPos().y)
			ImGui::OpenPopup("Create new element");

		if (ImGui::BeginPopup("Create new element")) {
			if (ImGui::BeginMenu("3D Object")) {
				if (ImGui::MenuItem("Empty Game Object")) {
					Broken::GameObject* go = EngineApp->scene_manager->CreateEmptyGameObject();
				}

				if (ImGui::MenuItem("Plane")) {
					Broken::GameObject* obj = EngineApp->scene_manager->LoadPlane();
					obj->GetComponent<Broken::ComponentTransform>()->SetRotation({ -90, 0, 0 });
					obj->GetComponent < Broken::ComponentTransform >()->Scale(10, 10, 10);
				}

				if (ImGui::MenuItem("Cube"))
					EngineApp->scene_manager->LoadCube();

				if (ImGui::MenuItem("Cylinder"))
					EngineApp->scene_manager->LoadCylinder()->GetComponent<Broken::ComponentTransform>()->SetRotation({ -90, 0, 0 });

				if (ImGui::MenuItem("Capsule"))
					EngineApp->scene_manager->LoadCapsule();

				if (ImGui::MenuItem("Sphere"))
					EngineApp->scene_manager->LoadSphere();

				if (ImGui::MenuItem("Camera")) {
					Broken::GameObject* cam = EngineApp->scene_manager->CreateEmptyGameObject();

					Broken::ComponentCamera* camera = (Broken::ComponentCamera*)cam->AddComponent(Broken::Component::ComponentType::Camera);
					cam->AddComponent(Broken::Component::ComponentType::MeshRenderer);
					camera->SetFarPlane(10);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("UI Element")) {
				if (ImGui::MenuItem("Canvas")) {
					Broken::GameObject* canvas_go = EngineApp->scene_manager->CreateEmptyGameObject();
					Broken::ComponentCanvas* camera = (Broken::ComponentCanvas*)canvas_go->AddComponent(Broken::Component::ComponentType::Canvas);
				}
				if (ImGui::MenuItem("Image")) {
					Broken::GameObject* image_go = EngineApp->scene_manager->CreateEmptyGameObject();
					Broken::ComponentImage* image = (Broken::ComponentImage*)image_go->AddComponent(Broken::Component::ComponentType::Image);
				}
				if (ImGui::MenuItem("Text")) {
					Broken::GameObject* text_go = EngineApp->scene_manager->CreateEmptyGameObject();
					Broken::ComponentText* text = (Broken::ComponentText*)text_go->AddComponent(Broken::Component::ComponentType::Text);
				}
				if (ImGui::MenuItem("Button", false, false, false)) {
					Broken::GameObject* button_go = EngineApp->scene_manager->CreateEmptyGameObject();
					Broken::ComponentText* button = (Broken::ComponentText*)button_go->AddComponent(Broken::Component::ComponentType::Button);
				}
				if (ImGui::MenuItem("Checkbox", false, false, false)) {
				}
				if (ImGui::MenuItem("Input Text", false, false, false)) {
				}
				if (ImGui::MenuItem("Progress Bar", false, false, false)) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}			
	}
	ImGui::End();

	// --- Manage Drag & Drop ---
	if (end_drag)
	{
		if (!dragged->FindChildGO(target) && target != dragged)
			target->AddChildGO(dragged);

		end_drag = false;
		dragged = nullptr;
		target = nullptr;
	}

	return true;
}

void PanelHierarchy::DrawRecursive(Broken::GameObject * Go)
{
	// --- Set node flags ---
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags node_flags = base_flags;

	if (Go == EngineApp->scene_manager->GetSelectedGameObject())
		node_flags |= ImGuiTreeNodeFlags_Selected;

	// --- Avoid displaying root ---
	if (Go->GetName() == EngineApp->scene_manager->GetRootGO()->GetName())
	{
		if (Go->childs.size() > 0)
		{
			for (std::vector<Broken::GameObject*>::iterator it = Go->childs.begin(); it != Go->childs.end(); ++it)
			{
				DrawRecursive(*it);
			}
		}
	}

	// --- Display Go node ---
    else 
	{
		if (Go->childs.size() == 0)
			node_flags |= ImGuiTreeNodeFlags_Leaf;

		// --- Create current node and get if it is opened or not ---

		if(!Go->GetActive())
		ImGui::PushStyleColor(ImGuiCol(), ImVec4(0.5, 0.5, 0.5, 1));

		ImGui::Image((ImTextureID)EngineApp->gui->prefabTexID, ImVec2(15, 15), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();

		bool open = ImGui::TreeNodeEx((void*)Go->GetUID(), node_flags, Go->GetName());

		if (!Go->GetActive())
		ImGui::PopStyleColor();

		// Our buttons are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			uint UID = Go->GetUID();
			ImGui::SetDragDropPayload("GO", &UID, sizeof(uint));
			dragged = Go;
			ImGui::Text(Go->GetName());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GO"))
			{
				target = Go;
				end_drag = true;
			}
			ImGui::EndDragDropTarget();
		}

		// --- Set Game Object to be destroyed ---
		if (ImGui::IsWindowFocused() && Go == EngineApp->scene_manager->GetSelectedGameObject() && EngineApp->input->GetKey(SDL_SCANCODE_DELETE) == Broken::KEY_DOWN)
		{
			EX_ENGINE_CONSOLE_LOG("Destroying: %s ...",  Go->GetName());
			EngineApp->scene_manager->SendToDelete(Go);
		}

		// --- Handle selection ---
		if (selected_uid == Go->GetUID() && wasclicked && ImGui::IsMouseReleased(0))
		{
			if (ImGui::IsItemHovered())
			{
				EngineApp->scene_manager->SetSelectedGameObject(Go);
				wasclicked = false;
			}
			else
				EngineApp->scene_manager->SetSelectedGameObject(nullptr);
		}

		// --- Handle selection ---
		if (ImGui::IsItemClicked())
		{
			selected_uid = Go->GetUID();
			wasclicked = true;
		}


		// --- Display children only if current node is open ---
		if (open)
		{
			// --- Check for children and draw them the same way ---
			if (Go->childs.size() > 0)
			{
				for (std::vector<Broken::GameObject*>::iterator it = Go->childs.begin(); it != Go->childs.end(); ++it)
				{
					DrawRecursive(*it);
				}
			}

			ImGui::TreePop();
		}

	}
}
