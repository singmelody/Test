#pragma once

#include "BaseType.h"
#include "FunctionBase.h"
#include "NetChannelBase.h"
#include "NetChannel.h"
#include "GateFuncExManager.h"
#include "NetManager.h"
#include <hash_set>

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

	int32 m_nAvatarID;
	int32 m_nNodeSocketID;
	int32 m_nWorldSocketID;

	typedef stdext::hash_set<int32> AOISet;

	AOISet m_setAOI;

	bool ProcessFSMExtendDatas( class FSMExtendDataList& list);

	bool ProcessPrivatePakcet(PacketBase* pPkt);

	virtual void AppendPacket(PacketBase* pPkt);
protected:
	virtual void OnPacketParsed(PacketBase* pPkt);
	bool CheckPacketFrequence();

private:
	void OnAOIListOption( class PacketAOIListOption* pPkt);
	bool IsInAOI(int32 nAvatarID);



	int32	m_nCurCltPacketCnt;
	uint64	m_nCurCltPacketTime;
};