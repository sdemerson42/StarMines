#include "Input.h"
#include "SFML\Window.hpp"

void Input::update()
{
	Events::InputEvent ie;
	if (sf::Joystick::isConnected(0))
	{
		ie.xAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
		ie.yAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
		ie.uAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
		ie.vAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R);
		ie.padXAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);
		ie.padYAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
		ie.zAxis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
		ie.a = sf::Joystick::isButtonPressed(0, 0);
		ie.b = sf::Joystick::isButtonPressed(0, 1);
		ie.x = sf::Joystick::isButtonPressed(0, 2);
		ie.y = sf::Joystick::isButtonPressed(0, 3);
		ie.shoulderL = sf::Joystick::isButtonPressed(0, 4);
		ie.shoulderR = sf::Joystick::isButtonPressed(0, 5);
		ie.select = sf::Joystick::isButtonPressed(0, 6);
		ie.start = sf::Joystick::isButtonPressed(0, 7);
		ie.stickL = sf::Joystick::isButtonPressed(0, 8);
		ie.stickR = sf::Joystick::isButtonPressed(0, 9);
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