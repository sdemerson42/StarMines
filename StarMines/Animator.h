#pragma once

#include "ISystem.h"

class Animator : public ISystem
{
public:
	Animator(ComponentManager *cm) :
		ISystem{ cm }
	{}
	void update() override;
};
