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
		m_pName = input[0].c_str();
	}
	void update()
	{
		// Set current component reference
		m_currentComponent = this;

		// Prepare local state
		ccDelCount = 0;

		// Local state cleanup
		while (ccDelCount > 0)
		{
			m_call.erase(begin(m_call));
			--ccDelCount;
		}
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
	Ruff::CCall curCCall;
	int ccDelCount;
private:
	static std::string m_tag;
	static CInput m_input;
	static const float m_axisDeadzone;
	static BehaviorComponent *m_currentComponent;
	//static std::map<std::string, Ruff::ByteCode> m_codeMap;

	//Ruff::RuffVM m_vm;
	const char *m_pName;
	std::vector<Ruff::Call> m_call;
	Entity *m_target;
	std::vector<int> m_sceneDespawnData;

	// Behavior state for Python
	union RegisterVal
	{
		float f;
		int i{ 0 };
	};
	static const int m_pyRegisterCount{ 20 };
	RegisterVal m_pyRegister[m_pyRegisterCount];
};


