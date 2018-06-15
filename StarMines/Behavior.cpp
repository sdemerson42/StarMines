#include "Behavior.h"
#include "BehaviorComponent.h"
#include "ComponentManager.h"
#include "LuaWrapper.h"

#include "LuaBridge.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


// Lua callable functions

int Behavior_getBCSize()
{
	return *LuaWrapper::bcompSz;
}

BehaviorComponent &Behavior_getBC(int index)
{
	return (*LuaWrapper::bcomps)[index];
}

Behavior::Behavior(ComponentManager *cm) :
	ISystem{ cm }
{
	luabridge::getGlobalNamespace(LuaWrapper::L).
		addFunction("getBCSize", &Behavior_getBCSize).
		addFunction("getBC", &Behavior_getBC);

	luabridge::getGlobalNamespace(LuaWrapper::L).
		beginClass<BehaviorComponent>("BehaviorComponent").
		addFunction("module", &BehaviorComponent::module).
		endClass();
}

void Behavior::update()
{
	// Execute Main lua script

	luaL_dofile(LuaWrapper::L, "Data/Lua/Main.lua");
	lua_pcall(LuaWrapper::L, 0, 0, 0);
}

void Behavior::onInputEvent(Events::InputEvent *evnt)
{
	BehaviorComponent::setInput(evnt);
}

