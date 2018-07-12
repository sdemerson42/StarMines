#pragma once

#include <string>
#include "Vector2.h"
#include "RuffCommon.h"

struct EventBase
{
	virtual ~EventBase()
	{}
};

class BehaviorComponent;

namespace Events
{
	struct RSCallEvent : EventBase
	{
		RSCallEvent()
		{}
		RSCallEvent(const Ruff::Call &_call, const std::string &_tag) :
			call{ _call }, tag{ _tag }
		{}
		Ruff::Call call;
		std::string tag;
	};

	struct SpawnDataEvent : EventBase
	{
		SpawnDataEvent()
		{}
		SpawnDataEvent(const std::string &_blueprint, float x, float y, bool _despawn = false) :
			blueprint{ _blueprint }, position{ x,y }
		{}
		std::string blueprint;
		Vector2 position;
		std::vector<int> initData;
		std::string sInitData;
		std::string persist;
		BehaviorComponent*bc = nullptr;
	};

	struct DespawnEvent : EventBase
	{
		DespawnEvent()
		{}
		DespawnEvent(Entity *_entity) :
			entity{ _entity }
		{}
		Entity *entity;
	};

	struct QueryEntityByTagEvent : EventBase
	{
		QueryEntityByTagEvent()
		{}
		QueryEntityByTagEvent(void *_client, const std::string &_tag, const std::string &_method) :
			client{ _client }, tag{ _tag }, method{ _method }, response{ nullptr }
		{}
		void *client;
		std::string tag;
		std::string method;
		Entity *response{ nullptr };
	};

	struct InputEvent : EventBase
	{
		float xAxis{0.0f};
		float yAxis{ 0.0f };
		float uAxis{ 0.0f };
		float vAxis{ 0.0f };
		float padXAxis{ 0.0f };
		float padYAxis{ 0.0f };
		float zAxis{ 0.0f };
		bool a{ false };
		bool b{ false };
		bool x{ false };
		bool y{ false };
		bool shoulderL{ false };
		bool shoulderR{ false };
		bool select{ false };
		bool start{ false };
		bool stickL{ false };
		bool stickR{ false };
	};

	struct SoundEvent : EventBase
	{
		SoundEvent()
		{}
		SoundEvent(const std::string &_tag, bool _loop, bool _hiPriority, bool _stop = false, float _volume = 50.0f) :
			tag{ _tag }, loop{ _loop }, hiPriority{ _hiPriority }, stop{ _stop }, volume { _volume }
		{}
		std::string tag;
		bool loop;
		bool hiPriority;
		bool stop;
		float volume;
	};

	struct MusicEvent : EventBase
	{
		MusicEvent()
		{}
		MusicEvent(const std::string &_tag, bool _loop, bool _stop = false, float _volume = 50.0f) :
			tag{ _tag }, loop{ _loop }, stop{ _stop }, volume{ _volume }
		{}
		std::string tag;
		bool loop;
		bool stop;
		float volume;
	};

	struct SceneChangeEvent : EventBase
	{
		SceneChangeEvent()
		{}
		SceneChangeEvent(const std::string &_name) :
			name{ name }
		{}
		std::string name;
	};

	struct ProxMapInitEvent : EventBase
	{
		int sceneX;
		int sceneY;
		int cellX;
		int cellY;
	};

	struct ViewEvent : EventBase
	{
		float viewW;
		float viewH;
		float portX;
		float portY;
		float portW;
		float portH;
		float centerX;
		float centerY;
	};

	struct CreateSceneFromScriptEvent : EventBase
	{
		std::string name;
		ProxMapInitEvent prox;
		ViewEvent view;
	};

	struct AddSceneDataEvent : EventBase
	{ 
		std::string name;
		std::string persist;
		bool cache;
		int count;
		std::string blueprint;
		float x;
		float y;
		std::string init;
	};
	
}