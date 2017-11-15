#pragma once

#include "IComponent.h"
#include "RuffVM.h"
#include "Events.h"

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

	static void setInput(Events::JoystickEvent *evnt)
	{
		float x = evnt->x;
		float y = evnt->y;
		if (abs(x) < 20.0f) x = 0.0f;
		if (abs(y) < 20.0f) y = 0.0f;
		m_input.x = x;
		m_input.y = y;
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
	static Events::JoystickEvent m_input;

	Ruff::RuffVM m_vm;
	std::vector<Ruff::Call> m_call;
	Entity *m_curCaller;
	Entity *m_target;

	void broadcastCall(Ruff::Call &c, const std::string &tag);
	void setTargetTag(const std::string &tag, const std::string &method);
	void onQueryEntityByTag(const Events::QueryEntityByTagEvent *);
};