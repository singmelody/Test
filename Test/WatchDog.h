#pragma once

#include "Thread.h"
#include "Singleton.h"
#include "MyMutex.h"

#define MAX_WATCH_STATE_SIZE 8

class WatchDog : public MyThread, public Singleton<WatchDog>
{
	struct WatchState
	{
		bool UseFlag;
		uint64 WatchTime;
		uint32 Step;
		uint32 MaxTime;
		std::string Name;

		WatchState()
		{
			UseFlag = false;
			WatchTime = 0;
			Step = 0;
		}
	};
public:
	WatchDog(void);
	virtual ~WatchDog(void);

	virtual void Stop();

	void RegWatchDog(int32 nIdx, char* sName, uint32 MaxTime = 5000);
	void UpdateTime(int32 nIdx);
	void NextStep(int32 nIdx);

protected:
	void Thread_Update();

	Mutex		m_lock;
	WatchState	m_watchStates[MAX_WATCH_STATE_SIZE];
};

