#include "StdAfx.h"
#include "WorldState_WaitingNodeData.h"


WorldState_WaitingNodeData::WorldState_WaitingNodeData(void)
{
}


WorldState_WaitingNodeData::~WorldState_WaitingNodeData(void)
{
}

void WorldState_WaitingNodeData::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncAvatar, WorldState_WaitingNodeData, PktNodeDataSync_AvatarData);
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncCommonData, );
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncFin, );
	REG_PACKET_HANDLER( pProc, PacketNodeDataSyncAllFinish, );

}
