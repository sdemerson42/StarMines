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
		m_curCollider{ nullptr }
	{}
	void initialize(const std::vector<std::string> &input) override
	{
		m_vm.loadScript(input[0]);
	}
	void update()
	{
		m_vm.update();
	}

	void addCollider(Entity *e)
	{
		m_collider.emplace_back(e);
	}

private:
	Ruff::RuffVM m_vm;
	std::vector<Entity *> m_collider;
	Entity *m_curCollider;
};