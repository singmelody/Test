#pragma once

#include "Singleton.h"
#include "ServerGrpInfo.h"
#include "NodeSrvGrp.h"
#include "GateSrvGrp.h"

class PeerModuleBase;
class ServerInfo;

enum SrvMask
{
	eSM_World = 1 << 1,
	eSM_Login = 1 << 2,
	
	eSM_LocalNode	= 1 << 3,
	eSM_RemoteNode	= 1 << 4,
	eSM_Gate		= 1 << 5,
};

enum SrvSegment
{
	eSrvID_ZoneStartBit = 24,	// 24-31
	eSrvID_GrpStartBit	= 18,	// 18-23
	eSrvID_TypeStartBit	= 13,	// 13-17
	
	eSrvID_ZoneBits		= 8,
	eSrvID_GrpBits		= 6,
	eSrvID_TypeBits		= 5,
	eSrvID_IndexBits	= 13,

	eSrvID_MaxZoneID	= ( 1 << eSrvID_ZoneBits) - 1,
	eSrvID_MaxGrpID		= ( 1 << eSrvID_GrpBits) - 1,
	eSrvID_MaxTypeID	= ( 1 << eSrvID_TypeBits) - 1,
	eSrvID_MaxIndexBits	= ( 1 << eSrvID_IndexBits) - 1,
};



const char* GetSrvTitle( int32 nSrvType);

class PacketBase;

class ServerManager : public Singleton<ServerManager>
{
public:
	ServerManager(void);
	~ServerManager(void);

	static int32 GetZoneID(int32 nSrvID);
	static int32 GetGroupID(int32 nSrvID);
	static int32 GetSrvType(int32 nSrvID);
	static int32 GetSrvIndex(int32 nSrvID);

	static int32 MakeSrvID( int32 nZoneID, int32 nGrpID, int32 nSrvType, int32 nIdx);
	int32 MakeSrvID( int32 nSrvType, int32 nSrvIdx);

	void SetPeerModule(PeerModuleBase* pPeerModule);

	ServerInfo* GetLoginInfo();
	ServerInfo* GetWarWorldInfo( PacketBase* pPkt);

	void SetSrvType( SrvType nSrvType);
protected:
	int32			m_nZoneID;	// ¥Ûµÿ∑…”•
	int32			m_nGrpID;	// ∑ÔªÀºØ

	int32			m_localWorldChannelID;

	ServerInfo*		m_pWarWorld;
	ServerInfo*		m_pLocalWorld;
	ServerInfo*		m_pLogin;
	ServerInfo*		m_pDBA;
	ServerInfo*		m_pGMI;

	PeerModuleBase* m_pPeerModule;
	SrvType			m_srvType;

	NodeSrvGrp		m_LocalNodeGrp;
	NodeSrvGrp		m_RemoteNodeGrp;

	GateSrvGrp		m_GateGrp;

	ServerGrp		m_NodeDataSyncGrp;
	ServerGrp		m_RemoteWorldGrp;
};

