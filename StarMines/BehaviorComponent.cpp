#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
#include "Entity.h"
#include "Events.h"


void BehaviorComponent::broadcastCall(Ruff::Call &c, const std::string &tag)
{
	Events::RSCallEvent rsc;
	rsc.call = c;
	rsc.tag = tag;
	broadcast(&rsc);
}