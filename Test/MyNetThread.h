#pragma once

#include "MyProcessThread.h"
#include <vector>

class NetEventHandler;

class MyNetThread : public MyProcessThread
{
	typedef std::vector<std::pair<NetEventHandler*, uint32>> HANDLER_VEC;
public:
	MyNetThread(void);
	virtual ~MyNetThread(void);

	void RegisterHandler(NetEventHandler* pHandler, uint32 nEvent);

	uint32 GetLoad() const{
		return m_nNumOfHandle;
	}

protected:
	virtual bool OnInit();
	virtual void OnQuit();
	virtual bool Process(const Time& time);

private:
	HANDLER_VEC		m_waitList;

	uint32			m_nNumOfHandle;	// 
};

