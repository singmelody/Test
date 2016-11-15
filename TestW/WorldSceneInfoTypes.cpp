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

//----------------------------------------------
// Trunk
//----------------------------------------------
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

//----------------------------------------------
// MainTrunk
//----------------------------------------------
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
	UpdateParallelBits();
}

void WorldSceneInfo_MainTrunk::OnSceneDestroy(Scene* pScene)
{
	WorldSceneInfo_Trunk::OnSceneDestroy(pScene);
	--_parallelCount;
	UpdateParallelBits();
}

void WorldSceneInfo_MainTrunk::OnEnterScene(WorldAvatar* pAvatar, WorldScene* pScene)
{
	WorldSceneInfo_Trunk::OnEnterScene( pAvatar, pScene);

	if(!pAvatar || !pScene)
		return;

	WorldTeam* pTeam = pAvatar->GetWorldTeam();
	if(!pTeam)
		return;

	if( pTeam->GetLeaderDID() == pAvatar->GetAvatarDID() )
		_teamInstance[pTeam->GetTeamID()] = SceneInfo::GetSceneInstanceID(pScene->GetSceneID());
}

void WorldSceneInfo_MainTrunk::OnLeaveScene(WorldAvatar* pAvatar, WorldScene* pScene)
{
	WorldSceneInfo_Trunk::OnLeaveScene(pAvatar, pScene);

	if(!pAvatar || !pScene)
		return;

	WorldTeam* pTeam = pAvatar->GetWorldTeam();
	if(!pTeam)
		return;

	if( pTeam->GetLeaderDID() == pAvatar->GetAvatarDID() )
		_teamInstance.erase(pTeam->GetTeamID());
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

	// try enter the unbusy line first
	Scene* pScene = m_Instances.GetFrontScene();
	if(pScene)
	{
		if(TryEnterScene( pAvatar, (WorldScene*)pScene, nFailReaon))
			return true;
	}

	// retry to enter the minload instance
	pScene = m_Instances.GetMinLoadScene();
	if(pScene)
	{
		if(TryEnterScene( pAvatar, (WorldScene*)pScene, nFailReaon))
			return true;
	}

	bool bHasCreatingScene = false;


	// retry to enter other line, because the minload line can't enter for some reason
	for (SceneInstanceMgr::iterator itr = m_Instances.begin(); itr != m_Instances.end(); ++itr)
	{
		WorldScene* pScene = (WorldScene*)itr->second;

		if(pScene->IsCreating())
		{
			bHasCreatingScene = true;
			continue;
		}

		if(TryEnterScene( pAvatar, pScene, nFailReaon))
			return true;
	}

	if(!bHasCreatingScene)
	{
		nFailReaon = eChangeSceneError_SceneIsFull;
		return false;
	}

	int32 nContextID = pAvatar->GetAvatarID();
	SceneRequestList& list = RequestList;

	if(!list.push(nContextID))
		return false;

	pAvatar->SetCurState(eWS_WaitScene);

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

WorldSceneInfo_Copy::WorldSceneInfo_Copy()
{

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

bool WorldSceneInfo_Copy::HandleEnterSceneExist(int32 nSceneID, WorldAvatar* pAvatar)
{
	int16 nInstanceID = SceneInfo::GetSceneInstanceID(nSceneID);

	if( nInstanceID != SCENE_ID_NULL )
	{
		WorldScene* pScene = GetWorldScene(nSceneID);
		if( pScene != NULL )
		{
			if( pScene->CheckEnterScene( pAvatar ))
			{
				pAvatar->HandleCreateSceneResult( eCreateScene_Succeed, pScene);
				return true;
			}
		}
	}

	return false;
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


void WorldSceneInfo_Copy::OnSceneCreateSucceed(WorldScene* pScene)
{
	WorldSceneInfo::OnSceneCreateSucceed(pScene);
	pScene->NotifyWaitingAvatars( eCreateScene_Succeed );
}

void WorldSceneInfo_Copy::OnSceneCreateFailed(WorldScene* pScene, int32 nErrorID)
{
	WorldSceneInfo::OnSceneCreateFailed(pScene, nErrorID);
	pScene->NotifyWaitingAvatars( nErrorID );
}

WorldSceneInfo_TeamCopy::WorldSceneInfo_TeamCopy()
	: TeamMgr(WorldTeamManager::Instance())
{
	m_strDftSceneClass = "WorldScene_TeamCopy";
}

CreateSceneRst WorldSceneInfo_TeamCopy::HandleCreateCopyRequest(WorldAvatar* pAvatar, int64 nSceneProcessBits)
{
	WorldTeam* pTeam = (WorldTeam*)pAvatar->GetTeam();

	if( pTeam && (pTeam->GetMemberCount() > (int32)m_nPlayerMax))
		return eCreateSceneError_TeamCopy;

	return WorldSceneInfo_Copy::HandleCreateCopyRequest( pAvatar, nSceneProcessBits);
}

WorldScene* WorldSceneInfo_TeamCopy::GetTeamExistScene(WorldTeam* pTeam)
{
	if(!pTeam)
		return NULL;

	WorldAvatar* pLeader = pTeam->GetLeader();
	if(pLeader != NULL)
	{
		WorldScene* pScene = (WorldScene*)pLeader->GetScene();
		if( pScene && pScene->SceneSID() == m_nSceneSID )
			return pScene;
		else
		{
			if( pLeader->GetPendingCreateScene() != SCENE_ID_NULL &&
				SceneInfo::GetSceneSID(pLeader->GetPendingCreateScene()) == m_nSceneSID)
			{
				return GetWorldScene(pLeader->GetPendingCreateScene());
			}
		}
	}

	Team::iterator itr = pTeam->begin();
	Team::iterator endItr = pTeam->end();

	for (; itr != endItr; ++itr)
	{
		WorldAvatar* pTeamMate = GetWorldAvatar( itr->second->m_nAvatarID );
		if( pTeamMate )
		{
			WorldScene* pScene = (WorldScene*)pTeamMate->GetScene();
			if( pScene && pScene->SceneSID() == m_nSceneSID )
				return pScene;
			else
			{
				if( pTeamMate->GetPendingCreateScene() != SCENE_ID_NULL &&
					SceneInfo::GetSceneSID(pTeamMate->GetPendingCreateScene()) == m_nSceneSID)
				{
					return GetWorldScene(pTeamMate->GetPendingCreateScene());
				}
			}
		}
	}

	return NULL;
}

void WorldSceneInfo_TeamCopy::OnEnterScene(WorldAvatar* pAvatar, WorldScene* pScene)
{
	WorldSceneInfo_Copy::OnEnterScene( pAvatar, pScene);
	pScene->UpdateCopyOwner( pAvatar );
}

bool WorldSceneInfo_TeamCopy::HandleEnterSceneExist(int32 nSceneID, WorldAvatar* pAvatar)
{
	// 队伍副本优先或个人副本不存在
	if( IsSelTeamCopyFirst() || !GetWorldScene( nSceneID ) )
	{
		WorldTeam* pTeam = pAvatar->GetWorldTeam();
		if( pTeam && ( pTeam->GetMemberCount() > (int32)m_nPlayerMax ))
		{
			MyLog::error("Team or member...");
			return false;
		}

		WorldScene* pTeamScene = GetTeamExistScene( pTeam );
		if(!pTeamScene)
		{
			MyLog::error("TeamScene Error...");
			return false;
		}

		if(!pTeamScene->CheckEnterScene( pAvatar ))
		{
			MyLog::error("WorldSceneInfo_TeamCopy HandleEnterSceneExist Error");
			return false;
		}

		if( pTeamScene->IsCreating() )
			pTeamScene->AddWaitingAvatar( pAvatar );
		else
			pAvatar->HandleCreateSceneResult( eCreateScene_Succeed, pTeamScene);

		return true;
	}

	bool b = WorldSceneInfo_Copy::HandleEnterSceneExist( nSceneID, pAvatar);
	return b;
}


