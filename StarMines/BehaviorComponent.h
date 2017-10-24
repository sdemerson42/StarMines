#pragma once

#include "IComponent.h"
#include "RuffVM.h"

class BehaviorComponent : public IComponent
{
	friend class Behavior;
public:
	void initialize(const std::vector<std::string> &input) override
	{
		m_vm.loadScript(input[0]);
	}
	void update()
	{
		m_vm.exec();
	}
private:
	Ruff::RuffVM m_vm;
};