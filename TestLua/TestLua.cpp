// TestLua.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LuaVM.h"
#include "LuaManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	LuaManager::Instance().InitLua(LuaVM::GetMainLuaVM());
	LuaVM::GetMainLuaVM().do_file("app.lua");

	luabridge::LuaRef ref = LuaVM::GetMainLuaVM().getGlobal("LuaOnStart");
	if( !ref.isNil() )
	{
		try
		{
			ref();
		}
		catch (luabridge::LuaException const& e)
		{
			printf("call LuaOnStart:%s", e.what());	
		}
	}
	return 0;
}

