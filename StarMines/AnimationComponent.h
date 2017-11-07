#pragma once

#include "IComponent.h"
#include <map>
#include <string>
#include "Vector2.h"
#include "RenderComponent.h"

class AnimationComponent : public IComponent
{
	friend class Animator;
public:
	AnimationComponent() :
		m_playing{ false }
	{}
	void initialize(const std::vector<std::string> &input) override
	{
		int tot = std::stoi(input[0]);
		int i{ 1 };
		int c{ 0 };
		while (c < tot)
		{
			Animation a;
			std::string name{ input[i++] };
			a.frameTot = std::stoi(input[i++]);
			int j{ 0 };
			while (j < a.frameTot)
			{
				a.framePos.emplace_back(Vector2{ std::stof(input[i]), std::stof(input[i+1]) });
				i += 2;
				++j;
			}
			a.fDelay = std::stoi(input[i++]);
			a.loop = bool(std::stoi(input[i++]));
			m_anim[name] = a;
			++c;
		}
		std::string def{ input[input.size() - 1] };
		if (def != "none")
			play(def);
	}
	void writeOut(std::vector<std::string> &output) override
	{
	}
	void play(const std::string &tag)
	{
		auto p = m_anim.find(tag);
		if (p != end(m_anim))
		{
			m_curAnim = p->second;
			m_playing = true;
			m_frame = 0;
			m_counter = 0;
			if (parent())
			{
				auto rc = parent()->getComponent<RenderComponent>();
				if (rc)
					rc->setTexPosition(m_curAnim.framePos[0].x, m_curAnim.framePos[0].y);
			}
		}
	}
	void stop()
	{
		m_playing = false;
	}
private:
	struct Animation : public Serializable
	{
		std::vector<Vector2> framePos;
		int frameTot;
		int fDelay;
		bool loop;
	};
	bool m_playing;
	int m_frame;
	int m_counter;
	Animation m_curAnim;
	std::map<std::string, Animation> m_anim;
};