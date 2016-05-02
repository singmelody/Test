#include "StdAfx.h"
#include "AvatarOnLineManager.h"
#include "PacketImpl.h"

AvatarOnLineManager::AvatarOnLineManager(void)
	: m_nMaxOnlineUser(0)
	, m_nTimeAccMaxOnlineUser(0)
	, config_MaxOnlineUser(0)
	, config_AccMaxOnlineUser_Enable(false)
	, config_AccMaxOnlineUser_InitValue(0)
	, config_AccMaxOnlineUser_DeltaTime(0)
	, config_AccMaxOnlineUser_Step(0)
{

}


AvatarOnLineManager::~AvatarOnLineManager(void)
{
}

void AvatarOnLineManager::Tick(int32 nFrameTime)
{
	ProcOnlineUserInfo(nFrameTime);
	ProcAccMaxOnlineUser(nFrameTime);
}

void AvatarOnLineManager::ProcOnlineUserInfo(int32 nFrameTime)
{
	static int32 s_interval = 0;
	s_interval += nFrameTime;

	// 10s sync once
	if(s_interval < 10*1000)
		return;

	SyncOnlineUserInfo2Login();
	s_interval = 0;
}

void AvatarOnLineManager::SyncOnlineUserInfo2Login()
{
	PacketOnlineUserInfo pkt;
	pkt.nMaxCount = GetMaxOnlineUser();
	pkt.nCount = GetOnlineAvatarCount();
	Send2Login(pkt);
}


