#pragma once

#include <vector>
#include <memory>
#include "Systems.h"
#include "Entity.h"
#include "ComponentManager.h"
#include "SFML\System.hpp"
#include "Factory.h"

class GameState
{
	friend class Factory;
public:
	GameState();
	void exec();
private:
	std::unique_ptr<ComponentManager> m_compManager;
	sf::RenderWindow m_window;
	std::vector<std::unique_ptr<ISystem>> m_sys;
	std::vector<std::shared_ptr<Entity>> m_entity;
	sf::Clock m_clock;
	Factory m_factory;
};