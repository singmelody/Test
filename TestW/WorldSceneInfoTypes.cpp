#include "StdAfx.h"
#include "ServerManager.h"

#include "WorldSceneInfoTypes.h"
#include "WorldSceneManager.h"
#include "SceneProcess.h"
#include "SceneProcessCont.h"
#include "WorldAvatar.h"
#include "ConfigManager.h"
#include "SceneManager.h"
#include "MyMath.h"
#include "WorldTeamManager.h"
#include <sstream>

WorldSceneInfo_Trunk::WorldSceneInfo_Trunk()
{
	nCreatingSceneCount = 0;
}

void WorldSceneInfo_Trunk::OnSceneCreateSucceed(WorldScene* pScene)
{
	nCreatingSceneCount--;
}

void WorldSceneInfo_Trunk::OnSceneCreateFailed(WorldScene* pScene)
{
	assert( nCreatingSceneCount > 0 );
	nCreatingSceneCount--;
}


int32 WorldSceneInfo_Trunk::HandleCreateParallel(int32 nNodeSrvID)
{
	SceneCreateArg arg;
	FillSceneCreateArg(arg);

	arg.m_nSceneSID = AllocSceneID();
	arg.m_NodeSrvID = nNodeSrvID;
	arg.m_nSceneProcessBits = 0;

	WorldScene* pScene = SceneMgr.CreateWorldScene(arg);
	if(!pScene)
		return eCreateSceneError_SceneInfo;

	nCreatingSceneCount++;
	return eCreateScene_Succeed;
}

int32 WorldSceneInfo_Trunk::GetSceneNormalVolumn()
{
	int32 nVolumn = int32(m_nPlayerMax * 0.8f);
	if( nVolumn == 0)
		nVolumn = 1;

	return nVolumn;
}

int32 WorldSceneInfo_Trunk::GetRequestTotalVolumn()
{
	int32 nTotalVolumn = nCreatingSceneCount * GetSceneNormalVolumn();
	return nTotalVolumn;
}

WorldSceneInfo_MainTrunk::WorldSceneInfo_MainTrunk(void)
{

}


WorldSceneInfo_MainTrunk::~WorldSceneInfo_MainTrunk(void)
{
}

bool WorldSceneInfo_MainTrunk::LoadSceneInfo(DBRow& row)
{
	if( !WorldSceneInfo_Trunk::LoadSceneInfo(row))
		false;

	std::stringstream ss;
	ss<<"maintrunk"<<m_nSceneSID;

	ConfigManager::GetConfigValue( "SceneConfig/ParalletCnt", ss.str(), m_nParalleCnt);
	return true;
}


bool WorldSceneInfo_MainTrunk::TryEnterTargetScene(WorldAvatar* pAvatar, int16 nInstanceID, int32& failreason)
{
	if (pAvatar->GetSceneSID() != m_nSceneSID )
	{
		WorldTeam* pTeam = pAvatar->GetWorldTeam();
		if(pTeam != NULL)
		{
			int64 nTeamID = pAvatar->GetTeamID();
			std::map<int64, int16>::iterator itr = _teamInstance.find(nTeamID);
			if( itr != _teamInstance.end() )
			{
				nInstanceID = itr->second;
			}
		}
	}

	if(WorldSceneInfo_Trunk::TryEnterTargetScene( pAvatar, nInstanceID, failreason))
		return true;

	return TryEnterAnyParallel( pAvatar, failreason);
}

void WorldSceneInfo_MainTrunk::OnSceneCreate(Scene* pScene)
{
	WorldSceneInfo_Trunk::OnSceneCreate(pScene);
	--_parallelCount;
	UpdateParallelBits();
}

void WorldSceneInfo_MainTrunk::OnSceneDestroy(Scene* pScene)
{

}

void WorldSceneInfo_MainTrunk::UpdateParallelBits()
{
	m_nParallelBits = 0;

	for (auto itr = m_Instances.begin(); itr != m_Instances.end(); ++itr)
	{
		Scene* pScene = itr->second;
		uint32 bit = uint32(1) << pScene->InstanceID();

		m_nParallelBits |= bit;
	}

	BroadcastParallel2Nodes();
}

void WorldSceneInfo_MainTrunk::BroadcastParallel2Nodes()
{
	PacketParallelInfo pkt;

	pkt.nSceneSID = m_nSceneSID;
	pkt.nParallelBits = m_nParallelBits;

	ServerManager::Instance().m_LocalNodeGrp.BroadcastPacket( &pkt );
}



bool WorldSceneInfo_MainTrunk::TryEnterAnyParallel(WorldAvatar* pAvatar, int32& nFailReaon)
{
	// when player enter the main line try create other line
	TryStartParallel();

	return true;
}

