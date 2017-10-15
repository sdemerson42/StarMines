#include "ComponentManager.h"
#include "Entity.h"

void ComponentManager::addComponent(Entity *e, const std::string &tag, const std::vector<std::string> &initArgs)
{
	if (tag == "render")
	{
		genAddComponent<RenderComponent>(e, initArgs, m_render, m_renderSz);
	}
}

void ComponentManager::deactivateComponent(Entity *e, const::string &tag)
{
	if (tag == "render")
	{
		genDeactivateComponent<RenderComponent>(e, m_render, m_renderSz);
	}
}

void ComponentManager::activateComponent(Entity *e, const::string &tag)
{
	if (tag == "render")
	{
		genActivateComponent<RenderComponent>(e, m_render, m_renderSz);
	}
}

void ComponentManager::removeComponent(Entity *e, const::string &tag)
{
	if (tag == "render")
	{
		genRemoveComponent<RenderComponent>(e, m_render, m_renderSz);
	}
}
