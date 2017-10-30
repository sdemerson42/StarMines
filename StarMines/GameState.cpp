#include "GameState.h"
#include "ReadData.h"

GameState::GameState() :
	m_compManager{ std::make_unique<ComponentManager>() },
	m_window{ sf::VideoMode{800,600}, "StarMines v0.1" }
{
	m_sys.emplace_back(std::make_unique<Animator>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Behavior>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Physics>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Renderer>(m_compManager.get(), m_window));

	// Test Data

	m_entity.emplace_back(std::make_unique<Entity>());
	auto e = m_entity[0].get();
	m_compManager->addComponent(e, "render", readData::loadStringData("data\\data.txt"));
	m_compManager->addComponent(e, "physics", readData::loadStringData("data\\data3.txt"));
	m_compManager->addComponent(e, "anim", readData::loadStringData("data\\data2.txt"));
	m_compManager->addComponent(e, "behavior", readData::loadStringData("data\\data4.txt"));
	e->getComponent<AnimationComponent>()->play("right");
};

void GameState::exec()
{
	while (m_window.isOpen())
	{
		sf::Event evnt;
		while (m_window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
			{
				m_window.close();
				return;
			}
		}
		if (m_clock.getElapsedTime().asMilliseconds() > 100 / 6)
		{
			m_clock.restart();
			for (auto &p : m_sys)
				p->update();
		}
	}
}