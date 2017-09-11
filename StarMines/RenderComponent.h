#pragma once

#include "IComponent.h"
#include "AutoList.h"
#include <string>
#include "Vector2.h"

class RenderComponent : public IComponent, public AutoList<RenderComponent>
{
public:
	enum class WindowLayer
	{
		MAIN, OVERLAY
	};
	enum class SceneLayer
	{
		BACK, FORE, ACTOR
	};

//private:
	int m_winLayer;
	int m_sceneLayer;
	std::string m_textureName;
	Vector2 m_position;
	Vector2 m_size;
};
