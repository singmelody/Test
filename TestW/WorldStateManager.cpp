#include "StdAfx.h"
#include "WorldStateManager.h"
#include "WorldState_Logining.h"

WorldStateManager::WorldStateManager(void)
{
	m_worldState[eWS_Logining] = &WorldState_Logining::Instance();
	m_worldState[eWS_Logined] = &WorldState_Logined::Instance();
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
