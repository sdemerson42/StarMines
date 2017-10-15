#include "Renderer.h"
#include "RenderComponent.h"
#include "Entity.h"
#include "ComponentManager.h"

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
	m_window.clear(sf::Color::Black);
	
	for (auto &dl : m_drawLayer)
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

	m_window.display();
}

