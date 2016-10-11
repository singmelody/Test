#include "StdAfx.h"
#include "WorldEnterManager.h"
#include "WorldSceneInfo.h"
#include "PacketImpl.h"
#include "WorldScene.h"
#include "WorldStateManager.h"

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

	Vector3& pos = pAvatar->GetTargetScenePoint();
	Vector3& dir = pAvatar->GetTargetSceneDir();

	WorldScene* pOldScene = (WorldScene*)pAvatar->GetScene();

	if( pOldScene != NULL )
	{
		WorldSceneInfo* pInfo = (WorldSceneInfo*)(pOldScene->m_pSceneInfo);
		pOldScene->OnLeaveScene( pAvatar, pOldScene);

		if( pOldScene->GetNodeID() == nTarNodeID)
		{
			// target scene and current scene on the same node
			pOldScene->ExitScene(eWS_Jumping);

			PacketShortChangeScene pkt;

			pkt.SetAvatarID( pAvatar->GetAvatarID() );
			pkt.targetScene = target_scene_id;
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

	return;

}
