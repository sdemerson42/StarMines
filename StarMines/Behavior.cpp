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
		beginClass<Vector2>("Vector2").
		addData("x", &Vector2::x).
		addData("y", &Vector2::y).
		endClass();

	luabridge::getGlobalNamespace(LuaWrapper::L).
		beginClass<Ruff::Call>("Call").
		addData("caller", &Ruff::Call::caller).
		addData("tag", &Ruff::Call::tag).
		addData("label", &Ruff::Call::label).
		addData("sdata", &Ruff::Call::sdata).
		endClass();

	luabridge::getGlobalNamespace(LuaWrapper::L).
		beginClass<BehaviorComponent>("BehaviorComponent").
		addFunction("module", &BehaviorComponent::module).
		addFunction("position", &BehaviorComponent::position).
		addFunction("setPosition", &BehaviorComponent::setPosition).
		addFunction("speed", &BehaviorComponent::speed).
		addFunction("setSpeed", &BehaviorComponent::setSpeed).
		addFunction("dir", &BehaviorComponent::dir).
		addFunction("setDir", &BehaviorComponent::setDir).
		addFunction("getRegInt", &BehaviorComponent::getRegisterInt).
		addFunction("setRegInt", &BehaviorComponent::setRegisterInt).
		addFunction("getRegFloat", &BehaviorComponent::getRegisterFloat).
		addFunction("setRegFloat", &BehaviorComponent::setRegisterFloat).
		addFunction("getCall", &BehaviorComponent::getCall).
		addFunction("sendToTag", &BehaviorComponent::sendToTag).
		addFunction("sendToCaller", &BehaviorComponent::sendToCaller).
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


