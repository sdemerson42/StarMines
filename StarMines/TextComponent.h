#pragma once

#include "IComponent.h"
#include "ColorState.h"
#include "SFML\Graphics.hpp"

class TextComponent : public IComponent, public ColorState
{
	friend class Renderer;
public:
	TextComponent()
	{}
	void initialize(const std::vector<std::string> &input) override
	{
		// Order: Font fName, size, color, string
		m_font.loadFromFile(input[0]);
		m_text.setFont(m_font);
		m_text.setCharacterSize(std::stoi(input[1]));

		int r = std::stoi(input[2]);
		int g = std::stoi(input[3]);
		int b = std::stoi(input[4]);
		int a = std::stoi(input[5]);

		setColor(r, g, b, a);
		m_text.setFillColor(sf::Color{ uint8_t(r), uint8_t(g), uint8_t(b), uint8_t(a) });
		m_text.setString(input[6]);
	}
	const std::string &getTag() const override
	{
		return m_tag;
	}
	void setString(const std::string &str)
	{
		m_text.setString(str);
	}
	void appendString(const std::string &str)
	{
		std::string s{ m_text.getString() };
		s.append(str);
		m_text.setString(s);
	}
	void setColor(int r, int g, int b, int a) override
	{
		ColorState::setColor(r, g, b, a);
		m_text.setFillColor(sf::Color{ (sf::Uint8)m_color.r, (sf::Uint8)m_color.g, (sf::Uint8)m_color.b, (sf::Uint8)m_color.a, });
	}
private:
	sf::Text m_text;
	sf::Font m_font;
	static std::string m_tag;
};
