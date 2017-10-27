#pragma once

#include "EventSystem.h"

class ComponentManager;

class ISystem : public EventHandler
{
public:
	ISystem(ComponentManager *cm) :
		m_compManager{ cm }
	{}
	virtual ~ISystem()
	{}
	virtual void update() = 0;
protected:
	ComponentManager *m_compManager;
};
