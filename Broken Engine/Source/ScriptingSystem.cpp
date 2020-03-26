#include "ScriptingSystem.h"
#include "Application.h"
#include "ModuleTimeManager.h"
#include "ModuleGui.h"

using namespace Broken;
ScriptingSystem::ScriptingSystem() {}
		 
ScriptingSystem::~ScriptingSystem() {}

//Function that Lua will be able to call as LOG
void ScriptingSystem::LogFromLua(const char* string) {
	ENGINE_CONSOLE_LOG("[Script]: %s", string);
}

float ScriptingSystem::GetRealDT() const {
	return App->time->GetRealTimeDt();
}

float ScriptingSystem::GetDT() const {
	return App->time->GetGameDt();
}

float ScriptingSystem::GameTime()
{
	return App->time->GetGameplayTimePassed();
}

void ScriptingSystem::PauseGame()
{
	App->GetAppState() = Broken::AppState::TO_PAUSE;	
}

void ScriptingSystem::ResumeGame()
{
	App->GetAppState() = Broken::AppState::TO_PLAY;
}

// MATHS -------------------------------------------------------------
int ScriptingSystem::FloatNumsAreEqual(float a, float b)
{
	return (fabs(a - b) < std::numeric_limits<float>::epsilon());
}

int ScriptingSystem::DoubleNumsAreEqual(double a, double b)
{
	return (fabs(a - b) < std::numeric_limits<double>::epsilon());
}