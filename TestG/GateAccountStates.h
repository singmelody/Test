#pragma once

#include "GateAccount.h"
#include "Singleton.h"

class GateAccountStateBase
{
public:
	static GateAccountStateBase* GetState(GateAccountState state);
	static void Init();

protected:
	explicit GateAccountStateBase(GateAccountState state);
	virtual ~GateAccountStateBase(){}

	class GateServer& GateSrv;
	class GateAccountManager& AccountMgr;

	const GateAccountState	m_state;
	static GateAccountStateBase*	s_states[eGateAccountState_Count];
};

class GateAccountState_ClntConnected : public GateAccountStateBase, public Singleton<GateAccountState_ClntConnected>
{};
class GateAccountState_CltReady : public GateAccountStateBase, public Singleton<GateAccountState_CltReady>
{};											
class GateAccountState_WaitingUserData : public GateAccountStateBase, public Singleton<GateAccountState_WaitingUserData>
{};											
class GateAccountState_Ready2EnterGame : public GateAccountStateBase, public Singleton<GateAccountState_Ready2EnterGame>
{};										  
class GateAccountState_CreatingAvatar : public GateAccountStateBase, public Singleton<GateAccountState_CreatingAvatar>
{};										  
class GateAccountState_CreatingAvatarFinish : public GateAccountStateBase, public Singleton<GateAccountState_CreatingAvatarFinish>
{};										   
class GateAccountState_DeletingAvatar : public GateAccountStateBase, public Singleton<GateAccountState_DeletingAvatar>
{};										   
class GateAccountState_RequestEnterGame : public GateAccountStateBase, public Singleton<GateAccountState_RequestEnterGame>
{};										   
class GateAccountState_Gaming : public GateAccountStateBase, public Singleton<GateAccountState_Gaming>
{};											
class GateAccountState_Destroy : public GateAccountStateBase, public Singleton<GateAccountState_Destroy>
{};

class GameAccountState_CreateAvatar : public GateAccountStateBase, public Singleton<GameAccountState_CreateAvatar>
{
public:
	GameAccountState_CreateAvatar(void) : GateAccountStateBase(eGateAccountState_CreatingAvatar){}
	virtual ~GameAccountState_CreateAvatar(void){}

protected:
	virtual const char* GetStateName() const { return "CreatingAvatar"; }
	virtual void OnEnter(GateAccount& account);
};

