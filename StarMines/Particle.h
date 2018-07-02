#pragma once

#include "ISystem.h"

class Particle : public ISystem
{
public:
	Particle(ComponentManager *cm) :
		ISystem{ cm }
	{}
	void update() override;
};