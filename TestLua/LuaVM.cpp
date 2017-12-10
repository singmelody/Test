#include "StdAfx.h"
#include "LuaVM.h"

LuaVM LuaVM::s_LuaVM;

LuaVM::LuaVM(lua_State* pL ):m_lua(pL), m_bIsOwner(false)
{
	if(!m_lua)
	{
		m_lua = luaL_newstate();
		luaL_openlibs(m_lua);

		m_bIsOwner = true;
	}
}


LuaVM::~LuaVM(void)
{
	if(m_lua && m_bIsOwner)
	{
		lua_gc( m_lua, LUA_GCCOLLECT, 0);
		lua_close(m_lua);
	}

	m_lua = NULL;
}

bool LuaVM::load_file(const std::string& strFile)
{
	assert(m_lua);
	return 0 == luaL_loadfile( m_lua, strFile.c_str());
}

bool LuaVM::do_file(const std::string& strFile)
{
	assert(m_lua);
	std::string path = (m_strPath + strFile).c_str();
	printf("Lua load File %s", path.c_str());
	return 0 == luaL_dofile( m_lua, path.c_str());
}

luabridge::LuaRef LuaVM::getGlobal(const std::string& strName)
{
	return luabridge::getGlobal( m_lua, strName.c_str());
}

