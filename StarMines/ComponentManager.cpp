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
}
