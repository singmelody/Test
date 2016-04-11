#pragma once

#include <map>
#include "BaseType.h"

enum SrvType
{
	eSrv_Unkown = 0,
	eSrv_Login,
	eSrv_DBA,	
	eSrv_World,
	eSrv_Gate,
	eSrv_Node,
	eSrv_NodeSHM,
	eSrv_Count
};

class PacketAddSrvInfo;
class ServerGrp;
class ParamPool;
class SockAddr;

typedef std::map< int32, ParamPool*> DogDataMap;


class ServerInfo
{
public:
	int32		nSrvID;
	int32		nSrvType;
	int32		nSocketID;

	int32		nListenPortPeer;
	char		ListenIPPeer[IPLEN];

	ServerGrp*	m_pParentGrp;

	bool		m_bUseSHM;

	ServerInfo();
	ServerInfo( int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr);
	virtual ~ServerInfo();

	virtual void FillPacket( PacketAddSrvInfo* pPkt);

	DogDataMap m_DogDataMap;
	uint64 m_DogDataTimeStamp;

	ParamPool* GetDogData( int32 paramType );
	ParamPool* GetDogData( int32 paramID, int32 nDataID);

	ParamPool* m_pParamDetails;

	virtual void UpdateDetailsPool() {}
};