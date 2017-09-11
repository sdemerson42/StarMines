#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <typeindex>
#include "IComponent.h"
#include "Vector2.h"


// Entity - Main game object class. Contains position state and
// a vector of components that define behavoir.
class Entity
{
public:
	Entity() = default;
	~Entity() = default;

	template<typename C>
	void addComponent()
	{
		m_component.emplace_back(std::make_shared<C>());
	}

	template<typename C>
	void addComponent(const std::vector<std::string> &args)
	{
		m_component.emplace_back(std::make_shared<C>());
		auto p = end(m_component) - 1;
		p->get()->initialize(args);
	}
	template<typename C>
	C* getComponent()
	{
		auto p = std::find_if(begin(m_component), end(m_component), [] (auto &sp)
		{
			auto ti = std::type_index(typeid(*sp.get()));
			return ti == std::type_index(typeid(C));
		});
		if (p == end(m_component)) return nullptr;
		return static_cast<C*>(p->get());
	}

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
	const Vector2 &getPosition() const
	{
		return m_position;
	}
private:
	std::vector<std::shared_ptr<IComponent>> m_component;
	Vector2 m_position;
};

