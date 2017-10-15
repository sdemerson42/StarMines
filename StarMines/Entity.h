#pragma once

#include <vector>
#include "IComponent.h"
#include "Vector2.h"

class ComponentManager;

// Entity - Main game object class. Contains position state and
// a vector of components that define behavior.
class Entity
{
	friend class ComponentManager;
public:
	Entity() = default;
	~Entity() = default;

	// Component Reference interface


	// Position interface

	void setPosition(float x, float y)
	{
		m_position.x = x;
		m_position.y = y;
	}
	void addPosition(float x, float y)
	{
		m_position.x += x;
		m_position.y += y;
	}
	const Vector2 &position() const
	{
		return m_position;
	}
private:
	std::vector<IComponent *> m_compRef;
	Vector2 m_position;
};

