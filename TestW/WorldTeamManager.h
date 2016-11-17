#pragma once
#include "Singleton.h"
#include "ID2ItemMap.h"
#include "WorldFuncExManager.h"
#include "WorldTeam.h"

class WorldTeamManager : public ID2ItemMap< int64, WorldTeam>, public WorldFuncExManager, public Singleton<WorldTeamManager>
{
public:
	WorldTeamManager(void);
	~WorldTeamManager(void);

	WorldTeam* GetWorldTeam(int64 nTeamID) { return (WorldTeam*)GetItem(nTeamID);}
};

