#pragma once

#include "Singleton.h"

void lua_register_1(class LuaVM& L);
void lua_register_2(class LuaVM& L);
void lua_register_3(class LuaVM& L);
void lua_register_4(class LuaVM& L);

class TestLua
{
public:
	static void Echo2()
	{
		printf("Hi I am Test");
	}
};


class LuaManager : public Singleton<LuaManager>
{
public:
	LuaManager(void);
	~LuaManager(void);

	void InitLua(class LuaVM& L);
};

