#include "ProxMap.h"
#include "PhysicsComponent.h"
#include "Entity.h"
#include <iostream>

void ProxMap::clear()
{
	for (auto &v : m_map)
		v.clear();
	m_map.clear();
}

void ProxMap::initialize(float w, float h, int cw, int ch)
{
	int iw = (int)w;
	int ih = (int)h;
	m_map.resize(iw / cw + 1);
	for (auto &v : m_map)
		v.resize(ih / ch + 1);
	m_cw = cw;
	m_ch = ch;
}

void ProxMap::insert(PhysicsComponent *pc)
{
	Indices ind = getIndices(pc);

	for (int i = ind.xb; i <= ind.xe; ++i)
	{
		for (int j = ind.yb; j <= ind.ye; ++j)
		{
			m_map[i][j].push_back(pc);
		}
	}

	
}

std::vector <PhysicsComponent *> ProxMap::retrieve(PhysicsComponent *pc)
{
	std::vector <PhysicsComponent *> r;
	Indices ind = getIndices(pc, true);
	for (int i = ind.xb; i <= ind.xe; ++i)
	{
		for (int j = ind.yb; j <= ind.ye; ++j)
		{
			for (auto p : m_map[i][j])
			{
				if (p != pc)
					r.push_back(p);
			}
		}
	}
	return r;
}

ProxMap::Indices ProxMap::getIndices(PhysicsComponent *pc, bool retrieval)
{
	int t, l;
	if (retrieval)
	{
		l = (int)(pc->parent()->position().x + pc->m_colliderPos.x + pc->m_moveVec.x);
		t = (int)(pc->parent()->position().y + pc->m_colliderPos.y + pc->m_moveVec.y);
	}
	else
	{
		l = (int)(pc->parent()->position().x + pc->m_colliderPos.x);
		t = (int)(pc->parent()->position().y + pc->m_colliderPos.y);
	}
	int r = l + (int)(pc->m_colliderSize.x);
	int b = t + (int)(pc->m_colliderSize.y);

	int ib = l / m_cw;
	if (ib < 0) ib = 0;
	int ie = r / m_cw;
	if (ie >= m_map.size()) ie = m_map.size() - 1;
	int jb = t / m_ch;
	if (jb < 0) jb = 0;
	int je = b / m_ch;
	if (je >= m_map[0].size()) je = m_map[0].size() - 1;

	Indices ind;
	ind.xb = ib;
	ind.xe = ie;
	ind.yb = jb;
	ind.ye = je;

	return ind;
}