#pragma once

#include <vector>
#include <string>
#include "Serializable.h"

class Entity;

class IComponent : public Serializable
{
public:
	IComponent(Entity *parent);
	virtual ~IComponent()
	{}
	virtual void initialize(const std::vector<std::string> &input)
	{}

	bool active() const
	{
		return m_active;
	}
	void setActive(bool b)
	{
		m_active = b;
	}
	Entity *parent();
private:
	Entity *m_parent;
	bool m_active{ true };
};
