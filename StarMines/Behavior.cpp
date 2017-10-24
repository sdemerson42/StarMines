#include "Behavior.h"
#include "BehaviorComponent.h"
#include "ComponentManager.h"

void Behavior::update()
{
	for (auto &c : m_compManager->m_behavior)
	{
		c.update();
	}
}