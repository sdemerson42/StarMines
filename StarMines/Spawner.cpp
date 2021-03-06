#include "Spawner.h"
#include "Factory.h"


Spawner::Spawner(ComponentManager *cm, Factory *fac) :
	ISystem{ cm }, m_factory{ fac }, m_sceneLock{ false }
{
	registerFunc(this, &Spawner::onSpawnData);
	registerFunc(this, &Spawner::onDespawnData);
	registerFunc(this, &Spawner::onSceneChange);
}

void Spawner::update()
{
	if (m_sceneLock)
		m_sceneLock = false;

	for (auto p : m_spawnData)
	{
		Entity::PersistType persist = Entity::PersistType::Default;
		if (p.persist == "global") persist = Entity::PersistType::Global;
		else if (p.persist == "scene") persist = Entity::PersistType::Scene;
		else if (p.persist == "none") persist = Entity::PersistType::None;

		if (p.initData.size() > 0)
			m_factory->activateFromBlueprint(p.blueprint, p.position.x, p.position.y, &p.initData, persist, p.bc);
		else
			m_factory->activateFromBlueprint(p.blueprint, p.position.x, p.position.y, nullptr, persist, p.bc);
	}
	m_spawnData.clear();

	for (auto &dd : m_despawnData)
	{
		m_factory->deactivate(dd.entity);
	}
	m_despawnData.clear();
}

void Spawner::onSpawnData(const Events::SpawnDataEvent *sde)
{
	if (m_sceneLock) return;
	m_spawnData.emplace_back(*sde);
}

void Spawner::onDespawnData(const Events::DespawnEvent *de)
{
	if (m_sceneLock) return;
	m_despawnData.emplace_back(*de);
}

void Spawner::onSceneChange(const Events::SceneChangeEvent *sce)
{
	m_spawnData.clear();
	m_despawnData.clear();
	m_sceneLock = true;
}

