#pragma once

#include "Singleton.h"
#include "WorldFuncManager.h"
#include "LoginActionDefine.h"
#include <map>
#include <vector>

class WorldLoginActionManager : public WorldFuncManager, public Singleton<WorldLoginActionManager>
{
public:
	WorldLoginActionManager(void);
	~WorldLoginActionManager(void);

	void Save2DB();

protected:
	std::map< int64, std::vector<LoginActionData> > m_LoginActionList; 
};

