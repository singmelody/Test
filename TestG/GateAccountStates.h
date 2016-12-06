#pragma once

#include "GateAccount.h"
#include "Singleton.h"
#include "PacketImpl.h"

class GateAccountStateBase
{
public:

	static GateAccountStateBase* GetState(GateAccountState state);
	static void Init();

	virtual const char* GetStateName() const = 0;
	virtual void OnEnter(GateAccount& account){}
	virtual void OnDisconnect(GateAccount& account) { account.ChangeState(eGateAccountState_Destroy); }
	virtual void OnLeave(GateAccount& account){}

	// from clt
	virtual void HandleCltPacket(GateAccount& account, class PacketCltReady& pkt) { DefaultHandlePacket( account, pkt, "Client");}
	virtual void HandleCltPacket(GateAccount& account, class PacketCreateAvatar& pkt) { DefaultHandlePacket( account, pkt, "Client");}
	virtual void HandleCltPacket(GateAccount& account, class PacketDelCharacter& pkt) { DefaultHandlePacket( account, pkt, "Client");}
	virtual void HandleCltPacket(GateAccount& account, class PacketCltSelectAvatar& pkt) { DefaultHandlePacket( account, pkt, "Client");}
	virtual void HandleCltPacket(GateAccount& account, class PacketLogout2Login& pkt) { DefaultHandlePacket( account, pkt, "Client");}
	virtual void HandleCltPacket(GateAccount& account, class PacketConnectGateSrv& pkt) { DefaultHandlePacket( account, pkt, "Client"); }

	// from world
	virtual void HandleWorldPacket(GateAccount& account, class PacketNewConnectioRet& pkt) { DefaultHandlePacket( account, pkt, "World"); }

	void Tick(GateAccount& account, int32 nFrameTime, Time& curTime);
protected:
	virtual void OnTick(GateAccount& account, int32 nFrameTime, Time& tCurr){}
	virtual bool HasExpiration() { return true; }
	virtual void OnExpire(GateAccount& account) { account.ChangeState(eGateAccountState_Destroy); }
	void DestroyAccount(GateAccount& account) { account.m_bDestroy = true; }

	explicit GateAccountStateBase(GateAccountState state);
	virtual ~GateAccountStateBase(){}

	void DefaultHandlePacket( GateAccount& account, class PacketBase& pkt, const char* pSzFrom);

	class GateServer& GateSrv;
	class GateAccountManager& AccountMgr;

	const GateAccountState	m_state;
	static GateAccountStateBase*	s_states[eGateAccountState_Count];
};

class GateAccountState_ClntConnected : public GateAccountStateBase, public Singleton<GateAccountState_ClntConnected>
{
public:
	GateAccountState_ClntConnected() : GateAccountStateBase(eGateAccountState_CltConnected){}
	virtual const char* GetStateName() const{ return "ClntConnected"; }
};
class GateAccountState_CltReady : public GateAccountStateBase, public Singleton<GateAccountState_CltReady>
{
public:
	GateAccountState_CltReady() : GateAccountStateBase(eGateAccountState_CltReady){}
	virtual const char* GetStateName() const{ return "CltReady"; }

};											

class GateAccountState_WaitingUserData : public GateAccountStateBase, public Singleton<GateAccountState_WaitingUserData>
{
public:
	GateAccountState_WaitingUserData() : GateAccountStateBase(eGateAccountState_WaitingUserData){}
	virtual const char* GetStateName() const{ return "WaitingUserData"; }

};											
class GateAccountState_Ready2EnterGame : public GateAccountStateBase, public Singleton<GateAccountState_Ready2EnterGame>
{
public:
	GateAccountState_Ready2EnterGame() : GateAccountStateBase(eGateAccountState_Ready2EnterGame){}
	virtual const char* GetStateName() const{ return "Ready2EnterGame"; }

};										  

