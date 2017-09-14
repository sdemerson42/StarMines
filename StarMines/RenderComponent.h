#pragma once

#include "IComponent.h"
#include "AutoList.h"
#include <string>
#include "Vector2.h"
#include "Entity.h"

class RenderComponent : public IComponent, public AutoList<RenderComponent>
{
public:
	RenderComponent(Entity *parent) :
		IComponent{ parent }
	{}
	void initialize(const std::vector<std::string> &input) override
	{

	}
	enum class WindowLayer
	{
		MAIN, OVERLAY, _SIZE
	};
	enum class SceneLayer
	{
		BACK, FORE, ACTOR, _SIZE
	};

	WindowLayer winLayer() const
	{
		return m_winLayer;
	}
	SceneLayer sceneLayer() const
	{
		return m_sceneLayer;
	}
	std::string texName() const
	{
		return m_textureName;
	}
	Vector2 position() const
	{
		return m_position;
	}
	Vector2 size() const
	{
		return m_size;
	}
//private:
	WindowLayer m_winLayer;
	SceneLayer m_sceneLayer;
	std::string m_textureName;
	Vector2 m_position;
	Vector2 m_size;
};
