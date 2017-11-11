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

	void onSpawnData(const Events::SpawnDataEvent *);
};
