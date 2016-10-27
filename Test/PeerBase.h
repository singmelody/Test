#pragma once

#include "PacketSender.h"
#include <map>

class NetManager;

class PeerBase : public PacketSender
{
	typedef std::map< int32, int32> PacketSendCounter;
	typedef std::map< int32, int32> PeerSendPacketCounter;

public:
	PeerBase(void);
	~PeerBase(void);

	bool PeerInit();
	void FillPeerConfig();

	virtual bool PeerRun();
	virtual bool PeerExit();
	virtual int PeerProcPacket();

	void PeerSend( PacketBase* pPkt, int32 nSocketID);

	void W2LPacketCounter( int32 nPacketType);
protected:
	NetManager* m_pPeerNetManager;

	std::string	m_peerIP;
	int32		m_peerPort;
	bool		m_peerUseIOCP;

	PacketSendCounter m_PeerSendPacketCounter;
	
	PacketSendCounter m_W2GPacketCounter;
	PacketSendCounter m_G2WPacketCounter;

	PacketSendCounter m_N2GPacketCounter;
	PacketSendCounter m_G2NPacketCounter;

	PacketSendCounter m_N2WPacketCounter;
	PacketSendCounter m_W2NPacketCounter;

	PacketSendCounter m_L2WPacketCounter;
	PacketSendCounter m_W2LPacketCounter;
};

