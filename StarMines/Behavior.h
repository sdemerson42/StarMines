#pragma once

#include "ISystem.h"
#include "Vector2.h"

class Behavior : public ISystem
{
public:
	Behavior(ComponentManager *cm) :
		ISystem{ cm }
	{
		registerFunc(this, &Behavior::onJoystickEvent);
	}
	void update() override;
private:
	void onJoystickEvent(Events::JoystickEvent *);
};
