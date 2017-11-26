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
		_if = 7,
		_and = 8,
		_or = 9,
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
		_equal = 21,
		_greater = 22,
		_less = 23,
		_notEqual = 24,

		label = 50,
		semi = 51,
		var = 52,
		sublabel = 53,

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
		letDir = 111,
		letPos = 112,
		despawn = 115,
		posBound = 116,
		setTargetTag = 113,
		letTargetPos = 114,
		setTargetCaller = 117,
		letInputAxis = 118,
		letCallData = 119,
		lock = 120,
		unlock = 121,
		playSound = 122,
		stopSound = 123,
		setText = 124,
		appendText = 125,
		setTextVal = 126,
		appendTextVal = 127,
		sendSceneSpawnData = 128,

		strbegin = 1000,
		strend = 1001
	};

	struct ByteCode
	{
		std::vector<int> code;
		std::map<std::string, int> label;
	};

	struct Call
	{
		Call()
		{}
		Call(const Call &r) :
			caller{ r.caller }, label{ r.label }, data{ r.data }
		{}
		Call &operator=(const Call &r)
		{
			caller = r.caller;
			label = r.label;
			data = r.data;
			return *this;
		}
		Entity *caller;
		std::string label;
		std::vector<int> data;
	};
}