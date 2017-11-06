#pragma once

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
		retsub = 17,
		pause = 18,

		label = 50,
		semi = 51,
		var = 52,
		sublabel = 53,
		strbegin = 54,
		strend = 55,

		log = 100,
		logstr = 101
	};
}