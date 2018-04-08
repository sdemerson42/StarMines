#include "Physics.h"
#include "ComponentManager.h"
#include "PhysicsComponent.h"
#include "BehaviorComponent.h"
#include "RuffCommon.h"
#include <iostream>

void Physics::update()
{
	fillMap();
	adjustMomentum();
	applyMomentum();
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

void Physics::adjustMomentum()
{
	m_collisionMap.clear();
	m_momentumTable.clear();

	for (int i{ 0 }; i < m_compManager->m_physicsSz; ++i)
	{
		auto cp = &m_compManager->m_physics[i];

		if ((cp->m_moveVec.x == 0.0f && cp->m_moveVec.y == 0.0f) || cp->m_speed == 0.0f)
			continue;

		auto v = m_proxMap.retrieve(cp);

		float cpx = cp->parent()->position().x + cp->m_colliderPos.x;
		float cpy = cp->parent()->position().y + cp->m_colliderPos.y;

		float vecX = cp->m_moveVec.x * cp->m_speed;
		float vecY = cp->m_moveVec.y * cp->m_speed;

		float aw = cp->m_colliderSize.x;
		float ah = cp->m_colliderSize.y;

		for (auto p : v)
		{
			float pVecX = p->m_moveVec.x * p->m_speed;
			float pVecY = p->m_moveVec.y * p->m_speed;
			float px = p->parent()->position().x + p->m_colliderPos.x;
			float py = p->parent()->position().y + p->m_colliderPos.y;
			float bw = p->m_colliderSize.x;
			float bh = p->m_colliderSize.y;

			// Solids?

			if (!cp->m_solid || !p->m_solid)
			{
				if (collide(cpx + vecX, cpy + vecY, aw, ah,
					px + pVecX, py + pVecY, bw, bh))
				{
					callSender(cp, p);
				}
				continue;
			}

			bool exFlag = false;

			// Check X

			float ax = cpx + vecX;
			float ay = cpy;
			float bx = px + pVecX;
			float by = py;

			// Leading exception
			if (vecX > 0.0f && cpx > px) exFlag = true;
			if (vecX < 0.0f && cpx < px) exFlag = true;

			if (!exFlag)
			{
				if (collide(ax, ay, aw, ah, bx, by, bw, bh))
				{
					callSender(cp, p);
					float vecScalar = 0.5f;
					bool scaleFlag = false;
					while (vecScalar > 0.1f)
					{
						vecX *= vecScalar;
						pVecX *= vecScalar;
						vecScalar *= 0.5f;
						ax = cpx + vecX;
						bx = px + pVecX;
						if (!collide(ax, ay, aw, ah, bx, by, bw, bh))
						{
							scaleFlag = true;
							break;
						}
					}
					if (!scaleFlag)
					{
						vecX = 0.0f;
						ax = cpx;
						bx = px;
					}
				}
			}

			// Check Y

			exFlag = false;
			ay = cpy + vecY;
			by = py + pVecY;

			// Leading exception
			if (vecY > 0.0f && cpy > py) exFlag = true;
			if (vecY < 0.0f && cpy < py) exFlag = true;

			if (!exFlag)
			{
				if (collide(ax, ay, aw, ah, bx, by, bw, bh))
				{
					callSender(cp, p);
					float vecScalar = 0.5f;
					bool scaleFlag = false;
					while (vecScalar > 0.1f)
					{
						vecY *= vecScalar;
						pVecY *= vecScalar;
						vecScalar *= 0.5f;
						ay = cpy + vecY;
						by = py + pVecY;
						if (!collide(ax, ay, aw, ah, bx, by, bw, bh))
						{
							scaleFlag = true;
							break;
						}
					}
					if (!scaleFlag)
					{
						vecY = 0.0f;
					}
				}
			}
			
		}

		MomentumData md;
		md.p = cp;
		md.momentum.x = vecX;
		md.momentum.y = vecY;
		m_momentumTable.push_back(md);

		cp->setDir(vecX, vecY);
	}
}

void Physics::applyMomentum()
{
	for (const auto &data : m_momentumTable)
	{
		data.p->parent()->addPosition(data.momentum.x, data.momentum.y);
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