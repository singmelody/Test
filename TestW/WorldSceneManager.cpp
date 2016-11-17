#include "StdAfx.h"
#include "WorldSceneManager.h"
#include "SceneInfo.h"
#include "WorldAvatarManager.h"
#include "NodeSrvGrp.h"
#include "Scene.h"
#include "ServerManager.h"
#include "WorldSceneInfoTypes.h"
#include "PacketImpl.h"
#include "MyLog.h"
#include "WorldScene.h"
#include "SceneManagerBase.h"
#include "PacketProcessor.h"
#include "TimeManager.h"
#include "WorldServer.h"
#include "WorldSceneTypes.h"

WorldSceneManager::WorldSceneManager(void)
	: AvatarMgr( WorldAvatarManager::Instance())
{
}


WorldSceneManager::~WorldSceneManager(void)
{
}

void WorldSceneManager::RegPeerPktHandle(class PacketProcessor* pProc)
{
	{WorldScene scene;}
	{WorldScene_Copy scene;}
	{WorldScene_PersonalCopy scene;}
	{WorldScene_TeamCopy scene;}
	{WorldScene_GuildCopy scene;}

	REG_PACKET_HANDLER( pProc, PacketCreateSceneResult, WorldSceneManager, PktNode_CreateNodeSceneRst);
	REG_PACKET_HANDLER( pProc, PacketSceneProcessBits, WorldSceneManager, PktNode_SceneProcessBits);
	REG_PACKET_HANDLER( pProc, PacketDestroySceneRequest, WorldSceneManager, PktNode_DestroySceneRequest);

	REG_PACKET_HANDLER( pProc, PacketWarScene, WorldSceneManager, PktNode_WarScene);

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
	case SceneInfo::eSceneCreate_MainTrunk:
		pInfo = new WorldSceneInfo_MainTrunk();
		break;

	case SceneInfo::eSceneCreate_TeamCopy:
		if(nPlayerMax > 1)
			pInfo = new WorldSceneInfo_TeamCopy();
		else
			pInfo = new WorldSceneInfo_ScenarioCopy();
		break;
	case SceneInfo::eSceneCreate_TokenCopy:
		pInfo = new WorldSceneInfo_TokenCopy();
		break;
	case SceneInfo::eSceneCreate_ScenarioCopy:
		pInfo = new WorldSceneInfo_ScenarioCopy();
		break;
	case SceneInfo::eSceneCreate_TrunkCopy:
		pInfo = new WorldSceneInfo_MainTrunkCopy();
		break;
	case SceneInfo::eSceneCreate_RootCopy:
		pInfo = new WorldSceneInfo_RootCopy();
		break;
	}

	return pInfo;
}

void WorldSceneManager::TickCreatingScenes(int32 nFrameTime)
{
	CreatingSceneMap& map = m_mapCreatingScenes;

	for (auto itr = map.begin(); itr != map.end(); ++itr)
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

	pInfo->OnSceneCreateFailed( pScene, eCreateSceneError_Timeout);

	PacketDestroyScene pkt;
	pkt.nSceneID = pScene->GetSceneID();
	Send2Node( &pkt, pScene->GetNodeID() );
	DestroyScene( nSceneID );
}

void WorldSceneManager::NotifyCreateSceneResult(int32 nNodeID, int32 nAvatarID, int32 nResult, WorldScene* pScene /*= NULL*/)
{
	PacketCreateSceneResult pkt;

	pkt.SetAvatarID( nAvatarID );

	if( pScene == NULL )
	{
		pkt.nSceneID = SCENE_ID_NULL;
		pkt.nSceneProcessBits = 0;
	}
	else
	{
		pkt.nSceneID = pScene->GetSceneID();
		pkt.nSceneProcessBits = pScene->m_nSceneProcessBits;
	}

	pkt.nResult = nResult;

	Send2Node( pkt, nNodeID);
}

void WorldSceneManager::SyncParallelInfo2Node( int32 nNodeSrvID)
{
	for(auto itr = SceneInfos.begin(); itr != SceneInfos.end() ; ++itr)
	{
		SceneInfoEx* pInfo = (SceneInfoEx*)itr->second;
		if(!pInfo->IsMainTrunk() )
			continue;

		if(pInfo->m_nParallelBits == 0)
			continue;

		((WorldSceneInfo_MainTrunk*)pInfo)->TryStartParallel();

		PacketParallelInfo pkt;
		pkt.nSceneSID = pInfo->m_nSceneSID;
		pkt.nParallelBits = pInfo->m_nParallelBits;

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

WorldScene* WorldSceneManager::CreateWorldScene( SceneCreateArg& arg)
{
	int32 nNodeSrvID = arg.m_NodeSrvID;
	int32 nSceneID = arg.m_SceneID;

	ServerInfo* pNodeInfo =  Servers.GetNodeInfo(nNodeSrvID);
	if(!pNodeInfo)
		return NULL;	// target srv not register  

	WorldScene* pScene = (WorldScene*)CreateScene(arg);
	if(!pScene)
		return NULL;

	pScene->SetSceneState( Scene::eSceneState_Creating );
	const SceneInfo* pInfo = pScene->GetSceneInfo();

	pScene->SetNodeID( nNodeSrvID );
	pScene->SetPendingTime( MAX_SCENE_CREATE_PENDING_TIME );
	pScene->SetRequestTime(TimeManager::Instance().CurTime());
	pScene->SetProcessBits( arg.m_nSceneProcessBits );

	m_mapCreatingScenes.AddItem( nSceneID, pScene);

	PacketCreateNodeScene pkt;
	pkt.nNodeSrvID = nNodeSrvID;
	pkt.nSceneID = pScene->GetSceneID();
	pkt.nSceneLv = pScene->GetSceneLevel();
	pkt.nArenaID = pScene->GetArenaID();
	pkt.nRequestTime = pScene->GetRequestTime();
	pkt.nSceneCustomData = arg.m_nSceneCustomData;

	WorldSrv.Send2Node( &pkt, nNodeSrvID);

	return pScene;
}

WorldSceneInfo* WorldSceneManager::GetWorldSceneInfo(uint16 nSceneSID)
{
	return (WorldSceneInfo*)GetSceneInfo(nSceneSID);
}

void WorldSceneManager::OnNodeCrash(int32 nNodeID /*= SERVERID_NULL*/)
{
	if(nNodeID == SERVERID_NULL )
		return;

	SceneDataMap& map = SceneInfos;

	for (SceneDataMap::iterator itr = map.begin(); itr != map.end(); ++itr)
	{
		SceneInfoEx* pInfo = (SceneInfoEx*)itr->second;
		assert(pInfo);

		SceneInstanceMgr& mgr = pInfo->m_Instances;

		for (auto subItr = mgr.begin(); subItr != mgr.end(); ++subItr)
		{
			Scene* pScene = subItr->second;
			if(!pScene)
				continue;

			assert( pInfo == pScene->GetSceneInfo() );

			if(pScene->GetNodeID() == nNodeID )
				pScene->SetSceneState( Scene::eSceneState_Closing );
		}
	}
}

void WorldSceneManager::PktNode_CreateNodeSceneRst(class PacketCreateSceneResult* pPkt)
{
	
}

void WorldSceneManager::PktNode_SceneProcessBits(class PacketSceneProcessBits* pPkt)
{

}

void WorldSceneManager::PktNode_DestroySceneRequest(class PacketDestroySceneRequest* pPkt)
{

}

void WorldSceneManager::PktNode_WarScene(class PacketWarScene* pPkt)
{

}
