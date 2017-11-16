#pragma once

#include <string>
#include "Vector2.h"
#include "RuffCommon.h"

struct EventBase
{
	virtual ~EventBase()
	{}
};


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
		Entity *response;
	};

	struct JoystickEvent : public EventBase
	{
		JoystickEvent()
		{}
		JoystickEvent(float _x, float _y, float _u, float _v) :
			x{ _x }, y{ _y }, u{ _u }, v{ _v }
		{}
		float x;
		float y;
		float u;
		float v;
	};
};