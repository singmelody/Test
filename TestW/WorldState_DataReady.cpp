#include "StdAfx.h"
#include "WorldState_DataReady.h"
#include "PacketImpl.h"
#include "AvatarOnLineManager.h"
#include "SceneManager.h"
#include "MyLog.h"
#include "WorldSceneInfo.h"
#include "WorldAvatar.h"
#include "Time.h"
#include "ParamPool.h"
#include "SceneProcessCont.h"
#include "SceneProcess.h"

WorldState_DataReady::WorldState_DataReady(void)
{
}


WorldState_DataReady::~WorldState_DataReady(void)
{
}

void WorldState_DataReady::OnEnterState(WorldAvatar* pAvatar)
{
	WorldState::OnEnterState(pAvatar);

	pAvatar->m_bAvatarDataDirty = false;	// enter this stage, think data the same with db

	AvatarOnLineManager::Instance().AddAvatar(pAvatar);

	ParamPool* pPool = pAvatar->GetParamPool();
	assert(pPool);
}

void WorldState_DataReady::OnLeaveState(WorldAvatar* pAvatar)
{
	WorldState::OnLeaveState(pAvatar);

	uint32 nLandTime = Time::CurrentTime().Second();
	PARAM_SET_VALUE( pAvatar->GetParamPool(), lastlandtime, nLandTime, true);

}

void WorldState_DataReady::Tick(int32 nFrameTime)
{
	TickList& list = m_listAvatars;

	TickNode* pNode = list.GetNext(NULL);
	while ( pNode != NULL)
	{
		WorldAvatar* pAvatar = (WorldAvatar*)( pNode->Get() );
		if(!pAvatar)
		{
			pNode = list.Remove(pNode);
			continue;
		}

		pNode = list.GetNext(pNode);
		pAvatar->SubPendingTime(nFrameTime);
		if( pAvatar->GetPendingTime() <= 0 )
		{
			PacketCltSelectAvatarFailed tmpPkt;
			tmpPkt.nReason = PacketCltSelectAvatarFailed::eFR_EnterSceneFailed;
			pAvatar->Send2Gate( &tmpPkt, true);
			DestroyAvatar(pAvatar);
			continue;
		}

		// success & next
		if(RequestEnterScene( pAvatar, pAvatar->m_nTargetSceneID))
			continue;

		// success & next
		if(TryEnterNextScene(pAvatar))
			continue;

		MyLog::error("Avatar Req Enter Game Failed AvatarID[%d] Account[%s]", pAvatar->GetAvatarID(), pAvatar->GetAccountName());

		PacketCltSelectAvatarFailed tmpPkt;
		tmpPkt.nReason = PacketCltSelectAvatarFailed::eFR_EnterSceneFailed;
		pAvatar->Send2Gate( &tmpPkt, true);

		DestroyAvatar(pAvatar);
	}
}

bool WorldState_DataReady::RequestEnterScene(WorldAvatar* pAvatar, int32 nTargetSceneID)
{
	assert( pAvatar );
	const int16 nSceneSID = SceneInfo::GetSceneSID(nTargetSceneID);

	WorldSceneInfo* pInfo = (WorldSceneInfo*)(SceneManager::GetSceneInfo(nSceneSID));
	if(!pInfo)
		return false;

	if(pInfo->DisableReEnter())
		return false;

	if(pInfo->IsCopy())
	{
		// check copy has any reset bit
		// if it's has been reset, back to parent scene
		SceneProcessCont* pCont = (SceneProcessCont*)pAvatar->GetCommonDataCont( eCommonData_SceneProcess );
		if(pCont != NULL)
		{
			SceneProcess* pProcess = pCont->GetProcess( nSceneSID );
			if( pProcess != NULL )
			{
				if(pProcess->HasResetFlag() )
				{
					if( pInfo->IsRootCopy() )
					{
						const EnterPointInfo* pEnter = pInfo->GetEnterPointInfo( nSceneSID, eEnterType_Default);
						if(pEnter)
						{
							pAvatar->SetTargetScenePoint( pEnter->GetPos() );
							pAvatar->SetTargetSceneDir( pEnter->vDir );
						}
					}
					else
						return false;
				}
			}
		}
	}

	int32 failReason = 0;

	return pInfo->TryEnterTargetScene( pAvatar, SceneInfo::GetSceneInstanceID( nTargetSceneID), failReason);
}

// scene instance not able to enter , etc war copy close, change target scene 2 current scene's nextscene and enter.
bool WorldState_DataReady::TryEnterNextScene(WorldAvatar* pAvatar)
{
	assert( pAvatar );

	int32 nTargetSceneID = pAvatar->m_nTargetSceneID;
	int16 nSceneSID = SceneInfo::GetSceneSID(nTargetSceneID);

	const SceneInfo* pCurSD = SceneManager::GetSceneInfo( nSceneSID );
	if(!pCurSD)
		nSceneSID = SCENE_ID_DEFAULT;
	else
	{
		if( pCurSD->IsRootCopy() )
		{
			MyLog::error("WorldState_DataReady::TryEnterNextScene error <1>, avatarid=[%d]", pAvatar->GetAvatarID());
			return false;
		}

		const SceneInfo* pNextInfo = SceneManager::GetSceneInfo( pCurSD->m_nNextSceneSID);
		if(pNextInfo != NULL)
		{
			if( pNextInfo->IsMainTrunk() )
				nSceneSID = pCurSD->m_nNextSceneSID;
			else
				nSceneSID = SCENE_ID_DEFAULT;
		}
		else
		{
			if( nSceneSID == SCENE_ID_DEFAULT )
			{
				MyLog::error("WorldState_DataReady::TryEnterNextScene error <2>, avatarid=[%d]", pAvatar->GetAvatarID());
				return false;
			}

			nSceneSID = SCENE_ID_DEFAULT;
		}
	}

	const SceneInfo* pNextSD = SceneManager::GetSceneInfo(nSceneSID);
	if( pNextSD == NULL )
	{
		MyLog::error("WorldState_DataReady::TryEnterNextScene error <3>, avatarid=[%d]", pAvatar->GetAvatarID());
		return false;
	}

	uint16 fromSceneSID = ( pCurSD == NULL ) ? 0 : pCurSD->m_SceneSID;

	const EnterPointInfo* pEnter = pNextSD->GetEnterPointInfo( fromSceneSID );
	if(pEnter == NULL)
	{
		MyLog::error("WorldState_DataReady::TryEnterNextScene error <4>, avatarid=[%d]", pAvatar->GetAvatarID());
		return false;
	}

	nTargetSceneID = SceneInfo::GenSceneID( nSceneSID );

	pAvatar->m_nTargetSceneID = nTargetSceneID;
	pAvatar->SetTargetScenePoint(pEnter->GetRandPos());
	pAvatar->SetTargetSceneDir(pEnter->vDir);

	return true;
}

void WorldState_DataReady::DestroyAvatar(WorldAvatar* pAvatar)
{
	pAvatar->m_bIsDestroy = true;
	pAvatar->SetCurState(eWS_ExitGame);
}