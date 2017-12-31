// TestLua.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LuaVM.h"
#include "LuaManager.h"

typedef  int int32;
typedef  long long int64;
typedef  unsigned int uint32;
typedef  unsigned long long uint64;



void echoBinary(uint32 n)
{
	for ( int32 i=31; i >= 0; --i)
	{
		printf("%d", ( n >> i ) & 1);
		if((i % 4) == 0)
			printf("|");
	}
	printf("\n");
}

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

