#pragma once

#include "BaseType.h"
#include "Factory.h"
#include "PoolAllocator.h"
#include <string>
#include "Singleton.h"
#include <hash_map>
#include "Time.h"

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

class GateAccountStateBase;
class PacketBase;

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

	const Time& GetExpireTime() const { return m_expireTime; }
	void SetExpireTime(uint32 nMilliSeconds);

	GateAvatar&	GetAvatar() { return m_avatar; }

	GateAccountStateBase* GetState() const { return m_pState; }
	int32 GetCltSocketID() const { return m_nClntSockID; }

	void Send2Clt(class PacketBase& pkt);

	std::string strUserCN;
protected:
	int32	m_nClntSockID;
	bool	m_bClntConnect;
	bool	m_bDestroy;
	bool	m_bNeedNoticeWorldWhenDestroy;
	int32	m_nUserIP;
	int32	m_nCityChatChannelID;

	Time	m_expireTime;

	GateAccountStateBase*	m_pState;
	GateAvatar				m_avatar;
};

typedef stdext::hash_map<int32, GateAccount*> GateAccountMap;

class GateAccountManager : public Singleton<GateAccountManager>
{
public:
	GateAccountManager();
	~GateAccountManager();

	class GateCltNetChannel* GetCltChannelByAvatarID(int32 nAvatarID);
	int32 GetCltChannelID(int32 nAvatarID);
	
	GateAccount* CreateAccount(int32 nCltSocketID);
	void RemoveAccount(int32 nCltSocketID);

	GateAccount* GetAccount(int32 nSocketID);
	GateAccount* GetAccountByAvatarID(int32 nAvatarID);

	size_t GetAccountCount() const { return m_mapSocketID2Account.size(); }
	size_t GetAvatarCount() const { return m_mapAvatarID2Account.size(); }

	void Send2AllAvatar(PacketBase& pkt);
	void Tick(int32 nFrameTime);
private:
	volatile long m_RWLock_mapAvatarID2ChannelID;
	
	GateAccountMap					m_mapSocketID2Account;
	GateAccountMap					m_mapAvatarID2Account;

	stdext::hash_map< int32, int16> m_mapAvatarID2ChannelID;	// avatarid -> channelid

};

