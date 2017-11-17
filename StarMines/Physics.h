#pragma once

#include "ISystem.h"
#include "ProxMap.h"
#include "Vector2.h"
#include <map>

class Physics : public ISystem
{
public:
	Physics(ComponentManager *cm) :
		ISystem{ cm }
	{}
	void update() override;
private:
	ProxMap m_proxMap;
	Vector2 m_sceneSize{ 800, 600 };
	int m_cellW{ 96 };
	int m_cellH{ 96 };
	std::map<PhysicsComponent *, std::vector<PhysicsComponent *>> m_collisionMap;

	void fillMap();
	void processMovement();
	bool collide(float xa, float ya, float wa, float ha, float xb, float yb, float wb, float hb);
};