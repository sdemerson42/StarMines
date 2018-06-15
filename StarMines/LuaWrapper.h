#pragma once

#include "LuaBridge.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <vector>

class BehaviorComponent;


struct LuaWrapper
{
	static luabridge::lua_State *L;
	static std::vector<BehaviorComponent> *bcomps;
	static int *bcompSz;
};
