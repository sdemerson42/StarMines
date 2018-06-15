#include "LuaWrapper.h"

luabridge::lua_State *LuaWrapper::L;
std::vector<BehaviorComponent> *LuaWrapper::bcomps;
int *LuaWrapper::bcompSz;