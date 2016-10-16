#pragma once
#include "Singleton.h"
class WorldTeamManager : public Singleton<WorldTeamManager>
{
public:
	WorldTeamManager(void);
	~WorldTeamManager(void);
};

