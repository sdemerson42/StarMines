#include "BehaviorComponent.h"
#include "PhysicsComponent.h"
#include "Entity.h"
#include "Events.h"

std::string BehaviorComponent::m_tag{ "behavior" };
Events::JoystickEvent BehaviorComponent::m_input{ 0,0 };

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

void BehaviorComponent::setTargetTag(const std::string &tag, const std::string &method)
{
	Events::QueryEntityByTagEvent q{ this, tag, method };
	broadcast(&q);
}
void BehaviorComponent::onQueryEntityByTag(const Events::QueryEntityByTagEvent *evnt)
{
	if (evnt->client == this)
		m_target = evnt->response;
}
