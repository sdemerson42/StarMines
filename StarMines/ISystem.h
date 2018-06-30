#pragma once

#include "EventSystem.h"
#include <iostream>

class ComponentManager;

class ISystem : public EventHandler
{
public:
	ISystem(ComponentManager *cm) :
		m_compManager{ cm }
	{}
	virtual ~ISystem()
	{
		std::cout << "ISystem at " << this << " deleted...\n";
	}
	virtual void update() = 0;
protected:
	ComponentManager *m_compManager;
};
