#include "Particle.h"
#include "ComponentManager.h"

void Particle::update()
{
	for (int i = 0; i < m_compManager->m_particleSz; ++i)
	{
		// Calculate current density

		auto &c = m_compManager->m_particle[i];
		c.m_pCount += c.m_densityPerFrame;
		if (c.m_pCount > 1.0f)
		{
			// Add particles
			int count = (int)c.m_pCount;
			c.m_pCount -= (float)count;
			for (int j = 0; j < count; ++j)
			{
				Vector2 position = c.parent()->position();
				int dx = rand() % 201 - 100;
				int dy = rand() % 201 - 100;
				float mag = sqrt(dx * dx + dy * dy);
				float dxf = (float)dx / mag;
				float dyf = (float)dy / mag;
				float speedVar = (float)rand() / (float)(RAND_MAX / (2.0f*c.m_speedVariance)) - c.m_speedVariance;
				float speed = c.m_speed + speedVar;
				dxf *= speed;
				dyf *= speed;
				int life = c.m_life + rand() % (c.m_lifeVariance * 2) - c.m_lifeVariance;

				c.m_particleData.emplace_back(position.x, position.y, dxf, dyf, life);
			}
			// move particles
			for (auto &pd : c.m_particleData)
			{
				pd.position.x += pd.direction.x;
				pd.position.y += pd.direction.y;
				pd.lifeCounter++;
			}
			// delete particles
			for (int j = 0; j < c.m_particleData.size(); ++j)
			{
				if (c.m_particleData[j].life == c.m_particleData[j].lifeCounter)
				{
					c.m_particleData.erase(begin(c.m_particleData) + j);
					--j;
				}
			}
		}
	}
}