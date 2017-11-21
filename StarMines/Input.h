#pragma once

#include "ISystem.h"
#include "Events.h"

class Input : public ISystem
{
public:
	Input(ComponentManager *cm) :
		ISystem{ cm }
	{}
	void update() override;
};
