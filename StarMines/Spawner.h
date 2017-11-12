#pragma once

#include "ISystem.h"

class Factory;

class Spawner : public ISystem
{
public:
	Spawner(ComponentManager *, Factory *);
	void update() override;
private:
	Factory *m_factory;
	std::vector<Events::SpawnDataEvent> m_spawnData;
	std::vector<Events::DespawnEvent> m_despawnData;

	void onSpawnData(const Events::SpawnDataEvent *);
	void onDespawnData(const Events::DespawnEvent *);
};
