#include "ProxMap.h"
#include "PhysicsComponent.h"
#include "Entity.h"
#include <algorithm>
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
				if (p != pc && std::find(begin(r), end(r), p) == end(r))
					r.push_back(p);
			}
		}
	}
	return r;
}

ProxMap::Indices ProxMap::getIndices(PhysicsComponent *pc, bool retrieval)
{
	int pcx = (int)pc->parent()->position().x + pc->m_colliderPos.x;
	int pcy = (int)pc->parent()->position().y + pc->m_colliderPos.y;
	int pcvx = (int)(pc->m_moveVec.x * pc->m_speed);
	int pcvy = (int)(pc->m_moveVec.y * pc->m_speed);
	int pccx = (int)pc->m_colliderSize.x;
	int pccy = (int)pc->m_colliderSize.y;

	int l = std::min(pcx, pcx + pcvx);
	int t = std::min(pcy, pcy + pcvy);
	int r = std::max(pcx + pccx, pcx + pccx + pcvx);
	int b = std::max(pcy + pccy, pcy + pccy + pcvy);

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