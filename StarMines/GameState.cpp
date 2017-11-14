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
	registerFunc(this, &GameState::onQueryEntityByTag);

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

void GameState::onQueryEntityByTag(Events::QueryEntityByTagEvent *evnt)
{
	std::string tag{ evnt->tag };
	std::string method{ evnt->method };
	if (method == "first")
	{
		auto e = std::find_if(begin(m_entity), end(m_entity), [&](std::shared_ptr<Entity> &up)
		{
			return up->active() && up->findTag(tag);
		});
		if (e != end(m_entity))
		{
			evnt->response = e->get();
		}
	}
	if (method == "near")
	{
		Entity *e{ nullptr };
		float ds{ 0.0f };
		auto c = static_cast<BehaviorComponent *>(evnt->client);
		float sx = c->parent()->position().x;
		float sy = c->parent()->position().y;
		for (auto &spe : m_entity)
		{
			if (!spe->active())
				continue;
			if (spe->findTag(evnt->tag))
			{
				if (!e)
				{
					e = spe.get();
					ds = pow(sx - spe->position().x, 2) + pow(sy - spe->position().y, 2);
				}
				else
				{
					float lds = pow(sx - spe->position().x, 2) + pow(sy - spe->position().y, 2);
					if (lds < ds)
					{
						ds = lds;
						e = spe.get();
					}
				}
			}
		}
		evnt->response = e;
	}
	broadcast(evnt);
}

void GameState::loadTestData(const std::string &fName)
{
	std::ifstream ifs{ fName };
	std::string b;
	while (ifs >> b)
	{
		float x, y;
		bool cache;
		ifs >> x >> y >> cache;
		m_factory.createFromBlueprint(b, x, y, cache);
		
	}
}