void WorldSceneInfo_MainTrunk::TryStartParallel()
{
	int32 nCurCnt = (int32)(m_Instances.size());
	if( nCurCnt >= m_nParalleCnt)
		return;

	NodeSrvList& list = ServerManager::Instance().m_LocalNodeGrp.m_listByTrunkValue;

	int32 nNodeCnt = (int32)(list.size());

	int32 nMaxCnt = MIN( nNodeCnt, m_nParalleCnt);	// 不在用一个Node上开启某个主场景的多个分线

	int32 nNewCnt = nMaxCnt - nCurCnt;
	if( nNewCnt <= 0 )
		return;

	// when create new scene, node loadvalue list can be changed, so save the list first
	NodeSrvInfo* infos[20];

	if( nNewCnt > 20 )
		nNewCnt = 20;

	for(int32 i = 0; i < nNodeCnt; ++i)
		infos[i] = list[i];

	for (int32 i = 0; i < nNodeCnt && nNewCnt > 0; ++i)
	{
		NodeSrvInfo* pInfo = infos[i];
		if (!HasSceneOnNodeServer(pInfo->nSrvID))
		{
			int32 nRst = HandleCreateParallel( pInfo->nSrvID );
			if(nRst == eCreateScene_Succeed )
				nNewCnt--;
		}
	}
}

bool WorldSceneInfo_MainTrunk::HasSceneOnNodeServer(int32 nNodeSrvID)
{
	for(SceneInstanceMgr::iterator itr = m_Instances.begin(); itr != m_Instances.end(); ++itr)
	{
		WorldScene* pScene = (WorldScene*)itr->second;
		if( pScene->m_nNodeID == nNodeSrvID )
			return true;
	}

	return false;
}

WorldSceneInfo_TeamCopy::WorldSceneInfo_TeamCopy()
	: TeamMgr(WorldTeamManager::Instance())
{

}

CreateSceneRst WorldSceneInfo_TeamCopy::HandleCreateCopyRequest(WorldAvatar* pAvatar, int64 nSceneProcessBits)
{
	WorldTeam* pTeam = (WorldTeam*)pAvatar->GetTeam();

	if( pTeam && (pTeam->GetMemberCount() > (int32)m_nPlayerMax))
		return eCreateSceneError_Team;

	return WorldSceneInfo_Copy::HandleCreateCopyRequest( pAvatar, nSceneProcessBits);
}

bool WorldSceneInfo_Copy::TryEnterTargetScene(WorldAvatar* pAvatar, int16 nInstanceID, int32& nFailReason)
{
	if(WorldSceneInfo::TryEnterTargetScene( pAvatar, nInstanceID, nFailReason))
		return true;

	if(IsRootCopy())
	{
		// root copy must be rebuild
	}
	else
	{
		if(!CanRebuildCopy())
			return false;

	}

	return TryRebuildSceneCopy(pAvatar);
}

bool WorldSceneInfo_Copy::TryRebuildSceneCopy(WorldAvatar* pAvatar)
{
	int64 nSceneProcessBits = 0;

	SceneProcessCont* pCont = (SceneProcessCont*)pAvatar->GetCommonDataCont( eCommonData_SceneProcess );
	if(!pCont)
	{
		SceneProcess* pProcess = pCont->GetProcess( m_nSceneSID );
		if( pProcess )
			nSceneProcessBits = pProcess->GetProcessBits();
	}

	WorldScene* pScene = HandleCreateCopy( pAvatar, nSceneProcessBits);
	if(!pScene)
		return false;

	return true;
}

WorldScene* WorldSceneInfo_Copy::HandleCreateCopy(WorldAvatar* pAvatar, int64 nSceneProcessBits, int64 nSceneCustomData)
{
	int32 nNodeSrvID = ServerManager::Instance().m_LocalNodeGrp.GetLowSceneLoadNode();

	SceneCreateArg arg;
	FillSceneCreateArg( arg );

	arg.m_SceneID = AllocSceneID();
	arg.m_NodeSrvID = nNodeSrvID;
	arg.m_nSceneProcessBits = nSceneProcessBits;
	arg.m_nSceneCustomData = nSceneCustomData;

	WorldScene* pScene = (WorldScene*)SceneMgr.CreateWorldScene(arg);
	if(!pScene)
		return NULL;

	if( pAvatar )
	{
		// simple copy own to one avatar, multi player copy own to team
		pScene->UpdateCopyOwner(pAvatar);
		pScene->AddWaitingAvatar(pAvatar);
	}

	return pScene;
}

CreateSceneRst WorldSceneInfo_Copy::HandleCreateCopyRequest(WorldAvatar* pAvatar, int64 nSceneProcessBits)
{
	WorldScene* pScene = HandleCreateCopy( pAvatar, nSceneProcessBits);
	if(!pScene)
		return eCreateSceneError_CreateWorldScene;

	pScene->SetSceneState(Scene::eSceneState_Creating);
	return eCreateScene_Succeed;
}

bool WorldSceneInfo_TeamCopy::HandleEnterSceneExist(int32 nSceneID, WorldAvatar* pAvatar)
{
	return true;
}

WorldScene* WorldSceneInfo_TeamCopy::GetTeamExistScene(WorldTeam* pTeam)
{
	return NULL;
}

void WorldSceneInfo_TeamCopy::OnEnterScene(WorldAvatar* pAvatar, WorldScene* pScene)
{

}

