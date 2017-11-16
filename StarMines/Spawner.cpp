#include "Spawner.h"
#include "Factory.h"


Spawner::Spawner(ComponentManager *cm, Factory *fac) :
	ISystem{ cm }, m_factory{ fac }
{
	registerFunc(this, &Spawner::onSpawnData);
	registerFunc(this, &Spawner::onDespawnData);
}

void Spawner::update()
{
	for (auto p : m_spawnData)
	{
		if (p.initData.size() > 0)
			m_factory->activateFromBlueprint(p.blueprint, p.position.x, p.position.y, &p.initData);
		else
			m_factory->activateFromBlueprint(p.blueprint, p.position.x, p.position.y);
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
	m_spawnData.emplace_back(*sde);
}

void Spawner::onDespawnData(const Events::DespawnEvent *de)
{
	m_despawnData.emplace_back(*de);
}

