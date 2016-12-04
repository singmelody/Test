#pragma once

#include "MyProcessThread.h"
#include <vector>
#include "MyMutex.h"

class NetEventHandler;
class NetReactor;

class MyNetThread : public MyProcessThread
{
	typedef std::vector<std::pair<NetEventHandler*, uint32>> HANDLER_VEC;
public:
	MyNetThread(uint32 nMaxNumOfHandle);
	virtual ~MyNetThread(void);

	void RegisterHandler(NetEventHandler* pHandler, uint32 nEvent);

	virtual uint32 GetLoad() const{ return m_nNumOfHandle; }

protected:
	virtual bool OnInit();
	virtual void OnQuit();
	virtual bool Process(const Time& time);

private:
	HANDLER_VEC		m_waitList;	// temp handle list
	Mutex			m_mutex;	
	NetReactor*		m_pReactor;
	uint32			m_nMaxNumOfHandle;


	uint32			m_nNumOfHandle;	// 
};

