#include "GameState.h"
#include "ReadData.h"
#include <algorithm>
#include "BehaviorComponent.h"
#include <iostream>
#include "LuaWrapper.h"

GameState::GameState() :
	m_compManager{ std::make_unique<ComponentManager>() },
	m_window{ sf::VideoMode{1920,1080}, "StarMines v0.1", sf::Style::Fullscreen }, m_factory{ this, "data\\blueprints.txt" }, m_sceneChange{ false }
{
	std::cout << "Constructing GameState...\n";

	//m_window.setVerticalSyncEnabled(true);

	// Prepare LuaWrapper

	LuaWrapper::L = luabridge::luaL_newstate();
	luaL_openlibs(LuaWrapper::L);
	LuaWrapper::bcomps = &m_compManager.get()->m_behavior;
	LuaWrapper::bcompSz = &m_compManager.get()->m_behaviorSz;
	std::cout << "Lua state created and common libraries opened...\n";

	// Create Systems

	m_sys.push_back(std::make_unique<Input>(m_compManager.get()));
	m_sys.push_back(std::make_unique<Spawner>(m_compManager.get(), &m_factory));
	m_sys.push_back(std::make_unique<Physics>(m_compManager.get()));
	m_sys.push_back(std::make_unique<Behavior>(m_compManager.get()));
	m_sys.push_back(std::make_unique<Animator>(m_compManager.get()));
	m_sys.push_back(std::make_unique<Particle>(m_compManager.get()));
	m_sys.push_back(std::make_unique<Sound>(m_compManager.get(), "data\\SoundFiles.txt"));
	m_sys.push_back(std::make_unique<Renderer>(m_compManager.get(), m_window));
	std::cout << "Systems created...\n";

	// Events

	registerFunc(this, &GameState::onRSCall);
	registerFunc(this, &GameState::onQueryEntityByTag);
	registerFunc(this, &GameState::onSceneChangeEvent);
	registerFunc(this, &GameState::onCreateNewSceneFromScript);
	registerFunc(this, &GameState::onAddSceneData);

	// Test Data
	loadTestData("Data\\TestData.txt");
	buildScene("Main");

	std::cout << "Gamestate constructed...\n";
};

void GameState::exec()
{
	std::cout << "-- Executing game loop... --\n";
	float delta{ 0.0f };

	// Test state

	bool frameCount = false;

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

		// Main loop

		if (m_clock.getElapsedTime().asMilliseconds() + delta > m_frameRate)
		{
			delta = m_clock.getElapsedTime().asMilliseconds() - m_frameRate;
			if (delta < 0.0f)
				delta = 0.0f;
			m_clock.restart();

			for (auto &p : m_sys)
				p->update();

			// ESCAPE KEY - to be removed later

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				m_window.close();

			// Turn on / off FrameCount

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
				frameCount = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
				frameCount = false;


			if (frameCount)
				std::cout << "Frame: " << m_clock.getElapsedTime().asMilliseconds() << std::endl;
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
				c->addPendingCall(call);
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

void GameState::onCreateNewSceneFromScript(const Events::CreateSceneFromScriptEvent *evnt)
{
	SceneData sd;
	sd.name = evnt->name;
	sd.prox = evnt->prox;
	sd.view = evnt->view;
	m_sceneData.push_back(sd);
}

void GameState::onAddSceneData(const Events::AddSceneDataEvent *evnt)
{
	auto p = std::find_if(begin(m_sceneData), end(m_sceneData), [&](SceneData &sd)
	{
		return sd.name == evnt->name;
	});

	if (p == end(m_sceneData))
	{
		std::cout << "WARNING: Failed to add scene data from script. Scene name was not found.\n";
		return;
	}

	SceneSpawnData ssd;
	
	ssd.persist = Entity::PersistType::None;
	if (evnt->persist == "global") ssd.persist = Entity::PersistType::Global;
	else if (evnt->persist == "scene") ssd.persist = Entity::PersistType::Scene;
	
	ssd.cache = evnt->cache;
	
	ssd.spawnData.blueprint = evnt->blueprint;
	ssd.spawnData.position.x = evnt->x;
	ssd.spawnData.position.y = evnt->y;
	
	if (evnt->init.size() > 0)
	{
		std::string n;
		for (char ch : evnt->init)
		{
			if (ch == ',')
			{
				ssd.spawnData.initData.push_back(std::stoi(n));
				n = "";
			}
			else
			{
				n += ch;
			}
		}
		ssd.spawnData.initData.push_back(std::stoi(n));
	}

	for (int i = 0; i < evnt->count; ++i)
	{
		p->data.push_back(ssd);
	}
}




void GameState::buildScene(const std::string &name)
{
	// Factory clears and builds new Entities
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
		ifs >> sd.prox.sceneX >> sd.prox.sceneY >> sd.prox.cellX >> sd.prox.cellY;
		ifs >> sd.view.viewW >> sd.view.viewH >> sd.view.portX >> sd.view.portY >>
			sd.view.portW >> sd.view.portH >> sd.view.centerX >> sd.view.centerY;
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
					sd.data.push_back(ssd);
				}
			}
		}
		m_sceneData.push_back(sd);
	}
	std::cout << "Scene data loaded...\n";
}