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
	~GameState()
	{
		std::cout << "Deleting GameState...\n";
		m_sys.clear();
		std::cout << "Systems deleted...\n";
		m_entity.clear();
		std::cout << "Entities deleted...\n";
	}
	void exec();
private:
	const float m_frameRate{ 100.0f / 6.0f };
	std::string m_name;
	std::string m_nextName;
	bool m_sceneChange;

	std::unique_ptr<ComponentManager> m_compManager;
	sf::RenderWindow m_window;
	std::vector<std::unique_ptr<ISystem>> m_sys;
	std::vector<std::shared_ptr<Entity>> m_entity;
	sf::Clock m_clock;
	Factory m_factory;

	struct SceneSpawnData
	{
		Events::SpawnDataEvent spawnData;
		Entity::PersistType persist;
		bool cache;
	};
	struct SceneData
	{
		std::string name;
		Events::ProxMapInitEvent prox;
		Events::ViewEvent view;
		std::vector<SceneSpawnData> data;
	};
	std::vector<SceneData> m_sceneData;

	void onRSCall(const Events::RSCallEvent *);
	void onQueryEntityByTag(Events::QueryEntityByTagEvent *);
	void onSceneChangeEvent(const Events::SceneChangeEvent *);
	void onCreateNewSceneFromScript(const Events::CreateSceneFromScriptEvent *);
	void onAddSceneData(const Events::AddSceneDataEvent *);

	void buildScene(const std::string &name);
	void loadSceneData(const std::string &fName);
};