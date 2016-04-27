#include "StdAfx.h"
#include "WatchDog.h"
#include "FunctionBase.h"
#include "MyLog.h"
#include "TimeManager.h"

WatchDog::WatchDog(void)
	: MyThread("Watch Dog")
{
	Init( new Function_Arg0<WatchDog>( this, &WatchDog::Thread_Update));
}


WatchDog::~WatchDog(void)
{

}

void WatchDog::RegWatchDog(int32 nIdx, char* sName, uint32 MaxTime /* = 5000 */)
{
	AUTOLOCK(m_lock);

	if( nIdx < 0 || nIdx >= MAX_WATCH_STATE_SIZE || sName == NULL)
		return;

	WatchState& state = m_watchStates[nIdx];
	if( state.UseFlag )
		return;

	state.Name = sName;
	state.UseFlag = true;
	state.WatchTime = TimeManager::Instance().CurTime();
	state.Step = 0;
	state.MaxTime = MaxTime;
}

void WatchDog::UpdateTime(int32 nIdx)
{
	if ( nIdx < 0 || nIdx >= MAX_WATCH_STATE_SIZE)
		return;

	WatchState& state = m_watchStates[nIdx];
	if(!state.UseFlag)
		return;

	state.WatchTime = TimeManager::Instance().CurTime();
	state.Step = 0;
}

void WatchDog::Thread_Update()
{
	uint64 nCurTime = TimeManager::Instance().CurTime();
	for (int32 i = 0; i < MAX_WATCH_STATE_SIZE; ++i)
	{
		WatchState& state = m_watchStates[i];
		if(!state.UseFlag)
			continue;

		uint64 nDeltaTime = nCurTime - state.WatchTime;
		if(nDeltaTime <= state.MaxTime)
			continue;

		MyLog::error( "WatchDog:[%s] Time [%llu] Step [%u] Error !", state.Name.c_str(), nDeltaTime, state.Step);
	}

	Sleep(1000);
}

void WatchDog::Stop()
{
	Exit();
	Wait();
}
