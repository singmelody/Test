#pragma once
#include "Singleton.h"

class MyThread;

class MyCltManager : public Singleton<MyCltManager>
{
public:
	MyCltManager(void);
	~MyCltManager(void);

	void Connect2LoginSrv(const char* sAddr, int32 nPort);
	void InitNetManager();
	virtual void Init();
	virtual void Tick(bool bSleep);

	void ThreadRun();
	void SendPacket(int32 nSocketID, PacketBase* pkt);
protected:
	MyThread*	m_pThread;
};

