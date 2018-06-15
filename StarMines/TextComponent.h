#pragma once

#include "IComponent.h"
#include "SFML\Graphics.hpp"

class TextComponent : public IComponent
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
		m_text.setFillColor(sf::Color{ uint8_t(std::stoi(input[2])), uint8_t(std::stoi(input[3])), uint8_t(std::stoi(input[4])), uint8_t(std::stoi(input[5])) });
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
private:
	sf::Text m_text;
	sf::Font m_font;
	static std::string m_tag;
};
