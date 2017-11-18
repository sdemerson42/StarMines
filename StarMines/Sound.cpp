#include "Sound.h"
#include "ReadData.h"
#include <iostream>
#include <algorithm>

Sound::Sound(ComponentManager *cm, const std::string &fName) :
	ISystem{ cm }
{
	
	auto data = readData::loadStringData(fName);
	std::cout << "Sound data loaded.\n";

	for (int i = 0; i < data.size(); i+=2)
		m_buffer[data[i]].loadFromFile(data[i + 1]);

	m_sound.resize(m_lowSz + m_hiSz);
	m_hiIndex = 0;
	m_lowIndex = m_hiSz;
	
	registerFunc(this, &Sound::onSound);
}

void Sound::onSound(const Events::SoundEvent *evnt)
{
	m_event.emplace_back(*evnt);
}

void Sound::update()
{
	for (auto &evnt : m_event)
	{
		if (evnt.stop)
		{
			auto s = std::find_if(begin(m_sound), end(m_sound), [&](SoundChannel &sc)
			{
				return sc.tag == evnt.tag;
			});
			if (s != end(m_sound))
			{
				s->channel.stop();
			}
		}
		else
		{
			auto sb = m_buffer.find(evnt.tag);
			if (sb != end(m_buffer))
			{
				auto free = std::find_if(begin(m_sound), end(m_sound), [&](SoundChannel &sc)
				{
					return sc.channel.getStatus() == sf::Sound::Stopped;
				});

				if (free != end(m_sound))
				{
					sf::Sound *s = &free->channel;
					free->tag = evnt.tag;
					s->setBuffer(sb->second);
					s->setVolume(evnt.volume);
					s->setLoop(evnt.loop);
					s->play();
				}
				else
				{

					sf::Sound *s;
					if (evnt.hiPriority)
					{
						s = &m_sound[m_hiIndex].channel;
						m_sound[m_hiIndex].tag = evnt.tag;
					}
					else
					{
						s = &m_sound[m_lowIndex].channel;
						m_sound[m_lowIndex].tag = evnt.tag;
					}
					if (s->getStatus() == sf::Sound::Playing)
						s->stop();
					s->setBuffer(sb->second);
					s->setVolume(evnt.volume);
					s->setLoop(evnt.loop);
					s->play();

					if (evnt.hiPriority)
					{
						++m_hiIndex;
						if (m_hiIndex == m_hiSz)
							m_hiIndex = 0;
					}
					else
					{
						++m_lowIndex;
						if (m_lowIndex == m_sound.size())
							m_lowIndex = m_hiSz;
					}
				}
			}


		}

	}
	m_event.clear();
}



