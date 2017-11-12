#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
#include "Entity.h"
#include "Events.h"

std::string BehaviorComponent::m_tag{ "behavior" };

void BehaviorComponent::resetVM()
{
	m_vm.reset();
}

void BehaviorComponent::broadcastCall(Ruff::Call &c, const std::string &tag)
{
	Events::RSCallEvent rsc;
	rsc.call = c;
	rsc.tag = tag;
	broadcast(&rsc);
}