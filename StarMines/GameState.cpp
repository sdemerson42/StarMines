#include "GameState.h"
#include "ReadData.h"
#include <algorithm>
#include "BehaviorComponent.h"

GameState::GameState() :
	m_compManager{ std::make_unique<ComponentManager>() },
	m_window{ sf::VideoMode{800,600}, "StarMines v0.1" }, m_factory{ this, "data\\blueprints.txt" }
{
	m_sys.emplace_back(std::make_unique<Animator>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Behavior>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Physics>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Renderer>(m_compManager.get(), m_window));
	m_sys.emplace_back(std::make_unique<Spawner>(m_compManager.get(), &m_factory));

	// Events

	registerFunc(this, &GameState::onRSCall);

	// Test Data

	loadTestData("Data\\TestData.txt");

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

void GameState::onRSCall(const Events::RSCallEvent *evnt)
{
	std::string tag{ evnt->tag };
	Ruff::Call call = evnt->call;
	for (auto &spe : m_entity)
	{
		if (spe->findTag(tag))
		{
			auto c = spe->getComponent<BehaviorComponent>();
			if (c)
				c->addCall(call);
		}
	}
}

void GameState::loadTestData(const std::string &fName)
{
	std::ifstream ifs{ fName };
	std::string b;
	while (ifs >> b)
	{
		float x, y;
		ifs >> x >> y;
		Events::SpawnDataEvent evnt{ b, x, y };
		broadcast(&evnt);
	}
}