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
	registerFunc(this, &Behavior::onInputEvent);

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
		beginClass<BehaviorComponent::CInput>("CInput").
		addData("x", &BehaviorComponent::CInput::x).
		addData("y", &BehaviorComponent::CInput::y).
		addData("u", &BehaviorComponent::CInput::u).
		addData("v", &BehaviorComponent::CInput::v).
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
		addFunction("incRegInt", &BehaviorComponent::incRegisterInt).
		addFunction("decRegInt", &BehaviorComponent::decRegisterInt).
		addFunction("getRegFloat", &BehaviorComponent::getRegisterFloat).
		addFunction("setRegFloat", &BehaviorComponent::setRegisterFloat).
		addFunction("getCall", &BehaviorComponent::getCall).
		addFunction("clearCalls", &BehaviorComponent::clearCalls).
		addFunction("sendToTag", &BehaviorComponent::sendToTag).
		addFunction("sendToCaller", &BehaviorComponent::sendToCaller).
		addFunction("spawn", &BehaviorComponent::spawn).
		addFunction("despawn", &BehaviorComponent::despawn).
		addFunction("playSound", &BehaviorComponent::playSound).
		addFunction("stopSound", &BehaviorComponent::stopSound).
		addFunction("playAnim", &BehaviorComponent::playAnim).
		addFunction("setTargetByTag", &BehaviorComponent::setTargetByTag).
		addFunction("setTargetByCaller", &BehaviorComponent::setTargetByCaller).
		addFunction("targetPosition", &BehaviorComponent::targetPosition).
		addFunction("active", &BehaviorComponent::active).
		addFunction("deactivate", &BehaviorComponent::deactivate).
		addFunction("input", &BehaviorComponent::input).
		addFunction("setText", &BehaviorComponent::setText).
		addFunction("appendText", &BehaviorComponent::appendText).
		addFunction("newScene", &BehaviorComponent::newScene).
		endClass();
}

void Behavior::update()
{
	// Execute Main lua script

	luaL_dofile(LuaWrapper::L, "Data/Lua/Main.lua");
	lua_pcall(LuaWrapper::L, 0, 0, 0);

	// Late update

	for (int i = 0; i < m_compManager->m_behaviorSz; ++i)
	{
		m_compManager->m_behavior[i].update();
	}
}

void Behavior::onInputEvent(Events::InputEvent *evnt)
{
	BehaviorComponent::setInput(evnt);
}


