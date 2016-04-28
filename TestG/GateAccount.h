#pragma once

enum GateAccountState
{
	eGAS_CltConnected,
	eGAS_CltReady,
	eGAS_WaitingUserData,
	eGAS_Ready2EnterGame,

	eGAS_CreatingAvatar,
	eGAS_CreatingAvatarFinish,
	
	eGAS_DeletingAvatar,

	eGAS_RequestEnterGame,

	eGAS_Gaming,
	eGAS_Destroy,

	eGAS_Count,
};

struct GateAvatar
{
	GateAvatar()
		: m_nAvatarID(0)
		, m_nSceneID(SCENE_ID_NULL)
	{

	}

	int32 m_nAvatarID;
	int32 m_nSceneID;
	int32 m_nPeerSockID;

	std::string m_strAvatarTitle;
};

class GateAccount
{
	friend class GateAccountManager;
	friend class GateAccountStateBase;

	DECLARE_FACTORY_ARG1( GateAccount, -1, new PoolAllocator, int16);
public:
	explicit GateAccount(int16 nCltSocketID);
	~GateAccount(void);

	static GateAccount* New(int16 nCltSocketID);
	static void Delete(GateAccount* ptr);

	void ChangeState(GateAccountState state);
};

