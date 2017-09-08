#pragma once

#include "IComponent.h"
#include "AutoList.h"

class RenderComponent : public IComponent, public AutoList<RenderComponent>
{
	// Challenge: Flexible method of assigning RCs to layers, to be sorted and handled by the Renderer.
public:
private:
};
