#pragma once

#include <string>
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
};