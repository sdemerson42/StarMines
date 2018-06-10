#pragma once

#include "IComponent.h"
#include "Events.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "RuffCommon.h"
#include "Python.h"

class Entity;

class BehaviorComponent : public IComponent
{
	friend class Behavior;
public:
	BehaviorComponent() :
		m_curCaller{ nullptr },
		m_target{ nullptr }
	{
		registerFunc(this, &BehaviorComponent::onQueryEntityByTag);
		m_call.reserve(64);
	}

	static void setInput(Events::InputEvent *evnt)
	{
		m_input = *evnt;
		if (abs(m_input.xAxis) < m_axisDeadzone) m_input.xAxis = 0.0f;
		if (abs(m_input.yAxis) < m_axisDeadzone) m_input.yAxis = 0.0f;
		if (abs(m_input.uAxis) < m_axisDeadzone) m_input.uAxis = 0.0f;
		if (abs(m_input.vAxis) < m_axisDeadzone) m_input.vAxis = 0.0f;
	}

	void initialize(const std::vector<std::string> &input) override
	{
		m_pName = input[0].c_str();
	}
	void update()
	{
		// Set current component reference
		m_currentComponent = this;

		// Execute Python script
		auto f{ _Py_fopen(m_pName, "r") };
		PyRun_SimpleFile(f, m_pName);
		fclose(f);

		// Local state cleanup
		m_call.clear();
		callIndex = 0;
	}
	std::string &getTag() const override
	{
		return m_tag;
	}
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

	// Behavior state access methods

	static BehaviorComponent *getCurrentComponent()
	{
		return m_currentComponent;
	}

	int getRegisterInt(int index) const
	{
		return m_pyRegister[index].i;
	}
	float getRegisterFloat(int index) const
	{
		return m_pyRegister[index].f;
	}
	void setRegisterInt(int index, int val)
	{
		m_pyRegister[index].i = val;
	}
	void setRegisterFloat(int index, float val)
	{
		m_pyRegister[index].f = val;
	}
	Entity *getCaller()
	{
		return m_curCaller;
	}
	void setCaller(Entity *e)
	{
		m_curCaller = e;
	}
	const std::vector<Ruff::Call> &getCalls()
	{
		return m_call;
	}
	void broadcastCall(Ruff::Call &c, const std::string &tag);
	void setTargetTag(const std::string &tag, const std::string &method);
	void onQueryEntityByTag(const Events::QueryEntityByTagEvent *);
	Ruff::CCall curCCall;
	int callIndex{ 0 };
private:
	static std::string m_tag;
	static Events::InputEvent m_input;
	static const float m_axisDeadzone;
	static BehaviorComponent *m_currentComponent;
	//static std::map<std::string, Ruff::ByteCode> m_codeMap;

	//Ruff::RuffVM m_vm;
	const char *m_pName;
	std::vector<Ruff::Call> m_call;
	Entity *m_curCaller{ nullptr };
	Entity *m_target;
	std::vector<int> m_sceneDespawnData;

	// Behavior state for Python
	union RegisterVal
	{
		float f;
		int i{ 0 };
	};
	static const int m_pyRegisterCount{ 10 };
	RegisterVal m_pyRegister[m_pyRegisterCount];
};


