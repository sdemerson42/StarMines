#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Serializable.h"
#include "EventSystem.h"

class Entity;

class IComponent : public Serializable, public EventHandler
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
	virtual std::string &getTag() const = 0;

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
		m_parent.reset(parent);
	}
	Entity *parent();
private:
	std::shared_ptr<Entity> m_parent;
	bool m_active{ true };
};
