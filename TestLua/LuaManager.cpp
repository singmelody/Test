#include "StdAfx.h"
#include "LuaManager.h"
#include "LuaVM.h"
#include "ExportLua.h"

void lua_register_1(LuaVM& L)
{

}

void lua_register_2(LuaVM& L)
{

}

void lua_register_3(LuaVM& L)
{

}

void lua_register_4(LuaVM& L)
{

}

LuaManager::LuaManager(void)
{
}


LuaManager::~LuaManager(void)
{
}

void LuaManager::InitLua(class LuaVM& L)
{
	L.SetLoadFilePath("./scripts/node/");

	register_CommonClass(L);

	lua_register_1(L);
	lua_register_2(L);
	lua_register_3(L);
	lua_register_4(L);

	luabridge::getGlobalNamespace(L)
		.beginClass<TestLua>("TestLua")
			.addStaticFunction( "Echo2", &TestLua::Echo2)
		.endClass();
}
