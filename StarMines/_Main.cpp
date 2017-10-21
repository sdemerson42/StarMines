#include <conio.h>
#include <iostream>

#include "SFML\Graphics.hpp"


int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, "Window" };

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
				window.close();
		}
		if (clock.getElapsedTime().asMilliseconds() > 100 / 6)
		{
			
		}
	}

	_getch();
}