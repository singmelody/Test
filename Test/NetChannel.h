#pragma once

#include "NetChannelBase.h"

class PacketBase;

class NetChannel : public NetChannelBase
{
	friend class NetManager;
public:
	NetChannel(void);
	virtual ~NetChannel(void);

	virtual void CloseChannel();
	virtual void AppendPacket(PacketBase* pPkt);
};

