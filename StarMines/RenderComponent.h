#pragma once

#include "IComponent.h"
#include "ColorState.h"
#include <string>
#include "Vector2.h"
#include "Entity.h"
#include "SFML\Graphics.hpp"


class RenderComponent : public IComponent, public ColorState
{
	friend class Animator;
	friend class Renderer;
public:
	RenderComponent()
	{
	}
	virtual void initialize(const std::vector<std::string> &input) override
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
		
		reactivate();
	}
	virtual void writeOut(std::vector<std::string> &output) override
	{
		initSerial(&m_textureName, Serializable::Type::STRING,
			&m_position.x, Serializable::Type::FLOAT,
			&m_position.y, Serializable::Type::FLOAT,
			&m_size.x, Serializable::Type::FLOAT,
			&m_size.y, Serializable::Type::FLOAT);
		writeSerial(output);
	}
	virtual const std::string &getTag() const override
	{
		return m_tag;
	}
	virtual void reactivate() override
	{
		m_transformed = false;
		m_rotation = 0.0f;
		m_scale = sf::Vector2f{ 1.0f, 1.0f };
		m_color.r = 255;
		m_color.g = 255;
		m_color.b = 255;
		m_color.a = 255;
	}

	enum class WindowLayer
	{
		MAIN, MENU, _SIZE
	};
	enum class SceneLayer
	{
		STATIC_BACK, BACK, FORE, ACTOR, OVERLAY, _SIZE
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
	void rotate(float angle)
	{
		m_rotation = angle;
		if (m_scale.x == 1.0f && m_scale.y == 1.0f && m_rotation == 0.0f)
			m_transformed = false;
		else
			m_transformed = true;
	}
	float rotation() const
	{
		return m_rotation;
	}
	void setScale(float x, float y)
	{
		m_scale.x = x;
		m_scale.y = y;
		if (x == 1.0f && y == 1.0f && m_rotation == 0.0f)
			m_transformed = false;
		else
			m_transformed = true;
	}
	const sf::Vector2f &scale() const
	{
		return m_scale;
	}

	bool isTransformed() const
	{
		return m_transformed;
	}
private:
	static std::string m_tag;
	WindowLayer m_winLayer;
	SceneLayer m_sceneLayer;
	std::string m_textureName;
	Vector2 m_position;
	Vector2 m_size;
	
	// Transform state

	bool m_transformed;
	float m_rotation;
	sf::Vector2f m_scale;
};
