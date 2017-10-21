#pragma once

#include <vector>

class PhysicsComponent;

class ProxMap
{
public:
	void clear();
	void initialize(float sw, float sh, int cw, int ch);
	void insert(PhysicsComponent *);
	std::vector <PhysicsComponent *> retrieve(PhysicsComponent *pc);
private:
	struct Indices
	{
		int xb;
		int xe;
		int yb;
		int ye;
	};
	Indices getIndices(PhysicsComponent *, bool retrieval = false);
	std::vector<std::vector<std::vector<PhysicsComponent *>>> m_map;
	float m_cw;
	float m_ch;
};