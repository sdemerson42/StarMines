#pragma once

#include "IComponent.h"
#include "RuffVM.h"

class Entity;

class BehaviorComponent : public IComponent
{
	friend class Behavior;
	friend class Ruff::RuffVM;
public:
	BehaviorComponent() :
		m_vm{ this },
		m_curCaller{ nullptr }
	{}
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

	void resetVM();

	void addCall(Ruff::Call& c)
	{
		m_call.emplace_back(c);
	}

private:
	static std::string m_tag;
	Ruff::RuffVM m_vm;
	std::vector<Ruff::Call> m_call;
	Entity *m_curCaller;

	void broadcastCall(Ruff::Call &c, const std::string &tag);
};