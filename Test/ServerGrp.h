#pragma once

#include "MySockAddr.h"
#include <map>
#include <string>

class ServerInfo;
class ServerManager;
class PacketSender;

typedef std::map< int32, ServerInfo*> SrvInfoMap;

class ServerGrp
{
public:
	ServerGrp(void);
	virtual ~ServerGrp(void);

	void Init(ServerManager* pMgr, int32 nSrvType);

	ServerInfo* CreateSrvInfo( int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr);
	ServerInfo* AddServer( int32 nID, int32 nSocketID, SockAddr& laddr);
	ServerInfo*	GetServer(int32 nID);
	
	void BroadcastPacket(class PacketBase* pPkt, int32 nExceptID = -1);
	void PeerSend( int32 nSocketID, class PacketBase* pPkt);

	int32 ServerCnt() { return (int32)m_SrvMap.size();}

	virtual void SortList(){}

	bool RemoveServer(int32 nID);
	void InitDogDetailsPools( PacketSender* pSender, int32 nDogSrvID);
	void BroadcastDogDetailsPools(class ModuleBase* pModule);
	void UpdateDogDetailsPools();

	SrvInfoMap		m_SrvMap;

	ServerManager*	m_pMgr;
	int32			m_nSrvType;
	std::string		m_strSrvName;

protected:
	virtual void OnAddServer(ServerInfo* pInfo){}
	virtual void OnRemoveServer(ServerInfo* pInfo){}
};

