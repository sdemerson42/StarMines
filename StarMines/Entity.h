#pragma once

#include <vector>
#include <typeindex>
#include <algorithm>
#include <string>
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

	// Tag interface

	void addTag(const std::string &tag)
	{
		if (!findTag(tag))
			m_tag.emplace_back(tag);
	}
	void removeTag(const std::string &tag)
	{
		auto p = std::find(begin(m_tag), end(m_tag), tag);
		if (p != end(m_tag))
			m_tag.erase(p);
	}
	bool findTag(const std::string &tag)
	{
		auto p = std::find(begin(m_tag), end(m_tag), tag);
		if (p != end(m_tag))
			return true;
		return false;
	}
	const std::string &name() const
	{
		return m_tag[0];
	}

	// Active

	void setActive(bool b)
	{
		m_active = b;
	}
	bool active() const
	{
		return m_active;
	}

	// Persistence

	enum class PersistType
	{
		Scene, Global, None 
	};
	void setPersist(PersistType p)
	{
		m_persist = p;
	}
	PersistType persist() const
	{
		return m_persist;
	}

private:
	std::vector<IComponent *> m_compRef;
	Vector2 m_position;
	std::vector<std::string> m_tag;
	bool m_active{ false };
	PersistType m_persist{ PersistType::None };
};

