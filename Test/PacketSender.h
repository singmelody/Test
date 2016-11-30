#pragma once

#include "Factory.h"

class PacketBase;

class PacketSender
{
public:
	PacketSender(void);
	virtual ~PacketSender(void);

	static void RegSync2ModuleArg();

	virtual void Send2World(PacketBase* pPkt){}
	virtual void Send2Login( PacketBase* pPkt){}
	virtual void Send2DBA( PacketBase* pPkt){}
	virtual void Send2Node( class PacketBase* pPkt, int32 nSrvID){}
	virtual void Send2Gate( class PacketBase* pPkt, bool bGateProc = false){}
	virtual void Send2Dog( class PacketBase* pPkt, int32 nSrvID = -1) {}

	virtual void Multicast2Gate( PacketBase* pPkt, class CasterTrunkGate* pTrunk, int32 nExceptID, int32 exceptGateChannelID){}
	virtual void Multicast2Node( PacketBase* pPkt, class CasterTrunkGate* pTrunk, int32 nExceptID){}
};

typedef void (PacketSender::*SENDPARAMFUNC1)( PacketBase* pPkt);
typedef void (PacketSender::*SENDPARAMFUNC2)( PacketBase* pPkt, int32 nParam);
typedef void (PacketSender::*SENDPARAMFUNC3)( PacketBase* pPkt, bool bParam);


class SyncPacketArg
{
public:
	SyncPacketArg();
	virtual ~SyncPacketArg(){}

	PacketSender*	m_pSender;

	virtual void SendPacket( PacketBase* pPacket) = 0;
};

class SyncPacketArg1 : public SyncPacketArg
{
public:
	virtual void SendPacket( PacketBase* pPacket)
	{
		(m_pSender->*m_pFunc)( pPacket );
	}

	SENDPARAMFUNC1 m_pFunc;
};


class SyncPacketArg2 : public SyncPacketArg
{
public:
	virtual void SendPacket( PacketBase* pPacket)
	{
		(m_pSender->*m_pFunc)( pPacket, m_nParam);
	}

	SENDPARAMFUNC2 m_pFunc;
	int32 m_nParam;
};

class SyncPacketArg3 : public SyncPacketArg
{
public:
	virtual void SendPacket( PacketBase* pPacket)
	{
		(m_pSender->*m_pFunc)( pPacket, m_bParam);
	}

	SENDPARAMFUNC3 m_pFunc;
	bool m_bParam;
};

class Sync2DBAArg : public SyncPacketArg
{
	DECLARE_FACTORY_ARG0( Sync2DBAArg, -1, new PoolAllocator)
public:
	Sync2DBAArg(){}

	Sync2DBAArg(PacketSender* pSender)
	{
		m_pSender = pSender;
	}

	virtual void SendPacket( PacketBase* pPacket)
	{
		m_pSender->Send2DBA(pPacket);
	}
};

class Sync2WorldArg : public SyncPacketArg
{
	DECLARE_FACTORY_ARG0( Sync2WorldArg, -1, new PoolAllocator);
public:
	Sync2WorldArg()
	{

	}

	Sync2WorldArg( PacketSender* pSender)
	{
		m_pSender = pSender;
	}

	virtual void SendPacket(PacketBase* pPacket)
	{
		m_pSender->Send2World(pPacket);
	}
};

class Sync2NodeArg : public SyncPacketArg
{
	DECLARE_FACTORY_ARG0( Sync2NodeArg, -1, new PoolAllocator);
public:
	Sync2NodeArg()
	{
		m_nNodeSrvID = SERVERID_NULL;
	}

	Sync2NodeArg( PacketSender* pSender, int32 nNodeSrvID)
	{
		m_pSender = pSender;
		m_nNodeSrvID = nNodeSrvID;
	}

	virtual void SendPacket(PacketBase* pPacket)
	{
		m_pSender->Send2Node(pPacket, m_nNodeSrvID);
	}

	int32 m_nNodeSrvID;
};