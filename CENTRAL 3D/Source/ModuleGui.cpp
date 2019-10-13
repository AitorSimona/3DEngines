#include "Application.h"
#include "Globals.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "Panel.h"
#include "PanelSettings.h"
#include "PanelAbout.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelHierarchy.h"
#include "PanelScene.h"
#include "PanelToolbar.h"

#include "Imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/ImGuizmo/ImGuizmo.h"

#include "OpenGL.h"

#include "mmgr/mmgr.h"



ModuleGui::ModuleGui(bool start_enabled) : Module(start_enabled)
{
	name = "GUI";
}

ModuleGui::~ModuleGui()
{}

bool ModuleGui::Init(json file)
{
	panelSettings = new PanelSettings("Settings");
	panels.push_back(panelSettings);

	panelAbout = new PanelAbout("About");
	panels.push_back(panelAbout);

	panelConsole = new PanelConsole("Console");
	panels.push_back(panelConsole);

	panelInspector = new PanelInspector("Inspector");
	panels.push_back(panelInspector);

	panelHierarchy = new PanelHierarchy("Hierarchy");
	panels.push_back(panelHierarchy);

	panelScene = new PanelScene("Scene");
	panels.push_back(panelScene);

	panelToolbar = new PanelToolbar("Toolbar");
	panels.push_back(panelToolbar);

	LoadStatus(file);

	return true;
}

bool ModuleGui::Start()
{
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Window Docking (Under Active Development)
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Deactivated because of lib crash when resizing window out of Main window bounds

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();


	return ret;
}

update_status ModuleGui::PreUpdate(float dt)
{  
	// Start the frame

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	//ImGuizmo::BeginFrame();

	//int mouse_x, mouse_y;
	//SDL_GetMouseState(&mouse_x, &mouse_y);
	//ImGui::GetIO().MousePos = ImVec2(float(mouse_x), float(mouse_y));

	ImGuiIO& io = ImGui::GetIO();
	capture_keyboard = io.WantCaptureKeyboard;
	capture_mouse = io.WantCaptureMouse;

	// Begin dock space
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	DockSpace();

	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update(float dt)
{

	if (ImGui::BeginMainMenuBar())
	{
		
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit"))
				{
					return UPDATE_STOP;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Console"))
				{
					panelConsole->OnOff();
				}

				if (ImGui::MenuItem("Configuration"))
				{
					panelSettings->OnOff();
				}

				if (ImGui::MenuItem("Inspector"))
				{
					panelInspector->OnOff();
				}

				if (ImGui::MenuItem("Hierarchy"))
				{
					panelHierarchy->OnOff();
				}

				if (ImGui::MenuItem("Scene"))
				{
					panelScene->OnOff();
				}

				if (ImGui::MenuItem("Toolbar"))
				{
					panelToolbar->OnOff();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("ImGui Demo"))
				{
					show_demo_window = !show_demo_window;
				}

				if (ImGui::MenuItem("Documentation"))
				{
					RequestBrowser("https://github.com/AitorSimona/CENTRAL-3D/wiki");
				}

				if (ImGui::MenuItem("Download latest"))
				{
					RequestBrowser("https://github.com/AitorSimona/CENTRAL-3D/releases");
				}

				if (ImGui::MenuItem("Report a bug"))
				{
					RequestBrowser("https://github.com/AitorSimona/CENTRAL-3D/issues");
				}

				if (ImGui::MenuItem("About"))
				{
					panelAbout->OnOff();
				}

				ImGui::EndMenu();
			}


			ImGui::EndMainMenuBar();
	}


	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate(float dt)
{
	// --- Iterate panels and draw ---
	for (uint i = 0; i < panels.size(); ++i)
	{
		if (panels[i]->IsEnabled())
			panels[i]->Draw();
	}

	// End dock space
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	ImGui::End();

	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	bool ret = true;

	// --- Iterate panels and delete ---
	for (uint i = 0; i < panels.size(); ++i)
	{
		delete panels[i];
		panels[i] = nullptr;
	}

	panelSettings = nullptr;
	panelAbout = nullptr;
	panelConsole = nullptr;
	panelHierarchy = nullptr;
	panelInspector = nullptr;
	panelScene = nullptr;
	panelToolbar = nullptr;


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}

void ModuleGui::Draw() const
{
	// Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}

void ModuleGui::DockSpace() const
{
	// --- Adapt to Window changes like resizing ---
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	//ImGui::DockSpaceOverViewport(viewport,dockspace_flags);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));

	static bool p_open = true;
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
}

void ModuleGui::RequestBrowser(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}


void ModuleGui::LogFPS(float fps, float ms)
{
	if (panelSettings != nullptr)
		panelSettings->AddFPS(fps, ms);
}

void ModuleGui::SaveStatus(json &file) const  
{
	for (uint i = 0; i < panels.size(); ++i)
		file["GUI"][panels[i]->GetName()] = panels[i]->IsEnabled();
};

void ModuleGui::LoadStatus(const json & file) 
{
	
	for (uint i = 0; i < panels.size(); ++i)
	{
		if (file["GUI"].find(panels[i]->GetName()) != file["GUI"].end())
			panels[i]->SetOnOff(file["GUI"][panels[i]->GetName()]);
		else
			LOG("|[error]: Could not find sub-node %s in GUI JSON Node, please check JSON EditorConfig", panels[i]->GetName());
	}
}
void ModuleGui::HandleInput(SDL_Event * event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

bool ModuleGui::IsKeyboardCaptured()
{
	return capture_keyboard;
}

;
