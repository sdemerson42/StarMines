#include "Physics.h"
#include "ComponentManager.h"
#include "PhysicsComponent.h"
#include "BehaviorComponent.h"
#include "RuffCommon.h"
#include <iostream>

void Physics::update()
{
	fillMap();
	processMovement();
}

void Physics::fillMap()
{
	m_proxMap.clear();
	m_proxMap.initialize(m_sceneSize.x, m_sceneSize.y, m_cellW, m_cellH);
	for (int i{ 0 }; i < m_compManager->m_physicsSz; ++i)
	{
		m_proxMap.insert(&m_compManager->m_physics[i]);
	}
}

void Physics::processMovement()
{
	m_collisionMap.clear();

	for (int i{ 0 }; i < m_compManager->m_physicsSz; ++i)
	{
		//bugTest();
		auto cp = &m_compManager->m_physics[i];

		if (cp->m_moveVec.x == 0.0f && cp->m_moveVec.y == 0.0f)
			continue;
		
		float vecX = cp->m_moveVec.x * cp->m_speed;
		float vecY = cp->m_moveVec.y * cp->m_speed;

		int xDir{ 0 };
		int yDir{ 0 };

		if (vecX < 0) xDir = -1;
		else if (vecX > 0) xDir = 1;

		if (vecY < 0) yDir = -1;
		else if (vecY > 0) yDir = 1;

		auto v = m_proxMap.retrieve(cp);

		float xa = cp->parent()->position().x + cp->m_colliderPos.x + vecX;
		float ya = cp->parent()->position().y + cp->m_colliderPos.y;
		float wa = cp->m_colliderSize.x;
		float ha = cp->m_colliderSize.y;

		for (auto p : v)
		{
			float xb = p->parent()->position().x + p->m_colliderPos.x;
			float yb = p->parent()->position().y + p->m_colliderPos.y;
			float wb = p->m_colliderSize.x;
			float hb = p->m_colliderSize.y;

			// x movement
			if (xDir != 0)
			{
				// Collision: Apply physics changes only if solid.
				if (collide(xa, ya, wa, ha, xb, yb, wb, hb))
				{
					if (cp->m_solid && p->m_solid)
					{
						if (xDir < 0)
						{
							float cpos = xb + wb + 0.001f;
							xa = cpos;
							vecX = 0.0f;
							cp->parent()->setPosition(cpos - cp->m_colliderPos.x, cp->parent()->position().y);
						}
						else
						{
							float cpos = xb - wa - 0.001f;
							xa = cpos;
							vecX = 0.0f;
							cp->parent()->setPosition(cpos - cp->m_colliderPos.x, cp->parent()->position().y);
						}
						cp->m_moveVec.x = 0.0f;
					}
					callSender(cp, p);
				}
			}
		}
		ya += vecY;
		for (auto p : v)
		{
			// y movement

			float xb = p->parent()->position().x + p->m_colliderPos.x;
			float yb = p->parent()->position().y + p->m_colliderPos.y;
			float wb = p->m_colliderSize.x;
			float hb = p->m_colliderSize.y;
			
			if (yDir != 0.0f)
			{
				if (collide(xa, ya, wa, ha, xb, yb, wb, hb))
				{
					if (cp->m_solid && p->m_solid)
					{
						// Collision: Apply physics changes only if solid.
						if (vecY < 0.0f)
						{
							float cpos = yb + hb + 0.001f;
							ya = cpos;
							vecY = 0.0f;
							cp->parent()->setPosition(cp->parent()->position().x, cpos - cp->m_colliderPos.y);
						}
						else
						{
							float cpos = yb - ha - 0.001f;
							ya = cpos;
							vecY = 0.0f;
							cp->parent()->setPosition(cp->parent()->position().x, cpos - cp->m_colliderPos.y);
						}
						cp->m_moveVec.y = 0.0f;
					}
					callSender(cp, p);
				}
			}
		}
		// Add remaining movement
		cp->parent()->addPosition(vecX, vecY);
	}
}

bool Physics::collide(float xa, float ya, float wa, float ha, float xb, float yb, float wb, float hb)
{
	return xa + wa > xb && xa < xb + wb && ya + ha > yb && ya < yb + hb;
}

void Physics::callSender(PhysicsComponent *cp, PhysicsComponent *p)
{
	bool send{ true };
	auto key = m_collisionMap.find(p);
	if (key != end(m_collisionMap))
	{
		auto val = std::find(begin(key->second), end(key->second), cp);
		if (val != end(key->second))
			send = false;
	}

	if (send)
	{
		auto bc = cp->parent()->getComponent<BehaviorComponent>();
		if (bc)
		{
			Ruff::Call call;
			call.caller = p->parent();
			call.label = "collision";
			bc->addCall(call);
		}
		bc = p->parent()->getComponent<BehaviorComponent>();
		if (bc)
		{
			Ruff::Call call;
			call.caller = cp->parent();
			call.label = "collision";
			bc->addCall(call);
		}
		m_collisionMap[cp].emplace_back(p);
	}
}

void Physics::bugTest()
{
	for (int j = 0; j < m_compManager->m_physicsSz; ++j)
	{
		auto cp = &m_compManager->m_physics[j];
		for (int i = 0; i < m_compManager->m_physicsSz; ++i)
		{
			float xa = cp->parent()->position().x + cp->m_colliderPos.x;
			float ya = cp->parent()->position().y + cp->m_colliderPos.y;
			float wa = cp->m_colliderSize.x;
			float ha = cp->m_colliderSize.y;
			auto p = &m_compManager->m_physics[i];
			if (p != cp)
			{
				float xb = p->parent()->position().x + p->m_colliderPos.x;
				float yb = p->parent()->position().y + p->m_colliderPos.y;
				float wb = p->m_colliderSize.x;
				float hb = p->m_colliderSize.y;
				if (collide(xa, ya, wa, ha, xb, yb, wb, hb))
					std::cout << "ERROR!\n";
			}
		}
	}
}