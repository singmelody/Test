#pragma once

#include "BaseType.h"
#include "Factory.h"
#include "PoolAllocator.h"
#include <string>

enum GateAccountState
{
	eGateAccountState_CltConnected,
	eGateAccountState_CltReady,
	eGateAccountState_WaitingUserData,
	eGateAccountState_Ready2EnterGame,

	eGateAccountState_CreatingAvatar,
	eGateAccountState_CreatingAvatarFinish,
	eGateAccountState_DeletingAvatar,
	eGateAccountState_RequestEnterGame,

	eGateAccountState_Gaming,
	eGateAccountState_Destroy,

	eGateAccountState_Count,
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

