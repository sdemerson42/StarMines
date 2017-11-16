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
	for (int i{ 0 }; i < m_compManager->m_physicsSz; ++i)
	{
		auto cp = &m_compManager->m_physics[i];
		if (cp->m_moveVec.x == 0.0f && cp->m_moveVec.y == 0.0f)
			continue;
		
		float vecX = cp->m_moveVec.x * cp->m_speed;
		float vecY = cp->m_moveVec.y * cp->m_speed;

		auto v = m_proxMap.retrieve(cp);
		
		for (auto p : v)
		{
			float xa = cp->parent()->position().x + cp->m_colliderPos.x;
			float ya = cp->parent()->position().y + cp->m_colliderPos.y;
			float wa = cp->m_colliderSize.x;
			float ha = cp->m_colliderSize.y;

			float xb = p->parent()->position().x + p->m_colliderPos.x;
			float yb = p->parent()->position().y + p->m_colliderPos.y;
			float wb = p->m_colliderSize.x;
			float hb = p->m_colliderSize.y;

			bool collFlag{ false };

			// x movement
			if (vecX != 0.0f)
			{
				float oxa = xa;
				xa += vecX;
				if (collide(xa, ya, wa, ha, xb, yb, wb, hb))
				{
					// Collision: Apply physics changes only if solid.
					collFlag = true;
					if (cp->m_solid && p->m_solid)
					{
						if (vecX < 0.0f)
						{
							float cpos = xb + wb + 0.001f;
							float diff = abs(cpos - oxa);
							vecX += diff;
							xa += diff;
							//cp->parent()->setPosition(xb + wb + 0.001f - p->m_colliderPos.x, cp->parent()->position().y);
						}
						else
						{
							float cpos = xb - wa - 0.001f;
							float diff = abs(cpos - oxa);
							vecX -= diff;
							xa -= diff;
							//cp->parent()->setPosition(xb - wa - 0.001f - p->m_colliderPos.x, cp->parent()->position().y);
						}
						//xa = cp->parent()->position().x + cp->m_colliderPos.x;
						cp->m_moveVec.x = 0.0f;
					}
				}
			}
		
			// y movement
			if (vecY != 0.0f)
			{
				float oya = ya;
				ya += vecY;
				if (collide(xa, ya, wa, ha, xb, yb, wb, hb))
				{
					if (cp->m_solid && p->m_solid)
					{
						collFlag = true;
						// Collision: Apply physics changes only if solid.
						if (vecY < 0.0f)
						{
							float cpos = yb + hb + 0.001f;
							float diff = abs(cpos - oya);
							vecY += diff;
							ya += diff;
							//cp->parent()->setPosition(cp->parent()->position().x, yb + hb + 0.001f - p->m_colliderPos.y);
						}
						else
						{
							float cpos = yb - ha - 0.001f;
							float diff = abs(cpos - oya);
							vecX -= diff;
							ya -= diff;
							//cp->parent()->setPosition(cp->parent()->position().x, yb - ha - 0.001f - p->m_colliderPos.y);
						}
						cp->m_moveVec.y = 0.0f;
					}
				}
			}
			// Events / notifications?
			if (collFlag)
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
			}
			
		}
		// Add remaining movement
		cp->parent()->addPosition(cp->m_moveVec.x * cp->m_speed, cp->m_moveVec.y * cp->m_speed);
	}
}

bool Physics::collide(float xa, float ya, float wa, float ha, float xb, float yb, float wb, float hb)
{
	return xa + wa > xb && xa < xb + wb && ya + ha > yb && ya < yb + hb;
}