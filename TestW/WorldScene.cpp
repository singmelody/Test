#include "StdAfx.h"
#include "WorldScene.h"
#include "PacketImpl.h"
#include "ParamPool.h"
#include "WorldAvatar.h"
#include "WorldSceneInfo.h"
#include "WorldSceneManager.h"

void WorldScene::EnterScene(WorldAvatar* pAvatar)
{
	pAvatar->SetScene( this );
	pAvatar->SetPendingCreateScene(SCENE_ID_NULL);
	AddPlayerCount(1);

	((WorldSceneInfo*)m_pSceneInfo)->OnEnterScene( pAvatar, this);
}

void WorldScene::ExitScene(WorldAvatar* pAvatar)
{
	pAvatar->SetScene(NULL);
	AddPlayerCount(-1);

	((WorldSceneInfo*)m_pSceneInfo)->OnLeaveScene( pAvatar, this);
}


void WorldScene::NotifyWaitingAvatars(int32 nErrorID)
{
	std::deque<int32>& queue = m_WaitingQueue;

	while (!queue.empty() )
	{
		int32 nAvatarID = queue[0];
		queue.pop_front();

		WorldAvatar* pAvatar = GetWorldAvatar(nAvatarID);
		if(pAvatar != NULL)
			pAvatar->HandleCreateSceneResult( nErrorID, this);
	}
}

void WorldScene::AddWaitingAvatar(WorldAvatar* pAvatar)
{
	m_WaitingQueue.push_back( pAvatar->GetAvatarID() );
	pAvatar->SetCurState( eWS_WaitScene );
	pAvatar->SetPendingCreateScene( GetSceneID());
}

WorldScene* GetWorldScene(int32 nSceneID)
{
	WorldScene* pScene = WorldSceneManager::Instance().GetWorldScene(nSceneID);
	return pScene;
}
