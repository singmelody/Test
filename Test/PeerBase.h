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

protected:
	NetManager* m_pPeerNetManager;

	std::string	m_peerIP;
	int32		m_peerPort;
	bool		m_peerUseIOCP;

	
};

