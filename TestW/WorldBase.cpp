#include "StdAfx.h"
#include "WorldBase.h"


WorldBase::WorldBase(void)
	: PeerModuleBase( Srv_World )
{
	m_olAvatarCount = 0;
	m_bWarWorld = false;
	m_pWorldDogPool = NULL;
}


WorldBase::~WorldBase(void)
{
}


bool WorldBase::Init(int32 nArgc, char* argv[])
{
	if(!PeerModuleBase::Init( nArgc, argv))
		return false;

	return true;
}

void WorldBase::ProcessConsole(int32 nFrameTime)
{
	if( SERVER_TICK_TIME_SHOW > 0 )
	{
		static uint32 nTickShowTime = 0;
		nTickShowTime += nFrameTime;

		if( nTickShowTime >= SERVER_TICK_TIME_SHOW)
		{
			nTickShowTime = 0;

			CalculateStatistics();
			ResetStatistics();

			WatchDog::Instance().NextStep(GAME_THREAD_WATCHDOG_ID);
			MyLog::performance("[%s] Tick: %dms PlayerOnline: %d", m_strModuleTitle.c_str(), m_nAvgFrameTime, );

		}
	}
}

bool WorldBase::CreateDogPool()
{
	if( !PeerModuleBase::CreateDogPool() )
		return false;

	m_pWorldDogPool = ParamSet::CreateNew( eParam_WorldInfo, 0);
	if(!m_pWorldDogPool)
		return false;

	m_listDogData.push_back( m_pWorldDogPool );

	return true;
}