#include "StdAfx.h"
#include "WorldStateManager.h"
#include "WorldState_Logining.h"
#include "WorldState_Logined.h"
#include "WorldState_DataLoading.h"

WorldStateManager::WorldStateManager(void)
{
	m_worldState[eWS_Logining] = &WorldState_Logining::Instance();
	m_worldState[eWS_Logined] = &WorldState_Logined::Instance();
	m_worldState[eWS_DataLoading] = &WorldState_DataLoading::Instance();
	m_worldState[eWS_DataReady] = &WorldState_DataReady::Instance();
	m_worldState[eWS_WaitScene] = &WorldState_WaitScene::Instance();
	m_worldState[eWS_Decommission] = &
		m_worldState[eWS_EnterScene,
		m_worldState[eWS_Jumping,
		m_worldState[eWS_Gaming,
		m_worldState[eWS_ExitGame,
		m_worldState[eWS_Count
	m_worldState[




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
