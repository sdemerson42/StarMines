#pragma once

#include "ISystem.h"

class Behavior : public ISystem
{
public:
	Behavior(ComponentManager *cm) :
		ISystem{ cm }
	{}
	void update() override;
private:

};
