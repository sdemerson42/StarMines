#pragma once

#include "ISystem.h"
#include "Vector2.h"

class Behavior : public ISystem
{
public:
	Behavior(ComponentManager *cm);
	void update() override;
private:
	void onInputEvent(Events::InputEvent *);
};
