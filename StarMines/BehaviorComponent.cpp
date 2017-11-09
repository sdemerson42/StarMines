#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
#include "Entity.h"
#include "Events.h"

void BehaviorComponent::sendCall(Ruff::Call &c)
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

void BehaviorComponent::broadcastCall(Ruff::Call &c, const std::string &tag)
{
	Events::RSCallEvent rsc;
	rsc.call = c;
	rsc.tag = tag;
	broadcast(&rsc);
}