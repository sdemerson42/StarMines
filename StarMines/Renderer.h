#pragma once

#include "ISystem.h"

#include "SFML/Graphics.hpp"
#include <vector>
#include <map>

class RenderComponent;

class Renderer : ISystem
{
public:
	Renderer(ComponentManager *cm, sf::RenderWindow &rw) :
		ISystem{ cm }, m_window { rw }
	{}
	void update();
private:
	void fillDrawLayer();
	void addQuad(RenderComponent &rc);
	void render();
	struct DrawLayer
	{
		int index;
		std::map<std::string, sf::VertexArray> vaMap;
	};
	std::vector<DrawLayer> m_drawLayer;
	sf::RenderWindow &m_window;
	std::map<std::string, sf::Texture> m_textureMap;
};
