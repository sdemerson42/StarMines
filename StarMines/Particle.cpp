#include "Particle.h"
#include "ComponentManager.h"
#include <algorithm>

void Particle::update()
{
	for (int i = 0; i < m_compManager->m_particleSz; ++i)
	{
		// Calculate current density

		auto &c = m_compManager->m_particle[i];
		c.m_pCount += c.m_densityPerFrame;
		if (c.m_pCount > 1.0f)
		{
			// Activate particles
			int count = (int)c.m_pCount;
			c.m_pCount -= (float)count;
			for (int j = 0; j < count; ++j)
			{
				Vector2 position = c.parent()->position();
				position.x += c.m_offset.x;
				position.y += c.m_offset.y;
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

				// Do not spawn new particles over the maximum allowed...

				if (c.m_particleSz < c.m_maxParticles)
				{
					c.m_particleData[c.m_particleSz++] = ParticleComponent::ParticleData{ position.x, position.y, dxf, dyf, life };
				}
			}
			// move particles
			for (auto &pd : c.m_particleData)
			{
				if (&pd - begin(c.m_particleData)._Ptr == c.m_particleSz)
					break;

				pd.position.x += pd.direction.x;
				pd.position.y += pd.direction.y;
				pd.lifeCounter++;
			}
			// deactivate particles
			for (int j = 0; j < c.m_particleSz; ++j)
			{
				// Deactivate and swap
				if (c.m_particleData[j].lifeCounter == c.m_particleData[j].life)
				{
					c.m_particleData[j].active = false;
					if (j == c.m_particleSz - 1)
					{
						--c.m_particleSz;
						break;
					}
					std::swap(c.m_particleData[j], c.m_particleData[c.m_particleSz - 1]);
					--c.m_particleSz;
				}
			}
		}
	}
}