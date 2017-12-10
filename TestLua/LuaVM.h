#pragma once

#include "lua-5.3.3/src/lua.hpp"
#include "luabridge/LuaBridge.h"

class LuaVM
{
public:
	explicit LuaVM(lua_State* pL = NULL);
	~LuaVM(void);

	lua_State* Get() const { return m_lua; }
	operator lua_State* (){
		return m_lua;
	}

	void Init(lua_State* pL = NULL);
	void SetLoadFilePath(const std::string& strPath) { m_strPath = strPath; }
	const std::string& GetLoadFilePath() const { return m_strPath; }

	bool load_file(const std::string& strFile);
	bool do_file(const std::string& strFile);

	luabridge::LuaRef getGlobal(const std::string& strName);
	static class LuaVM& GetMainLuaVM()
	{
		return s_LuaVM;
	}
private:
	static LuaVM	s_LuaVM;
	bool			m_bIsOwner;
	lua_State*		m_lua;
	std::string		m_strPath;
};

