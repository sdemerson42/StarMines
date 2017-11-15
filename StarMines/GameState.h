#pragma once

#include <vector>
#include <memory>
#include "Systems.h"
#include "Entity.h"
#include "ComponentManager.h"
#include "SFML\System.hpp"
#include "Factory.h"
#include "EventSystem.h"

class GameState : public EventHandler
{
	friend class Factory;
public:
	GameState();
	void exec();
private:
	const float m_frameRate{ 100.0f / 6.0f };

	std::unique_ptr<ComponentManager> m_compManager;
	sf::RenderWindow m_window;
	std::vector<std::unique_ptr<ISystem>> m_sys;
	std::vector<std::shared_ptr<Entity>> m_entity;
	sf::Clock m_clock;
	Factory m_factory;

	Vector2 getJoystickInput();

	void onRSCall(const Events::RSCallEvent *);
	void onQueryEntityByTag(Events::QueryEntityByTagEvent *);
	
	void loadTestData(const std::string &fName);
};