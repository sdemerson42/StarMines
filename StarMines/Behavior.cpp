#include "Behavior.h"
#include "BehaviorComponent.h"
#include "ComponentManager.h"

void Behavior::update()
{
	int sz = m_compManager->m_behaviorSz;
	for (int i = 0; i < sz; ++i)
		m_compManager->m_behavior[i].update();
}

void Behavior::onJoystickEvent(Events::JoystickEvent *evnt)
{
	BehaviorComponent::setInput(evnt);
}