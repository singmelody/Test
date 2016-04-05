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
};

class SyncPacketArg
{
public:
	SyncPacketArg();

	PacketBase*	m_pSender;

	virtual void SendPacket( PacketBase* pPacket) = 0;
};

class SyncPacketArg2 : public SyncPacketArg
{
public:
	virtual void SendPacket( PacketBase* pPacket);
};


