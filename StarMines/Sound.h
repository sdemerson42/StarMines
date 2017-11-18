#pragma once

#include "ISystem.h"
#include "SFML\Audio.hpp"
#include <vector>
#include <map>
#include "Events.h"

class Sound : public ISystem
{
public:
	Sound(ComponentManager *cm, const std::string &fName);
private:
	struct SoundChannel
	{
		sf::Sound channel;
		std::string tag;
	};
	std::map<std::string, sf::SoundBuffer> m_buffer;
	std::vector<SoundChannel> m_sound;
	unsigned int m_lowIndex;
	unsigned int m_hiIndex;
	const unsigned int m_hiSz{ 2 };
	const unsigned int m_lowSz{ 2 };
	std::vector<Events::SoundEvent> m_event;

	void onSound(const Events::SoundEvent *evnt);
	void update() override;
};