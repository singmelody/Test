#pragma once

class PacketBase;

class PacketSender
{
public:
	PacketSender(void);
	virtual ~PacketSender(void);

	static void RegSync2ModuleArg();

	virtual void Send2World(PacketBase* pPkt){}
	virtual void Send2DBA( PacketBase* pPkt){}
	virtual void Send2Node( class PacketBase* pPkt, int32 nSrvID);
	virtual void Send2Gate( class PacketBase* pPkt, bool bGateProc = false);

	virtual void MulticastGate( PacketBase* pPkt, class CasterTrunkGate* pTrunk, int32 exceptGateSrvID, int32 exceptGateChannelID);
};

class SyncPacketArg
{
public:
	SyncPacketArg();

	PacketSender*	m_pSender;

	virtual void SendPacket( PacketBase* pPacket) = 0;
};

typedef void (PacketSender::*SENDPARAMFUNC1)( PacketBase* pPkt);
typedef void (PacketSender::*SENDPARAMFUNC2)( PacketBase* pPkt, int32 nParam);
typedef void (PacketSender::*SENDPARAMFUNC3)( PacketBase* pPkt, bool bParam);

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


