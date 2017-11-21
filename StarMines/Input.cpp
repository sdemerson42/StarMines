#include "Input.h"
#include "SFML\Window.hpp"

void Input::update()
{
	Events::InputEvent ie{ 0.0f, 0.0f, 0.0f, 0.0f };
	if (sf::Joystick::isConnected(0))
	{
		ie.xAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		ie.yAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
		ie.uAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
		ie.vAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R);
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ie.yAxis = -100.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ie.yAxis = 100.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ie.xAxis = -100.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ie.xAxis = 100.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ie.vAxis = -100.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ie.vAxis = 100.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ie.uAxis = -100.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ie.uAxis = 100.0f;
	}
	broadcast(&ie);
}