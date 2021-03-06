#include "Behavior.h"
#include "BehaviorComponent.h"
#include "ComponentManager.h"
#include "RenderComponent.h"
#include "LuaWrapper.h"
#include "Renderer.h"

#include "LuaBridge.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

std::string Behavior::m_sceneName;

// Lua callable functions

int Behavior_getBCSize()
{
	return *LuaWrapper::bcompSz;
}

BehaviorComponent &Behavior_getBC(int index)
{
	return (*LuaWrapper::bcomps)[index];
}

const std::string &Behavior_sceneName()
{
	return Behavior::sceneName();
}

Behavior::Behavior(ComponentManager *cm) :
	ISystem{ cm }
{
	registerFunc(this, &Behavior::onInputEvent);
	registerFunc(this, &Behavior::onSceneChangeEvent);

	m_sceneName = "Main";


	luabridge::getGlobalNamespace(LuaWrapper::L).
		addFunction("getBCSize", &Behavior_getBCSize).
		addFunction("getBC", &Behavior_getBC).
		addFunction("sceneName", &Behavior_sceneName).
		addFunction("viewCenter", &Renderer::viewCenter).
		addFunction("setViewCenter", &Renderer::setViewCenter);

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
		addData("bc", &Ruff::Call::bc).
		endClass();

	luabridge::getGlobalNamespace(LuaWrapper::L).
		beginClass<RenderComponent::Color>("Color").
		addData("r", &RenderComponent::Color::r).
		addData("g", &RenderComponent::Color::g).
		addData("b", &RenderComponent::Color::b).
		addData("a", &RenderComponent::Color::a).
		endClass();

	luabridge::getGlobalNamespace(LuaWrapper::L).
		beginClass<Events::InputEvent>("Input").
		addData("xAxis", &Events::InputEvent::xAxis).
		addData("yAxis", &Events::InputEvent::yAxis).
		addData("uAxis", &Events::InputEvent::uAxis).
		addData("vAxis", &Events::InputEvent::vAxis).
		addData("zAxis", &Events::InputEvent::zAxis).
		addData("padXAxis", &Events::InputEvent::padXAxis).
		addData("padYAxis", &Events::InputEvent::padYAxis).
		addData("a", &Events::InputEvent::a).
		addData("b", &Events::InputEvent::b).
		addData("x", &Events::InputEvent::x).
		addData("y", &Events::InputEvent::y).
		addData("select", &Events::InputEvent::select).
		addData("start", &Events::InputEvent::start).
		addData("shoulderL", &Events::InputEvent::shoulderL).
		addData("shoulderR", &Events::InputEvent::shoulderR).
		addData("stickL", &Events::InputEvent::stickL).
		addData("stickR", &Events::InputEvent::stickR).
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
		addFunction("playMusic", &BehaviorComponent::playMusic).
		addFunction("stopMusic", &BehaviorComponent::stopMusic).
		addFunction("playAnim", &BehaviorComponent::playAnim).
		addFunction("setTargetByTag", &BehaviorComponent::setTargetByTag).
		addFunction("setTargetByCaller", &BehaviorComponent::setTargetByCaller).
		addFunction("targetPosition", &BehaviorComponent::targetPosition).
		addFunction("targetBehavior", &BehaviorComponent::targetBehavior).
		addFunction("active", &BehaviorComponent::active).
		addFunction("deactivate", &BehaviorComponent::deactivate).
		addFunction("input", &BehaviorComponent::input).
		addFunction("setText", &BehaviorComponent::setText).
		addFunction("appendText", &BehaviorComponent::appendText).
		addFunction("newScene", &BehaviorComponent::newScene).
		addFunction("gPersist", &BehaviorComponent::globalPersist).
		addFunction("createNewScene", &BehaviorComponent::createNewScene).
		addFunction("addSceneData", &BehaviorComponent::addSceneData).
		addFunction("rotate", &BehaviorComponent::rotate).
		addFunction("scale", &BehaviorComponent::scale).
		addFunction("color", &BehaviorComponent::color).
		addFunction("setColor", &BehaviorComponent::setColor).
		addFunction("textColor", &BehaviorComponent::textColor).
		addFunction("setTextColor", &BehaviorComponent::setTextColor).
		addFunction("callerHasTag", &BehaviorComponent::callerHasTag).
		endClass();
}

void Behavior::update()
{
	// On scene change: Execute scene change script

	if (m_sceneName != "")
	{
		luaL_dofile(LuaWrapper::L, "Data/Lua/System/_Persist.lua");
		lua_pcall(LuaWrapper::L, 0, 0, 0);
		m_sceneName = "";
	}

	// Execute Main lua script

	luaL_dofile(LuaWrapper::L, "Data/Lua/System/_Behavior.lua");
	lua_pcall(LuaWrapper::L, 0, 0, 0);

	// Late update

	for (int i = 0; i < m_compManager->m_behaviorSz; ++i)
	{
		m_compManager->m_behavior[i].update();
	}
}

const std::string &Behavior::sceneName()
{
	return m_sceneName;
}

void Behavior::onInputEvent(Events::InputEvent *evnt)
{
	BehaviorComponent::setInput(evnt);
}

void Behavior::onSceneChangeEvent(Events::SceneChangeEvent *evnt)
{
	m_sceneName = evnt->name;
}

