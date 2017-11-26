#pragma once

#include <vector>
#include <string>
#include "Entity.h"

class ComponentManager;
class GameState;
class Entity;

class Factory
{
public:
	Factory(GameState *gameState, const std::string &fName);
	void createFromBlueprint(const std::string &blueprint, float x = 0.0f, float y = 0.0f, std::vector<int> *initData = nullptr, bool cache = true, Entity::PersistType persist = Entity::PersistType::None);
	void activateFromBlueprint(const std::string &blueprint, float x = 0.0f, float y = 0.0f, std::vector<int> *initData = nullptr);
	void deactivate(Entity *e);

	void buildScene(const std::string &name);
	void clearScene();

	struct CompData
	{
		std::string type;
		std::vector<std::string> data;
	};
	struct Blueprint
	{
		std::string name;
		std::vector<std::string> tag;
		std::vector<CompData> compData;
	};
private:
	GameState *m_gameState;
	std::vector<Blueprint> m_blueprint;
	void addInitCall(Entity *e, std::vector<int> *initData);
};