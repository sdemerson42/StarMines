#include "Factory.h"
#include "GameState.h"
#include "ComponentManager.h"
#include "RuffCommon.h"
#include <fstream>
#include <iostream>
#include <algorithm>

std::istream &operator >> (std::istream &is, Factory::Blueprint &b);


Factory::Factory(GameState *gameState, const std::string &fName) :
	m_gameState{ gameState }
{
	std::ifstream ifs{ fName };
	if (!ifs)
	{
		std::cerr << "WARNING: Factory failed to open file for initialization.\n";
	}

	std::cout << "Loading Blueprints...\n[ ";

	while (true)
	{
		Blueprint b;
		if (!(ifs >> b))
			break;
		m_blueprint.emplace_back(b);
		std::cout << m_blueprint.back().name << "   ";
	}
	std::cout << " ]\nBlueprints Loaded...\n";
}

std::istream &operator >> (std::istream &is, Factory::CompData &cd)
{
	std::string s;
	if (!(is >> s) || s != "[")
		return is;
	if (!(is >> s))
		return is;
	cd.type = s;

	while (true)
	{
		if (!(is >> s))
			return is;
		if (s == "]")
			break;
		cd.data.emplace_back(s);
	}

	return is;
}

std::istream &operator >> (std::istream &is, Factory::Blueprint &b)
{
	std::string s;
	if (!(is >> s))
		return is;

	b.name = s;
	b.tag.emplace_back(s);

	char c;
	if (!(is >> c) || c != ':')
	{
		std::cerr << "WARNING: Factory failed to read Blueprint (formatting error).\n";
		return is;
	}
	
	while (true)
	{
		if (!(is >> s))
		{
			std::cerr << "WARNING: Factory failed to read Blueprint (formatting error).\n";
			return is;
		}
		if (s == "{")
			break;
		b.tag.emplace_back(s);
	}

	while (true)
	{
		if (!(is >> s))
		{
			std::cerr << "WARNING: Factory failed to read Blueprint (formatting error).\n";
			return is;
		}

		if (s == "}")
			break;
		if (s == "[")
			is.putback(s[0]);

		Factory::CompData cd;
		if (!(is >> cd))
		{
			std::cerr << "WARNING: Factory failed to read Blueprint (error reading Component data).\n";
			return is;
		}
		b.compData.emplace_back(cd);
	}

	return is;
}

void Factory::createFromBlueprint(const std::string &blueprint, float x, float y, std::vector<int> *initData, bool cache, Entity::PersistType persist)
{
	auto p = std::find_if(begin(m_blueprint), end(m_blueprint), [&](const Blueprint &b)
	{
		return b.name == blueprint;
	});
	if (p == end(m_blueprint))
	{
		std::cerr << "WARNING: Factory failed to create Entity from Blueprint '" << blueprint << "'.\n";
		return;
	}
	m_gameState->m_entity.emplace_back(std::make_shared<Entity>());
	auto e = m_gameState->m_entity[m_gameState->m_entity.size()-1].get();
	e->setPosition(x, y);
	for (auto &t : p->tag)
		e->addTag(t);

	for (auto &cd : p->compData)
	{
		m_gameState->m_compManager->addComponent(e, cd.type, cd.data);
	}
	if (initData)
		addInitCall(e, initData);

	e->setPersist(persist);

	if (cache)
		m_gameState->m_compManager->deactivateAll(e);
	else
		e->setActive(true);
}

void Factory::activateFromBlueprint(const std::string &blueprint, float x, float y, std::vector<int> *initData, Entity::PersistType persist)
{
	auto &v = m_gameState->m_entity;
	auto p = std::find_if(begin(v), end(v), [&](std::shared_ptr<Entity> &sp)
	{
		return sp->findTag(blueprint) && !sp->active();
	});

	if (p != end(v))
	{
		m_gameState->m_compManager->activateAll(p->get());
		(*p)->setPosition(x, y);

		auto c = p->get()->getComponent<BehaviorComponent>();
		if (c)
			c->reactivate();

		if (initData)
			addInitCall(p->get(), initData);
		
		if (persist != Entity::PersistType::Default)
			(*p)->setPersist(persist);
	}
	else
	{
		if (persist == Entity::PersistType::Default) persist = Entity::PersistType::None;
		createFromBlueprint(blueprint, x, y, initData, false, persist);
	}
}

void Factory::deactivate(Entity *e)
{
	m_gameState->m_compManager->deactivateAll(e);
}

void Factory::addInitCall(Entity *e, std::vector<int> *initData)
{
	auto c = e->getComponent<BehaviorComponent>();
	if (c)
	{
		Ruff::Call call;
		call.data = *initData;
		call.label = "init";
		c->addCall(call);
	}
}

void Factory::buildScene(const std::string &name)
{
	auto &data = std::find_if(begin(m_gameState->m_sceneData), end(m_gameState->m_sceneData), [&](GameState::SceneData &sd)
	{
		return sd.name == name;
	});
	if (data == end(m_gameState->m_sceneData))
	{
		std::cout << "WARNING: Scene '" << name << "' not built. Name could not be found.\n";
		return;
	}
	
	// Scene data found; begin construction...

	// Update ProxMap settings
	
	broadcast(&data->prox);

	// Update view and viewport

	broadcast(&data->view);

	// Build entites from scene data

	auto &v = data->data;
	for (auto &ssd : v)
	{
		createFromBlueprint(ssd.spawnData.blueprint, ssd.spawnData.position.x, ssd.spawnData.position.y, 
			(ssd.spawnData.initData.size() == 0 ? nullptr : &ssd.spawnData.initData), ssd.cache, ssd.persist);
	}

	// Clear scene data
	v.clear();
	
	std::cout << "Scene \"" << name << "\" built...\n";
}

void Factory::clearScene()
{
	std::string name{ m_gameState->m_name };

	auto sceneIter = std::find_if(begin(m_gameState->m_sceneData), end(m_gameState->m_sceneData), [&](GameState::SceneData &d)
	{
		return d.name == name;
	});

	if (sceneIter == end(m_gameState->m_sceneData)) return;

	sceneIter->data.clear();

	for (auto &spe : m_gameState->m_entity)
	{
		Entity::PersistType persist = spe->persist();
		if (persist == Entity::PersistType::Scene)
		{
			GameState::SceneSpawnData ssd;
			ssd.spawnData.blueprint = spe->name();
			ssd.spawnData.position = spe->position();
			auto bc = spe->getComponent<BehaviorComponent>();
			if (bc)
				ssd.spawnData.initData = bc->getSceneDespawnData();
			ssd.persist = Entity::PersistType::Scene;
			ssd.cache = false;
			sceneIter->data.emplace_back(ssd);
		}
		
		if (persist != Entity::PersistType::Global)
		{
			deactivate(spe.get());
			m_gameState->m_compManager->removeAll(spe.get());
		}

		auto pc = spe->getComponent<ParticleComponent>();
		if (pc)
			pc->reset();
	}

	auto b = begin(m_gameState->m_entity);
	const auto e = end(m_gameState->m_entity);
	while (b != e && (*b)->persist() == Entity::PersistType::Global)
		++b;
	if (b != e)
		m_gameState->m_entity.erase(b, e);
	std::cout << "Scene cleared...\n";
}