#pragma once

#include "PacketBase.h"
#include "MyPacket.h"
#include "PacketParamPool.h"

PACKET( PacketPack, PacketPackBase)
START_ADD_PACKET_MEMBER(PacketPack)
PACKET_END(PacketPack)

PACKET( PacketAvatarDecommision, PacketBase)
START_ADD_PACKET_MEMBER(PacketAvatarDecommision)
PACKET_END(PacketAvatarDecommision)

PACKET( PacketAddSrvInfo, PacketBase)
int32 nSrvType;
int32 nSrvID;
int32 nListenPortPeer;
int32 ListenIpPeer[IPLEN];
int32 nListenPortClt;
char  ListenIpClt[IPLEN];
START_ADD_PACKET_MEMBER(PacketAddSrvInfo)
	ADD_PACKET_MEMBER( nSrvType, int32, SrvType);
	ADD_PACKET_MEMBER( nSrvID, int32, SrvID);
	ADD_PACKET_MEMBER( nListenPortPeer, int32, nListenPortPeer);
	ADD_PACKET_MEMBER_STR( ListenIpPeer, IPLEN, NetIPPeer);
	ADD_PACKET_MEMBER( nListenPortClt, int32, listenPortClt);
	ADD_PACKET_MEMBER_STR( ListenIpClt, IPLEN, NetIPClt);
PACKET_END(PacketAddSrvInfo)

PACKET(PacketOnlineUserInfo, PacketBase)
int32 nMaxCount;
int32 nCount;
START_ADD_PACKET_MEMBER(PacketOnlineUserInfo)
	ADD_PACKET_MEMBER( nMaxCount, int32, nMaxCount);
	ADD_PACKET_MEMBER( nCount, int32, nCount);
PACKET_END(PacketOnlineUserInfo)


PACKET(PacketUserData, PacketParamPool)
int64 nAvatarDID;
int32 nIndex;
START_ADD_PACKET_MEMBER(PacketUserData)
	ADD_PACKET_MEMBER( nAvatarDID, int64, nAvatarDID);
	ADD_PACKET_MEMBER( nIndex, int32, nIndex);
PACKET_END(PacketUserData)