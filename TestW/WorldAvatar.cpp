#include "StdAfx.h"
#include "WorldAvatar.h"
#include "MyLog.h"
#include "WorldAvatarManager.h"
#include "WorldStateManager.h"
#include "AvatarOnLineManager.h"
#include "ServerManager.h"
#include "WorldState.h"
#include "Time.h"
#include "WorldSceneManager.h"
#include "ArenaDefine.h"
#include <sstream>

WorldAvatar* GetWorldAvatar(int32 nAvatarID)
{
	return (WorldAvatar*)WorldAvatarManager::Instance().GetAvatar(nAvatarID);
}

WorldAvatar* GetWorldAvatar(PacketBase* pPkt)
{
	if(!pPkt)
		return NULL;

	WorldAvatar* pAvatar = GetWorldAvatar(pPkt->GetAvatarID());
	return pAvatar;
}

WorldAvatar* GetWorldAvatarByDID(int64 nAvatarDID)
{
	return AvatarOnLineManager::Instance().GetAvatarByDID(nAvatarDID);
}

WorldAvatar* GetWorldAvatarByNameID(const char* name)
{
	return AvatarOnLineManager::Instance().GetAvatarByName(name);
}


FINISH_FACTORY_ARG0(WorldAvatar);
WorldAvatar::WorldAvatar()
	: m_bLastAvatarInfoValid(false)
	, m_bHasGateAvatar(false)
{
	m_target_scene_point.zero();
	m_target_scene_dir.zero();

	m_pWorld = NULL;

	m_bTargetNodeAvatarCreated = false;
	m_bAvatarDataDirty = false;

	m_curDataOnSide = eCurDataOnDBA;

	InitTickNode(this);

	m_nCurStageID = eWS_Null;
	m_pCurStage = NULL;
	m_bStageChanging = false;

	m_nChangeSceneState = 0;
	m_bIsDestroy = false;

	memset( m_charInterval, 0, sizeof(m_charInterval));
	//memset( &storageData, 0, sizeof(storageData));

	m_PendingCreateSceneID = SCENE_ID_NULL;

	m_worldArenaState = eMP_STATE_NULL;
	m_WorldAvatarFlag = 0;

}


WorldAvatar::~WorldAvatar()
{
	FACTORY_DELOBJ(m_pParamPool);
	RelaseComponent();
}

int64 WorldAvatar::GenUID()
{
	std::stringstream sstr;

	sstr << m_account.GetAccountName() << "." << ServerManager::Instance().m_nZoneID << "." << ServerManager::Instance().m_nGrpID;

	uint32 hashcode = UtilID::CreateFromString( sstr.str().c_str() );
	uint32 second = Time::CurrentTime().Second();

	return uint64(hashcode) << 32 | uint64(second);
}

Scene* WorldAvatar::GetScene()
{
	return WorldSceneManager::Instance().GetScene(m_sceneID);
}

void WorldAvatar::Send2Gate(PacketBase* pPkt, bool bGateProc)
{
	if(!pPkt)
		return;

	pPkt->SetAvatarID(GetAvatarID());
	m_pWorld->Send2Gate( pPkt, m_nGateSrvID, bGateProc);
}

void WorldAvatar::SetCurState( WorldStateID newStateID )
{
	if( m_bStageChanging )
	{
		MyLog::error("SetCurStage error, avatarid[%d], curstage[%d], newStage[%d]", GetAvatarID(), m_nCurStageID, newStateID);
		return;
	}

	if( m_nCurStageID == newStateID )
		return;

	m_bStageChanging = true;

	if( m_pCurStage != NULL)
		m_pCurStage->OnLeaveStage(this);

	m_nCurStageID = newStateID;
	m_pCurStage = WorldStateManager::Instance().GetState(m_nCurStageID);

	if( m_pCurStage != NULL)
		m_pCurStage->OnEnterState(this);

	m_bStageChanging = false;
}

bool WorldAvatar::Tick(int32 nDelaTime)
{
	TickChatInternal(nDelaTime);
	TickComponent(nDelaTime);

	return false;
}

void WorldAvatar::TickChatInternal(int32 nDeltaTime)
{
	for (int32 i = 0; i < CC_MAX; ++i)
	{
		m_charInterval[i] -= nDeltaTime;
		if( m_charInterval[i] < 0)
			m_charInterval[i] = 0;
	}
}

void WorldAvatar::SetChatInterval(int32 nChannel, int32 interval)
{
	if( nChannel < 0 || nChannel >= CC_MAX)
		return;

	m_charInterval[nChannel] = interval;
}

int32 WorldAvatar::GetChatInterval(int32 nChannel)
{
	if( nChannel < 0 || nChannel >= CC_MAX)
		return INT_MAX;

	return m_charInterval[nChannel];
}

void WorldAvatar::RelaseComponent()
{
	//FACTORY_DELOBJ(m_pRelationComponent);
	//FACTORY_DELOBJ(m_pCDComponent);
}

void WorldAvatar::CreateComponent()
{
	
}

void WorldAvatar::ReleaseComponent()
{
	//FACTORY_DELOBJ( m_pRelationCom );

}

bool WorldAvatar::Init(CreateWorldAvatarArg& args)
{
	SetNodeSrvID( SrvID_NULL );
	SetGateSrvID( args.nGateSrvID );
	
	m_nGateSrvID = args.nGateChannelID;

	CreateComponent();

	return true;
}
