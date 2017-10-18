#include "Animator.h"
#include "AnimationComponent.h"
#include "ComponentManager.h"
#include "RenderComponent.h"
#include "Entity.h"

void Animator::update()
{
	int i{ 0 };
	while (i < m_compManager->m_animSz)
	{
		auto &c = m_compManager->m_anim[i];
		auto &a = c.m_curAnim;

		if (c.m_playing)
		{
			++c.m_counter;
			if (c.m_counter == a.fDelay)
			{
				c.m_counter = 0;
				if (++c.m_frame == a.frameTot)
				{
					if (!a.loop)
					{
						c.m_playing = false;
					}
					else
					{
						c.m_frame = 0;
					}
				}
				if (c.m_playing)
				{
					auto r = c.parent()->getComponent<RenderComponent>();
					r->m_position = a.framePos[c.m_frame];
				}
			}
		}
		++i;
	}
}