#include "StdAfx.h"
#include "WorldStateManager.h"
#include "WorldState_Logining.h"
#include "WorldState_Logined.h"
#include "WorldState_DataLoading.h"
#include "WorldState_DataReady.h"
#include "WorldState_WaitScene.h"
#include "WorldState_Decommission.h"
#include "WorldState_EnterGame.h"
#include "WorldState_Jumping.h"
#include "WorldState_Gaming.h"
#include "PacketProcessor.h"

WorldStateManager::WorldStateManager(void)
{
	m_worldState[eWS_Logining] = &WorldState_Logining::Instance();
	m_worldState[eWS_Logined] = &WorldState_Logined::Instance();
	m_worldState[eWS_DataLoading] = &WorldState_DataLoading::Instance();
	m_worldState[eWS_DataReady] = &WorldState_DataReady::Instance();
	m_worldState[eWS_WaitScene] = &WorldState_WaitScene::Instance();
	m_worldState[eWS_Decommission] = &WorldState_Decommission::Instance();
	m_worldState[eWS_EnterGame] = &WorldState_EnterGame::Instance();
	m_worldState[eWS_Jumping] = &WorldState_Jumping::Instance();
	m_worldState[eWS_Gaming] = &WorldState_Gaming::Instance();
	m_worldState[eWS_ExitGame] = &WorldState_ExitGame::Instance();
	m_worldState[eWS_Billing] = &WorldState_Billing::Instance();
	m_worldState[eWS_WaitingLogining] = &WorldState_WaitLogining::Instance();
	m_worldState[eWS_WaitingNodeData] = &WorldState_WaitingNodeData::Instance();
	m_worldState[eWS_WritingDBA] = &WorldState_WritingDBA::Instance();
}


WorldStateManager::~WorldStateManager(void)
{
}

WorldState* WorldStateManager::GetState(int32 nStateID)
{
	if( nStateID < 0 || nStateID >= eWS_Count)
		return NULL;

	return m_worldState[nStateID];
}

void WorldStateManager::OnNodeCrash()
{

}

void WorldStateManager::RegPeerPktHandle(PacketProcessor* pProc)
{
	for (int32 i = 0; i < eWS_Count; ++i)
	{
		WorldState* pState = m_worldState[i];
		if(!pState)
			continue;

		pState->RegPeerPktHandle( pProc);
	}


}
