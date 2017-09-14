#include "IComponent.h"
#include "Entity.h"

IComponent::IComponent(Entity *parent) :
	m_parent{ parent }
{}

Entity *IComponent::parent()
{
	return m_parent;
}