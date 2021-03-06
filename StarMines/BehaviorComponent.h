#pragma once

#include "IComponent.h"
#include "Events.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "RuffCommon.h"
#include "RenderComponent.h"

class Entity;

class BehaviorComponent : public IComponent
{
	friend class Behavior;
public:
	BehaviorComponent() :
		m_target{ nullptr }
	{
		registerFunc(this, &BehaviorComponent::onQueryEntityByTag);
		m_call.reserve(64);
		m_regMapIndex = 0;
	}

	static void setInput(Events::InputEvent *evnt)
	{
		m_input = *evnt;

		if (abs(m_input.xAxis) < m_axisDeadzone) m_input.xAxis = 0.0f;
		if (abs(m_input.yAxis) < m_axisDeadzone) m_input.yAxis = 0.0f;
		if (abs(m_input.uAxis) < m_axisDeadzone) m_input.uAxis = 0.0f;
		if (abs(m_input.vAxis) < m_axisDeadzone) m_input.vAxis = 0.0f;
	}

	void initialize(const std::vector<std::string> &input) override
	{
		m_luaModule = input[0];
		reactivate();
	}
	void reactivate() override
	{
		m_call.clear();
		m_pendingCall.clear();
		for (int i = 0; i < m_registerCount; ++i)
		{
			m_register[i].i = 0;
			m_register[i].f = 0.0f;
		}
		m_regMap.clear();
		m_regMapIndex = 0;
		setActive(true);
	}

	void update()
	{
		// Late update
		m_call.clear();
		m_call = m_pendingCall;
		m_pendingCall.clear();
	}
	const std::string &getTag() const override
	{
		return m_tag;
	}
	
	// Lua Interface Functions

	const std::string &module();
	const Vector2 &position();
	void setPosition(float x, float y);
	float speed();
	void setSpeed(float x);
	const Vector2 &dir();
	void setDir(float x, float y);
	const Ruff::Call &getCall();
	void clearCalls();
	void sendToTag(const std::string &tag, const std::string &label, const std::string &sdata);
	void sendToCaller(const std::string &label, const std::string &sdata);
	void spawn(const std::string &bTag, float x, float y, const std::string &sdata, const std::string &persist);
	void despawn(const std::string &sdata);
	void playSound(const std::string &tag, float volume, bool hi, bool loop);
	void stopSound(const std::string &tag);
	void playMusic(const std::string &tag, float volume, bool loop);
	void stopMusic();
	void playAnim(const std::string &tag);
	void setTargetByCaller();
	void setTargetByTag(const std::string &tag, const std::string &method);
	const Vector2 &targetPosition();
	BehaviorComponent *targetBehavior();
	void deactivate();
	bool active();
	const Events::InputEvent &input() const;
	void setText(const std::string &txt);
	void appendText(const std::string &txt);
	void newScene(const std::string &scene);
	bool globalPersist();
	void createNewScene(const std::string &name, const std::string &prox, const std::string &view);
	void addSceneData(const std::string &name, const std::string &persist, bool cache, int count,
		const std::string &blueprint, float x, float y, const std::string &sInitData);
	void rotate(float angle);
	void scale(float x, float y);
	const RenderComponent::Color &color();
	void setColor(int r, int g, int b, int a);
	const RenderComponent::Color &textColor();
	void setTextColor(int r, int g, int b, int a);
	bool callerHasTag(const std::string &tag);

	// End Lua



	void onMove()
	{
		// Probably no longer necessary
		//m_vm.setParent(this);
	}

	void addCall(Ruff::Call &c)
	{
		m_call.push_back(c);
	}
	void addPendingCall(Ruff::Call &c)
	{
		m_pendingCall.push_back(c);
	}

	std::vector<int> getSceneDespawnData()
	{
		auto r{ m_sceneDespawnData };
		m_sceneDespawnData.clear();
		return r;
	}
	std::vector<int> &refSceneDespawnData()
	{
		return m_sceneDespawnData;
	}

	// Behavior state access methods

	static BehaviorComponent *getCurrentComponent()
	{
		return m_currentComponent;
	}

	int getRegisterInt(const std::string &var) 
	{
		int index;
		auto p = m_regMap.find(var);
		if (p != end(m_regMap))
			index = p->second;
		else
		{
			index = m_regMapIndex;
			m_regMap[var] = m_regMapIndex++;
		}

		return m_register[index].i;
	}
	float getRegisterFloat(const std::string &var) 
	{
		int index;
		auto p = m_regMap.find(var);
		if (p != end(m_regMap))
			index = p->second;
		else
		{
			index = m_regMapIndex;
			m_regMap[var] = m_regMapIndex++;
		}
		return m_register[index].f;
	}
	void setRegisterInt(const std::string &var, int val)
	{
		int index;
		auto p = m_regMap.find(var);
		if (p != end(m_regMap))
			index = p->second;
		else
		{
			index = m_regMapIndex;
			m_regMap[var] = m_regMapIndex++;
		}
		m_register[index].i = val;
	}
	void setRegisterFloat(const std::string &var, float val)
	{
		int index;
		auto p = m_regMap.find(var);
		if (p != end(m_regMap))
			index = p->second;
		else
		{
			index = m_regMapIndex;
			m_regMap[var] = m_regMapIndex++;
		}
		m_register[index].f = val;
	}
	void incRegisterInt(const std::string &var)
	{
		int index;
		auto p = m_regMap.find(var);
		if (p != end(m_regMap))
			index = p->second;
		else
		{
			index = m_regMapIndex;
			m_regMap[var] = m_regMapIndex++;
		}
		++m_register[index].i;
	}
	void decRegisterInt(const std::string &var)
	{
		int index;
		auto p = m_regMap.find(var);
		if (p != end(m_regMap))
			index = p->second;
		else
		{
			index = m_regMapIndex;
			m_regMap[var] = m_regMapIndex++;
		}
		--m_register[index].i;
	}
	std::vector<Ruff::Call> &getCalls()
	{
		return m_call;
	}
	Entity *&target()
	{
		return m_target;
	}
	void broadcastCall(Ruff::Call &c, const std::string &tag);
	void onQueryEntityByTag(const Events::QueryEntityByTagEvent *);
private:

	static std::string m_tag;
	static Events::InputEvent m_input;
	static const float m_axisDeadzone;
	static BehaviorComponent *m_currentComponent;

	void callDataSync(Ruff::Call &c);
	void spawnDataSync(Events::SpawnDataEvent &e);

	std::string m_luaModule;
	std::vector<Ruff::Call> m_call;
	std::vector<Ruff::Call> m_pendingCall;
	Entity *m_target{ nullptr };
	std::vector<int> m_sceneDespawnData;

	// Behavior state for Lua
	union RegisterVal
	{
		float f;
		int i{ 0 };
	};
	static const int m_registerCount{ 128 };
	RegisterVal m_register[m_registerCount];
	std::map<std::string, int> m_regMap;
	int m_regMapIndex;

	// Persisting state for Lua return

	Ruff::Call m_curCall;
};