class GateAccountState_CreatingAvatar : public GateAccountStateBase, public Singleton<GateAccountState_CreatingAvatar>
{
public:
	GateAccountState_CreatingAvatar() : GateAccountStateBase(eGateAccountState_CreatingAvatar){}

protected:
	virtual const char* GetStateName() const { return "CreatingAvatar"; }
	virtual void OnEnter(GateAccount& account)
	{
		account.SetExpireTime(60*1000);
	}

	virtual void HandleWorldPacket(GateAccount& account, class PacketCreateAvatarRes& pkt)
	{

	}
};			

class GateAccountState_CreatingAvatarFinish : public GateAccountStateBase, public Singleton<GateAccountState_CreatingAvatarFinish>
{
public:
	GateAccountState_CreatingAvatarFinish() : GateAccountStateBase(eGateAccountState_CreatingAvatarFinish){}
	virtual const char* GetStateName() const{ return "CreatingAvatarFinish"; }
};										   

class GateAccountState_DeletingAvatar : public GateAccountStateBase, public Singleton<GateAccountState_DeletingAvatar>
{
public:
	GateAccountState_DeletingAvatar() : GateAccountStateBase(eGateAccountState_DeletingAvatar){}

	virtual const char* GetStateName() const{ return "DeletingAvatar"; }

};										   

class GateAccountState_RequestEnterGame : public GateAccountStateBase, public Singleton<GateAccountState_RequestEnterGame>
{
public:
	GateAccountState_RequestEnterGame() : GateAccountStateBase(eGateAccountState_RequestEnterGame){}

	virtual const char* GetStateName() const{ return "RequestEnterGame"; }
};										   
class GateAccountState_Gaming : public GateAccountStateBase, public Singleton<GateAccountState_Gaming>
{
public:
	GateAccountState_Gaming() : GateAccountStateBase(eGateAccountState_Gaming){}

	virtual const char* GetStateName() const{ return "Gaming"; }
};											
class GateAccountState_Destroy : public GateAccountStateBase, public Singleton<GateAccountState_Destroy>
{
public:
	GateAccountState_Destroy() : GateAccountStateBase(eGateAccountState_Destroy){}

	virtual const char* GetStateName() const{ return "Destroy"; }
};

class GameAccountState_CreateAvatar : public GateAccountStateBase, public Singleton<GameAccountState_CreateAvatar>
{
public:
	GameAccountState_CreateAvatar(void) : GateAccountStateBase(eGateAccountState_CreatingAvatar){}
	virtual ~GameAccountState_CreateAvatar(void){}

protected:
	virtual const char* GetStateName() const { return "CreatingAvatar"; }
	virtual void OnEnter(GateAccount& account);
};

template <typename PacketType>
void HandleCltPacket(PacketType& pkt, bool bCheckSrvReady = true)
{
	const int32 nSocketID = pkt.GetSocketID();
	GateAccount* pAccount = GateAccountManager::Instance().GetAccount(nSocketID);
	if (!pAccount)
	{
		MyLog::error("HandleCltPacket [%s] failed to GetAvatar! SocketID=[%d]", pkt.GetClassName(), nSocketID);
		GateServer::Instance().CloseCltConnection(nSocketID);
		return;
	}

	if(bCheckSrvReady && !GateServer::Instance().IsServerReady())
	{
		MyLog::message("HandleCltPacket [%s] failed for srv is not ready");
		pAccount->ChangeState(eGateAccountState_Destroy);
		return;
	}

	GateAccountStateBase* pState = pAccount->GetState();
	assert( pState );
	pState->HandleCltPacket( *pAccount, pkt);

}

template <class PacketType>
bool HandleWorldPacket(PacketType& pkt)
{
	const int32 nAvatarID = pkt.GetAvatarID();
	GateAccount* pAccount = GateAccountManager::Instance().GetAccountByAvatarID(nAvatarID);
	if(!pAccount)
	{
		MyLog::error("HandleWorldPacket [%s] failed to GetAvatar ! AvatarID=[%d]", pkt.GetClassName(), nAvatarID);
		return false;
	}

	GateAccountStateBase* pState = pAccount->GetState();
	assert(pState != NULL);
	pState->HandleWorldPacket(*pAccount, pkt);
}
