#pragma once

#include <string>
#include "WorldAvatarGroup.h"
#include "MyVector.h"
#include "WorldAccount.h"
#include "WorldScene.h"
#include "WorldStateManager.h"

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
	eWorldAvatar_Online	= 1 << 0,	// online & enter game
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
class ParamPool;

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

	void SetTargetScenePoint(f32 x, f32 y, f32 z) { m_target_scene_point.Set( x, y, z);}
	void SetTargetScenePoint(const Vector3& pos) { m_target_scene_point = pos;}
	const Vector3& GetTargetScenePoint() { return m_target_scene_point;}

	void SetTargetSceneDir(f32 x, f32 y, f32 z) { m_target_scene_dir.Set( x, y, z);}
	void SetTargetSceneDir(const Vector3& dir) { m_target_scene_dir = dir;}
	const Vector3& GetTargetSceneDir() { return m_target_scene_point;}

	void SetPendingCreateScene(int32 nSceneID) { m_PendingCreateSceneID = nSceneID; }
	int32 GetPendingCreateScene() { return m_PendingCreateSceneID; }

	void SetWorld(WorldServer* pWorld) { m_pWorld = pWorld; }

	void SendPacket(PacketBase* pkt);
	void BroadCastPacket2AllAvatar(PacketBase* pPkt);

	virtual void Send2DBA( PacketBase* pPkt);
	virtual void Send2Node( PacketBase* pPkt);	
	virtual void Send2Login( PacketBase* pPkt);	
	virtual void Send2Gate( PacketBase* pPkt, bool bGateProc = false);

	void Send2CurNode(PacketBase& pkt);

	int32 GetAvatarNameID();
	int32 GetUserID();

	virtual void SetParamPool(ParamPool* p);
	class WorldAvatarCommonData*	GetCommonDataManager() const;
	//WorldTalenTreeData*		GetTalentTreeData() { return m_pTalentTreeData; }

	void NoticeBillingLogout(bool bExitGame);

	ParamPool*	GetRoleSet(int32 nIdx) { return Account.GetRoleSet(nIdx);}
	WorldAccount Account;

	bool CheckInState(WorldStateID stageID) { return m_nCurStageID == stageID; }

	bool IsGaming() { return m_nCurStageID == eWS_Gaming;}
	bool IsExitingGame() { return m_nCurStageID == eWS_ExitGame; }

	void SetCurState( WorldStateID newStateID );

	bool PushAllData2CurNode();

	void HandleCreateSceneResult( int32 nResult, WorldScene* pScene = NULL);

	void OnEnterGameFailed();
	void OnAvatarEnterGame();
	void OnAvatarLeaveGame();
	bool CheckPlayerEnterScene(WorldScene* pScene);

	void SyncAvatarData2DB(bool bExitGame, int32 nSyncFlag);

	void SetDataLoadingFlags(int32 bit) { m_nDataLoadingFlags |= bit; }
	bool CheckDataLoadingFlags(int32 bit) { return (m_nDataLoadingFlags & bit) == bit; }

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
	void			SetAvatarFlag(int32 nFlag) { m_WorldAvatarFlag |= nFlag; }
	void			CancelAvatarFlag(int32 nFlag) { m_WorldAvatarFlag &= (~nFlag); }
	void			RelaseComponent();

	virtual Team*	GetTeam();

	int64			GenGalaxyUID();

	WorldAccount	m_account;
	int32			m_nChangeSceneState;	// 0->first login;1->normal playing
	CurDataStage	m_curDataOnSide;

	int32			m_nTargetSceneID;
	int32			m_nDataLoadingFlags;
	int32			m_nCommonDataMaskFinish;

	bool			m_bTargetNodeAvatarCreated;
	bool			m_bIsDestroy;
	int32			m_nTargetNodeID;
	bool			m_bAvatarDataDirty;
	int32			m_nCurStageID;
protected:
	int32			m_charInterval[CC_MAX];

	Vector3			m_target_scene_point;
	Vector3			m_target_scene_dir;

	WorldState*		m_pCurStage;



	int32			m_WorldAvatarFlag;

	int8			m_worldArenaState;

	bool			m_bStageChanging;
	bool			m_bHasGateAvatar;
	bool			m_bLastAvatarInfoValid;

	int32			m_PendingCreateSceneID;
	WorldServer*	m_pWorld;
	uint8			m_nLastAvatarLevel;

// 	WorldTalenTreeData*			m_pTalentTreeCom;
// 	WorldRelationComponent*		m_pRelationCom;
// 	WorldAbilityComponent*		m_pWorldAbilityCom;
// 	WorldManufactureComponent*	m_pManufactureCom;
// 	WorldArenaRankComponent*	m_pArenaRankComponent;
};