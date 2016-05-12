#include "StdAfx.h"
#include "WorldStateManager.h"


WorldStateManager::WorldStateManager(void)
{
}


WorldStateManager::~WorldStateManager(void)
{
}

WorldState* WorldStateManager::GetState(WorldStateID nStateID)
{
	if( nStateID < 0 || nStateID >= eWS_Count)
		return NULL;

	return m_worldState[nStateID];
}
