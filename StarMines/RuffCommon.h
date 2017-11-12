#pragma once

#include <map>
#include <vector>
#include <string>

class Entity;

namespace Ruff
{
	enum Code
	{
		prim = 0,
		add = 1,
		sub = 2,
		mult = 3,
		div = 4,
		mod = 5,
		let = 6,
		ifEquals = 7,
		ifLess = 8,
		ifGreater = 9,
		inc = 10,
		dec = 11,
		halt = 12,
		rand = 13,
		jump = 14,
		def = 15,
		retSub = 17,
		pause = 18,
		sleep = 19,
		ifCallerTag = 20,

		label = 50,
		semi = 51,
		var = 52,
		sublabel = 53,
		strbegin = 54,
		strend = 55,

		log = 100,
		logStr = 101,
		sendCall = 102,
		setDir = 103,
		playAnim = 104,
		sendTag = 105,
		setPos = 106,
		addPos = 107,
		spawn = 108,
		setSpeed = 109,
		letSpeed = 110,
		letDirX = 111,
		letDirY = 112,
		letPosX = 113,
		letPosY = 114,
		despawn = 115

	};

	struct ByteCode
	{
		std::vector<int> code;
		std::map<std::string, int> label;
	};

	struct Call
	{
		Entity *caller;
		std::string label;
	};
}