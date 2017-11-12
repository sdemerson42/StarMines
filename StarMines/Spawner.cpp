#include "Spawner.h"
#include "Factory.h"


Spawner::Spawner(ComponentManager *cm, Factory *fac) :
	ISystem{ cm }, m_factory{ fac }
{
	registerFunc(this, &Spawner::onSpawnData);
}

void Spawner::update()
{
	for (auto p : m_spawnData)
	{
		m_factory->activateFromBlueprint(p.blueprint, p.position.x, p.position.y);
	}
	m_spawnData.clear();
}

void Spawner::onSpawnData(const Events::SpawnDataEvent *sde)
{
	m_spawnData.emplace_back(*sde);
}

