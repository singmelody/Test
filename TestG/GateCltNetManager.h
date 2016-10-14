#pragma once

#include "BaseType.h"
#include "FunctionBase.h"
#include "NetChannelBase.h"
#include "NetChannel.h"
#include "GateFuncExManager.h"
#include "NetManager.h"

class GateCltNetManager : public BASENETMANAGER, public GateFuncExManager
{
public:
	GateCltNetManager(
		bool bLZOCompresss,
		int32 sockRcBuffSize,
		int32 rcBuffSize,
		int32 sockSnBuffSize,
		int32 snBuffSize,
		FunctionBase_Arg1<int32>* funcAccept = NULL,
		FunctionBase_Arg1<int32>* funcCon = NULL,
		FunctionBase_Arg1<int32>* funcDisCon = NULL,
		int32 MAX_SOCKETS = MY_SOCKET_LIST_SIZE
		);
	~GateCltNetManager(void);

	virtual NetChannelBase* CreateNewChannel();
	virtual void OnReceivedPacket( NetChannelBase* pChannel, PacketBase* pPkt);

	bool Dispatch2PeerChannel( int32 nChannel, PacketBase* pPkt);
};

class GateCltNetChannel : public NetChannel
{
public:
	GateCltNetChannel();
};