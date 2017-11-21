#pragma once

#include "IComponent.h"
#include "RuffVM.h"
#include "Events.h"
#include <iostream>

class Entity;

class BehaviorComponent : public IComponent
{
	friend class Behavior;
	friend class Ruff::RuffVM;
public:
	BehaviorComponent() :
		m_vm{ this },
		m_curCaller{ nullptr },
		m_target{ nullptr }
	{
		registerFunc(this, &BehaviorComponent::onQueryEntityByTag);
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
		m_vm.loadScript(input[0]);
	}
	void update()
	{
		m_vm.update();
	}
	std::string &getTag() const override
	{
		return m_tag;
	}
	void onMove()
	{
		m_vm.setParent(this);
	}
	void resetVM();

	void addCall(Ruff::Call& c)
	{
		m_call.emplace_back(c);
	}

private:
	static std::string m_tag;
	static Events::InputEvent m_input;
	static const float m_axisDeadzone;

	Ruff::RuffVM m_vm;
	std::vector<Ruff::Call> m_call;
	Entity *m_curCaller;
	Entity *m_target;

	void broadcastCall(Ruff::Call &c, const std::string &tag);
	void setTargetTag(const std::string &tag, const std::string &method);
	void onQueryEntityByTag(const Events::QueryEntityByTagEvent *);
};