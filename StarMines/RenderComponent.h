#pragma once

#include "IComponent.h"
#include "AutoList.h"
#include <string>
#include "Vector2.h"
#include "Entity.h"

class RenderComponent : public IComponent, public AutoList<RenderComponent>
{
public:
	RenderComponent()
	{
	}
	void initialize(const std::vector<std::string> &input) override
	{
		initSerial(&m_textureName, Serializable::Type::STRING,
			&m_position.x, Serializable::Type::FLOAT,
			&m_position.y, Serializable::Type::FLOAT,
			&m_size.x, Serializable::Type::FLOAT,
			&m_size.y, Serializable::Type::FLOAT);
		readSerial(input);
		setWinLayer(WindowLayer::MAIN);
		setSceneLayer(SceneLayer::ACTOR);
	}
	void writeOut(std::vector<std::string> &output) override
	{
		initSerial(&m_textureName, Serializable::Type::STRING,
			&m_position.x, Serializable::Type::FLOAT,
			&m_position.y, Serializable::Type::FLOAT,
			&m_size.x, Serializable::Type::FLOAT,
			&m_size.y, Serializable::Type::FLOAT);
		writeSerial(output);
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
	void setWinLayer(WindowLayer layer)
	{
		m_winLayer = layer;
	}
	SceneLayer sceneLayer() const
	{
		return m_sceneLayer;
	}
	void setSceneLayer(SceneLayer layer)
	{
		m_sceneLayer = layer;
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
private:
	WindowLayer m_winLayer;
	SceneLayer m_sceneLayer;
	std::string m_textureName;
	Vector2 m_position;
	Vector2 m_size;
};
