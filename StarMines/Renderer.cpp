#include "Renderer.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Entity.h"
#include "ComponentManager.h"
#include <iostream>
#include "SFML\System.hpp"

sf::View Renderer::m_view;

void Renderer::update()
{
	fillDrawLayer();
	render();
}

void Renderer::fillDrawLayer()
{
	m_drawLayer.clear();
	m_drawLayer.resize(int(RenderComponent::SceneLayer::_SIZE) * int(RenderComponent::WindowLayer::_SIZE));
	for (int i = 0; i < m_compManager->m_renderSz; ++i)
		addQuad(m_compManager->m_render[i]);
}

void Renderer::addQuad(RenderComponent &rc)
{
	int index = int(rc.winLayer()) * int(RenderComponent::SceneLayer::_SIZE) + int(rc.sceneLayer());
	auto &map = m_drawLayer[index].vaMap;
	auto va = map.find(rc.texName());
	if (va == end(map))
	{
		map[rc.texName()] = sf::VertexArray{};
		va = map.find(rc.texName());
		va->second.setPrimitiveType(sf::Quads);
	}

	float tx = rc.position().x;
	float ty = rc.position().y;
	float w = rc.size().x;
	float h = rc.size().y;
	float x = rc.parent()->position().x;
	float y = rc.parent()->position().y;

	va->second.append(sf::Vertex(sf::Vector2f{ x, y }, sf::Vector2f{ tx, ty }));
	va->second.append(sf::Vertex(sf::Vector2f{ x + w, y }, sf::Vector2f{ tx + w, ty }));
	va->second.append(sf::Vertex(sf::Vector2f{ x + w, y + h }, sf::Vector2f{ tx + w, ty + h }));
	va->second.append(sf::Vertex(sf::Vector2f{ x, y + h }, sf::Vector2f{ tx, ty + h }));
}

void Renderer::render()
{
	m_window.clear();

	// Draw all vertex arrays

	for (auto &dl : m_drawLayer)
	{
		auto index = &dl - &m_drawLayer[0];
		// View settings
		// Begin with default view...
		// Change to scene view after rendering static backgrounds...
		if (index == 1)
			m_window.setView(m_view);
		// Change back to default for UI layer...
		if (index == int(RenderComponent::SceneLayer::_SIZE))
			m_window.setView(m_window.getDefaultView());
		for (auto &vaMap : dl.vaMap)
		{
			auto tp = m_textureMap.find(vaMap.first);
			if (tp == end(m_textureMap))
			{
				m_textureMap[vaMap.first].loadFromFile(vaMap.first);
				tp = m_textureMap.find(vaMap.first);
			}

			m_window.draw(vaMap.second, sf::RenderStates{ &tp->second });
		}
	}

	// Draw text objects

	for (unsigned int i{ 0 }; i < m_compManager->m_textSz; ++i)
	{
		auto &tc = m_compManager->m_text[i];
		tc.m_text.setPosition(tc.parent()->position().x, tc.parent()->position().y);
		m_window.draw(tc.m_text);
	}

	m_window.display();
}

const Vector2 &Renderer::viewCenter()
{
	return Vector2{ m_view.getCenter().x, m_view.getCenter().y };
}
void Renderer::setViewCenter(float x, float y)
{
	m_view.setCenter(x, y);
}

void Renderer::onView(const Events::ViewEvent *evnt)
{
	m_view.setSize(evnt->viewW, evnt->viewH);
	m_view.setViewport(sf::FloatRect{ evnt->portX, evnt->portY, evnt->portW, evnt->portH });
	m_view.setCenter(evnt->centerX, evnt->centerY);
}

