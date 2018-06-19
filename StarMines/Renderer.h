#pragma once

#include "ISystem.h"

#include "SFML/Graphics.hpp"
#include <vector>
#include <map>

class RenderComponent;

class Renderer : public ISystem
{
public:
	Renderer(ComponentManager *cm, sf::RenderWindow &rw) :
		ISystem{ cm }, m_window { rw }
	{
		registerFunc(this, &Renderer::onView);
	}
	void update() override;
	static const Vector2 &viewCenter();
	static void setViewCenter(float x, float y);
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
	static sf::View m_view;

	void onView(const Events::ViewEvent *evnt);
};
