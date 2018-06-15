#include "GameState.h"
#include "ReadData.h"
#include <algorithm>
#include "BehaviorComponent.h"
#include <iostream>
#include "LuaWrapper.h"

GameState::GameState() :
	m_compManager{ std::make_unique<ComponentManager>() },
	m_window{ sf::VideoMode{800,600}, "StarMines v0.1" }, m_factory{ this, "data\\blueprints.txt" }, m_sceneChange{ false }
{
	//m_window.setVerticalSyncEnabled(true);

	// Prepare LuaWrapper

	LuaWrapper::L = luabridge::luaL_newstate();
	luaL_openlibs(LuaWrapper::L);
	LuaWrapper::bcomps = &m_compManager.get()->m_behavior;
	LuaWrapper::bcompSz = &m_compManager.get()->m_behaviorSz;

	// Create Systems

	m_sys.emplace_back(std::make_unique<Input>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Spawner>(m_compManager.get(), &m_factory));
	m_sys.emplace_back(std::make_unique<Physics>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Behavior>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Animator>(m_compManager.get()));
	m_sys.emplace_back(std::make_unique<Sound>(m_compManager.get(), "data\\SoundFiles.txt"));
	m_sys.emplace_back(std::make_unique<Renderer>(m_compManager.get(), m_window));

	// Events

	registerFunc(this, &GameState::onRSCall);
	registerFunc(this, &GameState::onQueryEntityByTag);
	registerFunc(this, &GameState::onSceneChangeEvent);

	// Test Data
	loadTestData("Data\\TestData.txt");
	buildScene("Main");
};

void GameState::exec()
{
	float delta{ 0.0f };
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

		if (m_clock.getElapsedTime().asMilliseconds() + delta > m_frameRate)
		{
			delta = m_clock.getElapsedTime().asMilliseconds() - m_frameRate;
			if (delta < 0.0f)
				delta = 0.0f;
			m_clock.restart();

			for (auto &p : m_sys)
				p->update();
			std::cout << m_clock.getElapsedTime().asMilliseconds() << std::endl;
		}

		// Scene changes
		if (m_sceneChange)
		{
			m_sceneChange = false;
			buildScene(m_nextName);
		}
	}
}


void GameState::onRSCall(const Events::RSCallEvent *evnt)
{
	std::string tag{ evnt->tag };
	Ruff::Call call = evnt->call;
	call.data = evnt->call.data;
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

void GameState::onSceneChangeEvent(const Events::SceneChangeEvent *evnt)
{
	m_sceneChange = true;
	m_nextName = evnt->name;
}

void GameState::buildScene(const std::string &name)
{
	m_factory.clearScene();
	m_factory.buildScene(name);
	m_name = name;
}

void GameState::loadTestData(const std::string &fName)
{
	std::ifstream ifs{ fName };
	std::string s;
	
	while (true)
	{
		if (!(ifs >> s)) break;
		SceneData sd;
		sd.name = s;
		ifs >> s;
		while ((ifs >> s) && s == "[")
		{
			Entity::PersistType persist{ Entity::PersistType::None };
			bool cache{ false };
			std::string pString, cString;
			ifs >> pString >> cString;
			if (pString == "global") persist = Entity::PersistType::Global;
			else if (pString == "scene") persist = Entity::PersistType::Scene;
			if (cString == "cache") cache = true;
			while (true)
			{
				ifs >> s;
				if (s == "]") break;
				SceneSpawnData ssd;
				ssd.cache = cache;
				ssd.persist = persist;
				int total = stoi(s);
				ifs >> s;
				ssd.spawnData.blueprint = s;
				ifs >> s;
				ssd.spawnData.position.x = stof(s);
				ifs >> s;
				ssd.spawnData.position.y = stof(s);
				ifs >> s;
				int dTotal = stoi(s);
				for (int i = 0; i < dTotal; ++i)
				{
					ifs >> s;
					ssd.spawnData.initData.push_back(stoi(s));
				}
				for (int i = 0; i < total; ++i)
				{
					sd.data.emplace_back(ssd);
				}
			}
		}
		m_sceneData.emplace_back(sd);
	}
}