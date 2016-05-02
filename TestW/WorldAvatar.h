#pragma once

#include <string>

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
	INT32 GetUserID();

	virtual void SetParamPool(ParamPool* p);
	WorldAvatarCommonData*	GetCommonDataManager() const;
	WorldTalenTreeData*		GetTalentTreeData() { return m_pTalentTreeData; }

	ParamPool*	GetRoleSet(int32 nIdx) { return Account.GetRoleSet(nIdx);}
protected:
	Vector3	m_target_scene_point;
	Vector3 m_target_scene_dir;

	WorldAccount	m_account;

	bool	nTargetNodeAvatarCreated;
	int32	nTargetNodeID;
	int32	nTargetSceneID;
};