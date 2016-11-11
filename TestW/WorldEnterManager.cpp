#include "StdAfx.h"
#include "WorldEnterManager.h"
#include "WorldSceneInfo.h"
#include "PacketImpl.h"
#include "WorldScene.h"
#include "WorldStateManager.h"
#include "WorldAvatar.h"
#include "WorldStateManager.h"
#include "WorldServer.h"

WorldEnterManager::WorldEnterManager(void)
{
}


WorldEnterManager::~WorldEnterManager(void)
{
}

bool WorldEnterManager::HandleEnterScene(WorldAvatar* pAvatar, WorldScene* pScene)
{
	if(!pScene)
		return false;

	int32 nTarNodeID = pScene->GetNodeID();
	int32 nTarSceneID = pScene->GetSceneID();

	WorldSceneInfo* pInfo = (WorldSceneInfo*)( pScene->GetSceneInfo() );
	if(!pInfo)
		return false;

	assert( nTarNodeID != SrvID_NULL );

	const Vector3& pos = pAvatar->GetTargetScenePoint();
	const Vector3& dir = pAvatar->GetTargetSceneDir();

	WorldScene* pOldScene = (WorldScene*)pAvatar->GetScene();

	if( pOldScene != NULL )
	{
		WorldSceneInfo* pInfo = (WorldSceneInfo*)(pOldScene->m_pSceneInfo);
		pInfo->OnLeaveScene( pAvatar, pOldScene);

		if( pOldScene->GetNodeID() == nTarNodeID)
		{
			// target scene and current scene on the same node
			pOldScene->ExitScene(pAvatar);

			pAvatar->SetCurState( eWS_Jumping );

			PacketShortChangeScene pkt;

			pkt.SetAvatarID( pAvatar->GetAvatarID() );
			pkt.targetScene = nTarSceneID;
			pkt.x = pos.x;
			pkt.y = pos.y;
			pkt.z = pos.z;
			pkt.dx = dir.x;
			pkt.dy = dir.y;
			pkt.dz = dir.z;

			Send2Node( pkt, nTarNodeID);
			return true;
		}
	}
	
	pAvatar->m_bTargetNodeAvatarCreated = false;
	pAvatar->m_nTargetNodeID = nTarNodeID;
	pAvatar->m_nTargetSceneID = nTarSceneID;

	// target node create avatar
	PacketNodeCreateAvatar pkt;
	pkt.nLoginFlag = ( pOldScene == NULL ? eNode_Avatar_Login : eNode_Avatar_Transport);
	pkt.SetAvatarID(pAvatar->GetAvatarID());
	pkt.nTypeID = pAvatar->GetType();
	
	pkt.nSceneID = nTarSceneID;

	pkt.nWorldID = WorldSrv.GetSrvID();
	pkt.nNodeID = nTarNodeID;
	pkt.nGateSrvID = pAvatar->GetGateSrvID();
	pkt.nGateChannelID = pAvatar->GetGateChannelID();

	pkt.x = pos.x;
	pkt.y = pos.y;
	pkt.z = pos.z;

	pkt.dx = dir.x;
	pkt.dy = dir.y;
	pkt.dz = dir.z;
	pkt.nState = pAvatar->m_nChangeSceneState;
	Send2Node( pkt, nTarNodeID);

	if (pOldScene == NULL)
	{
		pAvatar->OnAvatarEnterGame();
		pAvatar->SetCurState(eWS_EnterGame);
	}
	else
	{
		pAvatar->SetCurState( eWS_Decommission );
	}

	return true;

}
