#pragma once

#include <vector>
#include <string>
#include "Serializable.h"

class Entity;

class IComponent : public Serializable
{
public:
	IComponent() : m_parent{ nullptr }
	{}
	IComponent(Entity *parent);
	virtual ~IComponent()
	{}
	virtual void initialize(const std::vector<std::string> &input)
	{}
	virtual void writeOut(std::vector<std::string> &output)
	{}

	bool active() const
	{
		return m_active;
	}
	void setActive(bool b)
	{
		m_active = b;
	}
	void setParent(Entity *parent)
	{
		m_parent = parent;
	}
	Entity *parent();
private:
	Entity *m_parent;
	bool m_active{ true };
};
