#include <conio.h>
#include <iostream>

#include "SFML\Graphics.hpp"

#include "Entity.h"
#include "ComponentManager.h"
#include "Animator.h"
#include "Renderer.h"
#include "Physics.h"
#include "Behavior.h"

#include "ReadData.h"


int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, "Window" };

	sf::Clock clock;
	
	ComponentManager cm;

	std::vector<ISystem *> sys;
	sys.push_back(new Animator{ &cm });
	sys.push_back(new Physics{ &cm });
	sys.push_back(new Behavior{ &cm });
	sys.push_back(new Renderer{ &cm, window });

	Entity e;
	cm.addComponent(&e, "render", readData::loadStringData("data\\data.txt"));
	cm.addComponent(&e, "physics", readData::loadStringData("data\\data3.txt"));
	cm.addComponent(&e, "anim", readData::loadStringData("data\\data2.txt"));
	cm.addComponent(&e, "behavior", readData::loadStringData("data\\data4.txt"));
	e.getComponent<AnimationComponent>()->play("right");

	

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
			clock.restart();
			for (auto p : sys)
				p->update();
		}
	}

	_getch();
}