#include "ComponentManager.h"
#include "Entity.h"

void ComponentManager::addComponent(Entity *e, const std::string &tag, const std::vector<std::string> &initArgs)
{
	if (tag == "render")
	{
		genAddComponent<RenderComponent>(e, initArgs, m_render, m_renderSz);
	}
	if (tag == "anim")
	{
		genAddComponent<AnimationComponent>(e, initArgs, m_anim, m_animSz);
	}
	if (tag == "physics")
	{
		genAddComponent<PhysicsComponent>(e, initArgs, m_physics, m_physicsSz);
	}
	if (tag == "behavior")
	{
		genAddComponent<BehaviorComponent>(e, initArgs, m_behavior, m_behaviorSz);
	}
}

void ComponentManager::deactivateComponent(Entity *e, const std::string &tag)
{
	if (tag == "render")
	{
		genDeactivateComponent<RenderComponent>(e, m_render, m_renderSz);
	}
	if (tag == "anim")
	{
		genDeactivateComponent<AnimationComponent>(e, m_anim, m_animSz);
	}
	if (tag == "physics")
	{
		genDeactivateComponent<PhysicsComponent>(e, m_physics, m_physicsSz);
	}
	if (tag == "behavior")
	{
		genDeactivateComponent<BehaviorComponent>(e, m_behavior, m_behaviorSz);
	}
}

void ComponentManager::activateComponent(Entity *e, const std::string &tag)
{
	if (tag == "render")
	{
		genActivateComponent<RenderComponent>(e, m_render, m_renderSz);
	}
	if (tag == "anim")
	{
		genActivateComponent<AnimationComponent>(e, m_anim, m_animSz);
	}
	if (tag == "physics")
	{
		genActivateComponent<PhysicsComponent>(e, m_physics, m_physicsSz);
	}
	if (tag == "behavior")
	{
		genActivateComponent<BehaviorComponent>(e, m_behavior, m_behaviorSz);
	}
}

void ComponentManager::removeComponent(Entity *e, const std::string &tag)
{
	if (tag == "render")
	{
		genRemoveComponent<RenderComponent>(e, m_render, m_renderSz);
	}
	if (tag == "anim")
	{
		genRemoveComponent<AnimationComponent>(e, m_anim, m_animSz);
	}
	if (tag == "physics")
	{
		genRemoveComponent<PhysicsComponent>(e, m_physics, m_physicsSz);
	}
	if (tag == "behavior")
	{
		genRemoveComponent<BehaviorComponent>(e, m_behavior, m_behaviorSz);
	}
}
