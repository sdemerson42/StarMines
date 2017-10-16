#pragma once

#include <vector>
#include <typeindex>
#include <algorithm>
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

	template<typename T>
	T *getComponent()
	{
		std::type_index ti{ typeid(T) };
		auto p = std::find_if(begin(m_compRef), end(m_compRef), [&](auto cp)
		{
			return std::type_index{ typeid(*cp) } == ti;
		});
		if (p != end(m_compRef))
			return static_cast<T*>(*p);
		return nullptr;
	}

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

