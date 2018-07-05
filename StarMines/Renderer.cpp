#include "Renderer.h"
#include "RenderComponent.h"
#include "ParticleComponent.h"
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

	for (int i = 0; i < m_compManager->m_particleSz; ++i)
		addParticleQuad(m_compManager->m_particle[i]);
}

void Renderer::addQuad(RenderComponent &rc)
{
	int index = int(rc.winLayer()) * int(RenderComponent::SceneLayer::_SIZE) + int(rc.sceneLayer());
	auto &map = m_drawLayer[index].vaMap;
	auto val = map.find(rc.texName());
	if (val == end(map))
	{
		map[rc.texName()] = std::vector<VALayer>{};
		val = map.find(rc.texName());
		val->second.push_back(VALayer{});
		val->second.back().va.setPrimitiveType(sf::Quads);
	}

	// Default VertexArray 0 is untransformed

	sf::VertexArray *va = &val->second[0].va;
	
	float tx = rc.position().x;
	float ty = rc.position().y;
	float w = rc.size().x;
	float h = rc.size().y;
	float x = rc.parent()->position().x;
	float y = rc.parent()->position().y;

	// Transformed?

	if (rc.isTransformed())
	{
		val->second.push_back(VALayer{});
		VALayer &layer = val->second.back();
		layer.isTransformed = true;
		sf::Vector2f center{ x + w / 2, y + h / 2 };
		layer.transform.rotate(rc.rotation(), center);
		layer.transform.scale(rc.scale(), center);
		layer.va.setPrimitiveType(sf::Quads);
		va = &layer.va;
	}

	sf::Color color{ (sf::Uint8)rc.m_color.r, (sf::Uint8)rc.m_color.g,
		(sf::Uint8)rc.m_color.b, (sf::Uint8)rc.m_color.a };
	va->append(sf::Vertex(sf::Vector2f{ x, y }, color, sf::Vector2f{ tx, ty }));
	va->append(sf::Vertex(sf::Vector2f{ x + w, y }, color, sf::Vector2f{ tx + w, ty }));
	va->append(sf::Vertex(sf::Vector2f{ x + w, y + h }, color, sf::Vector2f{ tx + w, ty + h }));
	va->append(sf::Vertex(sf::Vector2f{ x, y + h }, color, sf::Vector2f{ tx, ty + h }));
}

void Renderer::addParticleQuad(ParticleComponent &pc)
{
	
	int index = int(pc.winLayer()) * int(RenderComponent::SceneLayer::_SIZE) + int(pc.sceneLayer());
	auto &map = m_drawLayer[index].vaMap;
	auto val = map.find(pc.texName());
	if (val == end(map))
	{
		map[pc.texName()] = std::vector<VALayer>{};
		val = map.find(pc.texName());
		val->second.push_back(VALayer{});
		val->second.back().va.setPrimitiveType(sf::Quads);
	}

	// Particle transformation is presently unsupported. Place
	// particles in default array.

	sf::VertexArray &va = val->second[0].va;
	
	for (ParticleComponent::ParticleData &pd : pc.m_particleData)
	{
		if (!pd.active)
			break;

		float tx = pc.position().x;
		float ty = pc.position().y;
		float w = pc.size().x;
		float h = pc.size().y;
		float x = pd.position.x;
		float y = pd.position.y;

		sf::Color color{ (sf::Uint8)pc.m_color.r, (sf::Uint8)pc.m_color.g, 
			(sf::Uint8)pc.m_color.b, (sf::Uint8)pc.m_color.a };
		va.append(sf::Vertex(sf::Vector2f{ x, y }, color, sf::Vector2f{ tx, ty }));
		va.append(sf::Vertex(sf::Vector2f{ x + w, y }, color, sf::Vector2f{ tx + w, ty }));
		va.append(sf::Vertex(sf::Vector2f{ x + w, y + h }, color, sf::Vector2f{ tx + w, ty + h }));
		va.append(sf::Vertex(sf::Vector2f{ x, y + h }, color, sf::Vector2f{ tx, ty + h }));
	}
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

			for (auto &val : vaMap.second)
			{
				if (val.va.getVertexCount() > 0)
				{
					sf::RenderStates states{ &tp->second };
					if (val.isTransformed)
						states.transform = val.transform;

					m_window.draw(val.va, states);
				}
			}
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

