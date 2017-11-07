#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
#include "Entity.h"

void BehaviorComponent::sendCall(Call &c)
{
	auto p = m_curCaller->getComponent<BehaviorComponent>();
	if (p)
		p->addCall(c);
}

void BehaviorComponent::setDir(float x, float y)
{
	auto p = parent()->getComponent<PhysicsComponent>();
	if (p)
		p->setDir(x, y);
}