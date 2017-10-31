#include "GameState.h"
#include "ReadData.h"

GameState::GameState() :
	m_compManager{ std::make_unique<ComponentManager>() },
	m_window{ sf::VideoMode{800,600}, "StarMines v0.1" }, m_factory{ this, "data\\blueprints.txt" }
{
	m_sys.emplace_back(std::make_unique<Animator>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Behavior>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Physics>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Renderer>(m_compManager.get(), m_window));

	// Test

	m_factory.createFromBlueprint("RedGuy");
	m_factory.createFromBlueprint("Lump", 190, 200);
	m_factory.createFromBlueprint("Lump", 450, 170);
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