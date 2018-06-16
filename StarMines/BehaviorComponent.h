#pragma once

#include "IComponent.h"
#include "Events.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "RuffCommon.h"

class Entity;

class BehaviorComponent : public IComponent
{
	friend class Behavior;
public:
	BehaviorComponent() :
		m_target{ nullptr }
	{
		registerFunc(this, &BehaviorComponent::onQueryEntityByTag);
		m_call.reserve(64);
	}

	static void setInput(Events::InputEvent *evnt)
	{
		m_input.x = evnt->xAxis;
		m_input.y = evnt->yAxis;
		m_input.u = evnt->uAxis;
		m_input.v = evnt->vAxis;

		if (abs(m_input.x) < m_axisDeadzone) m_input.x = 0.0f;
		if (abs(m_input.y) < m_axisDeadzone) m_input.y = 0.0f;
		if (abs(m_input.u) < m_axisDeadzone) m_input.u = 0.0f;
		if (abs(m_input.v) < m_axisDeadzone) m_input.v = 0.0f;
	}

	void initialize(const std::vector<std::string> &input) override
	{
		m_luaModule = input[0];
	}
	void update()
	{
		// BC Loop handled by Main.lua
	}
	const std::string &getTag() const override
	{
		return m_tag;
	}

	// Lua Interface Functions

	const std::string &module();
	const Vector2 &position();
	void setPosition(float x, float y);
	float speed();
	void setSpeed(float x);
	const Vector2 &dir();
	void setDir(float x, float y);
	const Ruff::Call &getCall();
	void sendToTag(const std::string &tag, const std::string &label, const std::string &sdata);
	void sendToCaller(const std::string &label, const std::string &sdata);

	// End Lua



	void onMove()
	{
		// Probably no longer necessary
		//m_vm.setParent(this);
	}
	//void resetVM();

	void addCall(Ruff::Call &c)
	{
		m_call.emplace_back(c);
	}

	std::vector<int> getSceneDespawnData()
	{
		auto r{ m_sceneDespawnData };
		m_sceneDespawnData.clear();
		return r;
	}
	std::vector<int> &refSceneDespawnData()
	{
		return m_sceneDespawnData;
	}

	// Behavior state access methods

	static BehaviorComponent *getCurrentComponent()
	{
		return m_currentComponent;
	}

	int getRegisterInt(int index) const
	{
		return m_register[index].i;
	}
	float getRegisterFloat(int index) const
	{
		return m_register[index].f;
	}
	void setRegisterInt(int index, int val)
	{
		m_register[index].i = val;
	}
	void setRegisterFloat(int index, float val)
	{
		m_register[index].f = val;
	}
	std::vector<Ruff::Call> &getCalls()
	{
		return m_call;
	}
	Entity *&target()
	{
		return m_target;
	}

	struct CInput
	{
		float x;
		float y;
		float u;
		float v;
	};

	const CInput &input() const
	{
		return m_input;
	}
	void broadcastCall(Ruff::Call &c, const std::string &tag);
	void setTargetTag(const std::string &tag, const std::string &method);
	void onQueryEntityByTag(const Events::QueryEntityByTagEvent *);
private:

	static std::string m_tag;
	static CInput m_input;
	static const float m_axisDeadzone;
	static BehaviorComponent *m_currentComponent;

	void callDataSync(Ruff::Call &c);

	std::string m_luaModule;
	std::vector<Ruff::Call> m_call;
	Entity *m_target;
	std::vector<int> m_sceneDespawnData;

	// Behavior state for Lua
	union RegisterVal
	{
		float f;
		int i{ 0 };
	};
	static const int m_registerCount{ 20 };
	RegisterVal m_register[m_registerCount];

	// Persisting state for Lua return

	Ruff::Call m_curCall;
};


