#include "StdAfx.h"
#include "WorldSceneManager.h"
#include "SceneInfo.h"


WorldSceneManager::WorldSceneManager(void)
	: AvatarMgr( WorldAvatarManager::Instance())
{
}


WorldSceneManager::~WorldSceneManager(void)
{
}

void WorldSceneManager::OnSceneCreated(Scene* pScene)
{
	SceneManager::OnSceneCreated(pScene);

	NodeSrvInfo* pInfo = (NodeSrvInfo*)Servers.GetNodeInfo( pScene->GetNodeID() );
	if(!pInfo)
		return;

	pInfo->AddupSceneLoadValue( pScene, true);
}

void WorldSceneManager::OnSceneDestroy(Scene* pScene)
{
	SceneManager::OnSceneDestroy(pScene);

	NodeSrvInfo* pInfo = (NodeSrvInfo*)Servers.GetNodeInfo( pScene->GetNodeID() );
	if(!pInfo)
		return;

	pInfo->AddupSceneLoadValue( pScene, false);
}

SceneInfo* WorldSceneManager::CreateSceneInfo(int32 nSceneType, int32 nPlayerMax)
{
	SceneInfo* pInfo = NULL;

	switch(nSceneType)
	{
	case SceneInfo::SceneCreate_MainTrunk:
		pInfo = new WorldSceneInfo_MainTrunk();
		break;

	case SceneInfo::SceneCreate_TeamCopy:
		if(playerMax > 1)
			pInfo = new WorldSceneInfo_TeamCopy();
		else
			pInfo = new WorldSceneInfo_ScenarioCopy();
		break;
	case SceneInfo::SceneCreate_TokenCopy:
		pInfo = new WorldSceneInfo_TokenCopy();
		break;
	case SceneInfo::SceneCreate_Scenario:
		pInfo = new WorldSceneInfo_ScenarioCopy();
		break;
	case SceneInfo::SceneCreate_TrunkCopy:
		pInfo = new WorldSceneInfo_TrunkCopy();
		break;
	case SceneInfo::SceneCreate_RootCopy:
		pInfo = new WorldSceneInfo_RootCopy();
		break;
	}
}

void WorldSceneManager::TickCreatingScenes(int32 nFrameTime)
{
	CreatingSceneMap& map = m_mapCreatingScenes;

	for (auto itr = map.beign(); itr != map.end(); ++itr)
	{
		WorldScene* pScene = itr->second;

		int32 nPendingTime = pScene->GetPendingTime();
		if( nPendingTime <= 0 )
		{
			map.RemoveItem( pScene->GetSceneID() );
			OnCreatingSceneTimeout( pScene );
			break;
		}
		else
			pScene->SetPendingTime( nPendingTime - nFrameTime );
	}
}

void WorldSceneManager::TickClosingScenes(int32 nFrameTime)
{

}

void WorldSceneManager::OnCreatingSceneTimeout(WorldScene* pScene)
{
	int32 nSceneID = pScene->GetSceneID();

	MyLog::error("Delete Pending Scene id = %d", nSceneID);

	// close process
	WorldSceneInfo* pInfo = (WorldSceneInfo*)pScene->GetSceneInfo();
	if(!pInfo)
		return;

	pInfo->OnSceneCreatedFailed( pScene, CreateSceneError_Timeout);

	PacketDestroyScene pkt;
	pkt.SceneID = pScene->GetSceneID();
	Send2Node( &pkt, pScene->GetNodeID() );
	DestroyScene( nSceneID );
}

void WorldSceneManager::NotifyCreateSceneResult(int32 nNodeID, int32 nAvatarID, int32 nResult, WorldScene* pScene /*= NULL*/)
{

}

void WorldSceneManager::SyncParallelInfo2Node( int32 nNodeSrvID)
{
	for(auto itr = SceneInfos.begin(); itr != SceneInfos.end() ; ++itr)
	{
		SceneInfoEx* pInfo = (SceneInfoEx*)itr->second;
		if(!pInfo->IsMainTrunk() )
			continue;

		if(pInfo->m_ParallelBits == 0)
			continue;

		((WorldSceneInfo_MainTrunk*)pInfo)->TryStartParallel();

		PacketParallelInfo pkt;
		pkt.sceneSID = pInfo->m_SceneSID;
		pkt.parallelBits = pInfo->m_ParallelBits;

		Send2Node( &pkt, nNodeSrvID);
	}
}

bool WorldSceneManager::GetSceneCreateParam( int16 nSceneSID, SceneCreateArg& obj)
{
	if( nSceneSID == SCENE_ID_NULL )
		nSceneSID = 1;

	const SceneInfo* pSD = GetSceneInfo(nSceneSID);
	if(!pSD)
		return false;

	pSD->FillSceneCreateArg(obj);

	return true;

}

WorldScene* WorldSceneManager::CreateWorldScene( SceneCreateArg& obj)
{
	int32 nNodeSrvID = arg.m_NodeSrvID;
	INT32 nSceneID = arg.m_SceneID;

	ServerInfo* pNodeInfo =  Servers.GetNodeInfo(nNodeSrvID);
	if(!pNodeInfo)
		return NULL;	// target srv not register  

	balfadfa
}

void WorldSceneManager::OnNodeCrash(int32 nNodeID /*= SERVERID_NULL*/)
{

}
