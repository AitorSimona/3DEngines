#ifndef __SCRIPTINGGAMEOBJECT_H__
#define __SCRIPTINGGAMEOBJECT_H__

#include "BrokenCore.h"
class lua_State;

namespace luabridge
{
	class LuaRef;
}

BE_BEGIN_NAMESPACE
class BROKEN_API ScriptingGameobject {
public:
	ScriptingGameobject();
	~ScriptingGameobject();

public:

	// GameObject-related functions
	uint FindGameObject(const char* go_name);
	uint GetMyUID();
	uint GetMyLayer(lua_State* L);
	uint GetLayerByID(uint UID, lua_State* L);
	uint GetScriptGOParent();
	uint GetGOParentFromUID(uint gameobject_UUID);
	void DestroyGOFromScript(uint gameobject_UUID);
	void SetActiveGameObject(uint gameobject_UUID, bool active);

	luabridge::LuaRef GetGameObjectPos(uint gameobject_UUID, lua_State* L);
	float GetGameObjectPosX(uint gameobject_UUID);
	float GetGameObjectPosY(uint gameobject_UUID);
	float GetGameObjectPosZ(uint gameobject_UUID);
	void TranslateGameObject(uint gameobject_UUID, float x, float y, float z);

	uint GetComponentFromGO(uint gameobject_UUID, const char* component_name);

	// Current Camera
	int GetPosInFrustum(float x, float y, float z, float fovratio1, float fovratio2);
	int GetFrustumPlanesIntersection(float x, float y, float z, float fovratio, lua_State* L);
	int GetTopFrustumIntersection(float x, float y, float z, float fovratio);
	int GetBottomFrustumIntersection(float x, float y, float z, float fovratio);
	int GetRightFrustumIntersection(float x, float y, float z, float fovratio);
	int GetLeftFrustumIntersection(float x, float y, float z, float fovratio);

	// Get a script file
	luabridge::LuaRef GetScript(uint gameobject_UUID, lua_State* L);
};
BE_END_NAMESPACE
#endif // __SCRIPTINGGAMEOBJECT_H__
