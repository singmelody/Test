#pragma once
#include "Singleton.h"
#include "WorldStateGame.h"

// pull data from node ( when node is leaving etc..)
class WorldState_Decommission : public WorldStateGameNode, public Singleton<WorldState_Decommission>
{
public:
	WorldState_Decommission(void);
	~WorldState_Decommission(void);
};

