#pragma once
#include "Singleton.h"
#include "PacketBase.h"
#include <map>

typedef std::map<std::string, int32> MapTicks;


class MyThread;

class MyCltManager : public Singleton<MyCltManager>
{
public:
	MyCltManager(void);
	~MyCltManager(void);

	void Connect2LoginSrv(const char* sAddr, int32 nPort);
	
	void SendCltSelectAvatar(int32 nIdx);
	void SendCltCreateCharacter(const std::string& strRoleName);
	void SendCltCreateDefaultCharacter();
	void SendCltDelCharacter(int32 nIdx);

	void InitNetManager();
	virtual void Init();
	virtual void Tick(bool bSleep);

	void ThreadRun();
	void SendPacket(int32 nSocketID, PacketBase* pkt);
protected:
	int32 GetTicks(const std::string& key);
	int32 SetTicks(const std::string& key);

	MapTicks					m_mapTicks;

	std::map<int32, MyClt*>		m_pCltsLoginPending;
	MyThread*					m_pThread;

	// 
	MyAOIManager*				m_pAOIManager;
};

