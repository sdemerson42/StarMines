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
};