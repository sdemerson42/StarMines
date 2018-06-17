#pragma once

#include "ISystem.h"
#include "Vector2.h"

class Behavior : public ISystem
{
public:
	Behavior(ComponentManager *cm);
	void update() override;
	static const std::string &sceneName();
private:
	void onInputEvent(Events::InputEvent *);
	void onSceneChangeEvent(Events::SceneChangeEvent *);
	static std::string m_sceneName;
};
