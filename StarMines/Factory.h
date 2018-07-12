#pragma once

#include <vector>
#include <string>
#include "Entity.h"
#include "EventSystem.h"

class ComponentManager;
class GameState;
class Entity;

class Factory : public EventHandler
{
public:
	Factory(GameState *gameState, const std::string &fName);
	void createFromBlueprint(const std::string &blueprint, float x = 0.0f, float y = 0.0f, std::vector<int> *initData = nullptr, bool cache = true, Entity::PersistType persist = Entity::PersistType::None, BehaviorComponent *bc = nullptr);
	void activateFromBlueprint(const std::string &blueprint, float x = 0.0f, float y = 0.0f, std::vector<int> *initData = nullptr, Entity::PersistType persist = Entity::PersistType::Default, BehaviorComponent *bc = nullptr);
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
	void addInitCall(Entity *e, std::vector<int> *initData, BehaviorComponent *bc = nullptr);
};