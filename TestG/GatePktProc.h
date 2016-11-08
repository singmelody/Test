#pragma once

#include "BaseType.h"
#include "MyPacketProc.h"
#include "GateFuncExManager.h"

class GatePeerPacketProc : public GateFuncExManager, public MyPacketProc
{
public:
	GatePeerPacketProc();
	virtual ~GatePeerPacketProc(){}

	virtual bool	OnPacketReceived(PacketBase* pPkt);
	virtual int32	DoProc();

private:
	void	DoProcOne( PacketBase* pPkt );
	void	ProcessFSMExtendDataPacket( class PacketFSMExtendData* pPkt, uint16 nChannelCnt, int16 channelList[]);
};

class GateCltPacketProc : public GateFuncExManager, public MyPacketProc
{
public:
	GateCltPacketProc();
	virtual ~GateCltPacketProc(){}

	virtual int32	DoProc();
};

