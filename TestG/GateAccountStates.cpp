#include "StdAfx.h"
#include "GateAccountStates.h"
#include "GateServer.h"
#include "GateAccount.h"
#include "MyLog.h"

GateAccountStateBase* GateAccountStateBase::s_states[eGateAccountState_Count];



GateAccountStateBase::GateAccountStateBase(GateAccountState state)
	: m_state(state)
	, GateSrv(GateServer::Instance())
	, AccountMgr(GateAccountManager::Instance())
{

}

void GateAccountStateBase::DefaultHandlePacket( GateAccount& account, class PacketBase& pkt, const char* pSzFrom)
{
	assert(pSzFrom);

	MyLog::error("DefaultHandlePacket Packet=[%s] from[%s] State=[%s] UserCN=[%s]",
		pkt.GetClassName(), pSzFrom, GetStateName(), account.strUserCN.c_str() );
}

void GateAccountStateBase::Init()
{
	s_states[eGateAccountState_CltConnected]			= GateAccountState_ClntConnected::InstancePtr();
	s_states[eGateAccountState_CltReady]				= GateAccountState_CltReady::InstancePtr();
	s_states[eGateAccountState_WaitingUserData]			= GateAccountState_WaitingUserData::InstancePtr();
	s_states[eGateAccountState_Ready2EnterGame]			= GateAccountState_Ready2EnterGame::InstancePtr();

	s_states[eGateAccountState_CreatingAvatar]			= GateAccountState_CreatingAvatar::InstancePtr();
	s_states[eGateAccountState_CreatingAvatarFinish]	= GateAccountState_CreatingAvatarFinish::InstancePtr();
	s_states[eGateAccountState_DeletingAvatar]			= GateAccountState_DeletingAvatar::InstancePtr();
	s_states[eGateAccountState_RequestEnterGame]		= GateAccountState_RequestEnterGame::InstancePtr();

	s_states[eGateAccountState_Gaming]					= GateAccountState_Gaming::InstancePtr();
	s_states[eGateAccountState_Destroy]					= GateAccountState_Destroy::InstancePtr();	
}

void GameAccountState_CreateAvatar::OnEnter(GateAccount& account)
{

}
