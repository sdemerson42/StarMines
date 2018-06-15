#pragma once

#include "IComponent.h"
#include <string>
#include "Vector2.h"
#include "Entity.h"

class RenderComponent : public IComponent
{
	friend class Animator;
public:
	RenderComponent()
	{
	}
	void initialize(const std::vector<std::string> &input) override
	{
		int wl;
		int sl;
		initSerial(&m_textureName, Serializable::Type::STRING,
			&m_position.x, Serializable::Type::FLOAT,
			&m_position.y, Serializable::Type::FLOAT,
			&m_size.x, Serializable::Type::FLOAT,
			&m_size.y, Serializable::Type::FLOAT,
			&wl, Serializable::Type::INT,
			&sl, Serializable::Type::INT
		);
		readSerial(input);
		m_winLayer = (WindowLayer)(wl);
		m_sceneLayer = (SceneLayer)(sl);
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
	const std::string &getTag() const override
	{
		return m_tag;
	}

	enum class WindowLayer
	{
		MAIN, MENU, _SIZE
	};
	enum class SceneLayer
	{
		BACK, FORE, ACTOR, OVERLAY, _SIZE
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
	void setTexPosition(int x, int y)
	{
		m_position.x = x;
		m_position.y = y;
	}
private:
	static std::string m_tag;
	WindowLayer m_winLayer;
	SceneLayer m_sceneLayer;
	std::string m_textureName;
	Vector2 m_position;
	Vector2 m_size;
};
