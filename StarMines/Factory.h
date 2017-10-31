#pragma once

#include <vector>
#include <string>

class ComponentManager;
class GameState;

class Factory
{
public:
	Factory(GameState *gameState, const std::string &fName);
	void createFromBlueprint(const std::string &blueprint, float x = 0.0f, float y = 0.0f);
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
};