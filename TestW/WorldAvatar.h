#pragma once

#include <string>
#include "BaseType.h"
#include "WorldAvatarGroup.h"

enum DataLoadingFlag
{
	eDLF_Avatar		= 1 << 0,	// avatar self data
	eDLF_CommonData	= 1 << 1,	// 
	eDLF_Relation	= 1 << 2,	// friend data
	eDLF_TalentData	= 1 << 3,
	eDLF_GateAffirm	= 1 << 4,	// gate confirm
	eDLF_BillingAffirm = 1 << 5,	// billing confirm
	
	eDLF_Team		= 1 << 6,
	eDLF_Guid		= 1 << 7,	

	eDLF_All		= eDLF_Avatar | eDLF_CommonData | eDLF_Relation | eDLF_TalentData | eDLF_GateAffirm | eDLF_BillingAffirm | eDLF_Team | eDLF_Guid,
};

enum CurDataStage
{
	eCurDataOnDBA = 0,
	eCurDataOnWorld,
	eCurDataOnNode,
};

enum WorldAvatarFlag
{
	eWAF_Online	= 1 << 0,	// online & enter game
};

class CreateWorldAvatarArg
{
public:
	int32 nGateSrvID;
	int32 nGateChannelID;

	std::string strUserAccount;

	CreateWorldAvatarArg()
	{
		nGateSrvID = SrvID_NULL;
		nGateChannelID = -1;
	}
};

class WorldServer;
class WorldState;
class PacketBase;
class Scene;
class WorldAvatar;

WorldAvatar* GetWorldAvatar(int32 nAvatarID);
WorldAvatar* GetWorldAvatar( PacketBase* pPkt);
WorldAvatar* GetWorldAvatarByDID( int64 nAvatarDID);
WorldAvatar* GetWorldAvatarByNameID( const char* name);

class WorldAvatar : public WorldAvatarGroup
{
	DECLARE_FACTORY_ARG0( WorldAvatar, -1, new PoolAllocator)
public:
	WorldAvatar();
	virtual ~WorldAvatar();

	bool Init(CreateWorldAvatarArg& args);

	void CreateComponent();
	void ReleaseComponent();
	void ClearParamPool();

	bool Tick(int32 nDeltaTime);
	void TickFCM(int32 nDeltaTime);

	void BillingHeartBeat(int32 nDeltaTime);
	void NodeAffirm(int32 nAffirm = 1);

	int64 GenUID();
	
	virtual Scene* GetScene();

	void SetTargetScenePoint(f32 x, f32 y, f32 z) { m_tartget_scene_point.Set( x, y, z);}
	void SetTargetScenePoint(const Vector3& pos) { m_target_scene_point = pos;}

	void SetTargetSceneDir(f32 x, f32 y, f32 z) { m_tartget_scene_dir.Set( x, y, z);}
	void SetTargetSceneDir(const Vector3& dir) { m_target_scene_dir = dir;}

	void SetPendingCreateScene(int32 nSceneID) { m_PendingCreateSceneID = nSceneID; }
	int32 GetPendingCreateScene() { return m_PendingCreateSceneID; }

	void SetWorld(WorldServer* pWorld) { m_pWorld = pWorld; }

	void SendPacket(PacketBase* pkt);
	void BroadCastPacket2AllAvatar(PacketBase* pPkt);

	virtual void Send2DBA( PacketBase* pPkt);
	virtual void Send2Node( PacketBase* pPkt);	
	virtual void Send2Login( PacketBase* pPkt);	
	virtual void Send2Gate( PacketBase* pPkt);

	void Send2CurNode(PacketBase& pkt);

	int32 GetAvatarNameID();
	int32 GetUserID();

	virtual void SetParamPool(ParamPool* p);
	WorldAvatarCommonData*	GetCommonDataManager() const;
	//WorldTalenTreeData*		GetTalentTreeData() { return m_pTalentTreeData; }

	ParamPool*	GetRoleSet(int32 nIdx) { return Account.GetRoleSet(nIdx);}
	bool CheckInStage(WorldStateID stageID) { return m_nCurStageID == stageID; }

	bool IsGaming() { return m_nCurStageID == eWS_Gaming;}
	bool IsExitingGame() { return m_nCurStageID == eWS_ExitGame; }

	void SetCurState( WorldStateID newStateID );

	bool PushAllData2CurNode();

	void HandleCreateSceneResult( int32 nResult, WorldScene* pScene = NULL);

	void OnEnterGameFailed();
	void OnAvatarEnterGame();
	bool CheckPlayerEnterScene(WorldScene* pScene);

	void SyncAvatarData2DB(bool bExitGame, int32 nSyncFlag);

	void SetDataLoadingFlags(int32 bit) { m_nDataLoadingFlags |= bit; }
	void CheckDataLoadingFlags(int32 bit) { return (m_nDataLoadingFlags & bit) == bit; }

	void			TickChatInternal(int32 nDeltaTime);
	virtual void	SetChatInterval(int32 nChannel, int32 interval);
	virtual int32	GetChatInterval(int32 nChannel);

	void			TickComponent(int32 nDeltaTime);

	bool			RequestBillingLogin();
	void			NotifyBillingLogout();

	bool			IsInBlackList( WorldAvatar* pAvatar);

	void			HasGateAvatar(bool v) { m_bHasGateAvatar = v; }
	bool			HasGateAvatar() const { return m_bHasGateAvatar; }

	void			SetWorldArenaState(int8 state) { m_worldArenaState = state; }
	int8			GetWorldArenaState() const { return m_worldArenaState; }

	bool			CheckAvatarFlag(int32 nFlag) { return (nFlag & m_WorldAvatarFlag) != 0; }
	void			SetAvatarFlag(int32 nFlag) { return m_WorldAvatarFlag |= nFlag; }
	void			CancelAvatarFlag(int32 nFlag) { return m_WorldAvatarFlag &= (~nFlag); }

	WorldAccount	m_account;
protected:
	int32			m_charInterval[CC_MAX];

	Vector3			m_target_scene_point;
	Vector3			m_target_scene_dir;


	int32			m_nCurStageID;
	WorldState*		m_pCurStage;
	WorldServer*	m_pWorld;

	CurDataStage	m_curDataOnSide;

	int32			m_nTargetNodeID;
	int32			m_nTargetSceneID;

	int32			m_nDataLoadingFlags;
	int32			m_WorldAvatarFlag;

	int8			m_worldArenaState;

	bool			m_bIsDestroy;
	bool			m_bStageChanging;
	bool			m_bTargetNodeAvatarCreated;
	bool			m_bAvatarDataDirty;
	bool			m_bHasGateAvatar;
	bool			m_bLastAvatarInfoValid;


	WorldTalenTreeData*			m_pTalentTreeCom;
	WorldRelationComponent*		m_pRelationCom;
	WorldAbilityComponent*		m_pWorldAbilityCom;
	WorldManufactureComponent*	m_pManufactureCom;
	WorldArenaRankComponent*	m_pArenaRankComponent;
};