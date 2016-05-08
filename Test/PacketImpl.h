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

PACKET(PacketUserDataRegFinish, PacketBase)
START_ADD_PACKET_MEMBER(PacketUserDataRegFinish)
PACKET_END(PacketUserDataRegFinish)

PACKET(PacketCltSelectAvatarFailed, PacketBase)
uint8 nReason;
uint8 nParam;
enum FailedReason
{
	eFR_Unkown,
	eFR_BadIndex,
	eFR_AvatarBanded,
	eFR_BillingFailed,
	eFR_EnterSceneFailed
};
START_ADD_PACKET_MEMBER(PacketCltSelectAvatarFailed)
ADD_PACKET_MEMBER( nReason, uint8, nReason);
ADD_PACKET_MEMBER( nParam, uint8, nParam);
PACKET_END(PacketCltSelectAvatarFailed)

PACKET(PacketCltSelectAvatar, PacketBase)
uint8 nAvatarIdx;
START_ADD_PACKET_MEMBER(PacketCltSelectAvatar)
	ADD_PACKET_MEMBER( nAvatarIdx, uint8, nAvatarIdx);
PACKET_END(PacketCltSelectAvatar)

PACKET(PacketCommonDataInit, PacketBase)
START_ADD_PACKET_MEMBER(PacketCommonDataInit)
PACKET_END(PacketCommonDataInit)

PACKET(PacketCommonDataCreate, PacketBase)
int32 nFlag;
START_ADD_PACKET_MEMBER(PacketCommonDataCreate)
	ADD_PACKET_MEMBER( nFlag, int32, nFlag);
PACKET_END(PacketCommonDataCreate)

PACKET(PacketCommonDataUpdate, PacketBase)
START_ADD_PACKET_MEMBER(PacketCommonDataUpdate)
PACKET_END(PacketCommonDataUpdate)

PACKET(PacketCommonDataDelete, PacketBase)
int32 nDelFlag;
START_ADD_PACKET_MEMBER(PacketCommonDataDelete)
	ADD_PACKET_MEMBER( nDelFlag, uint8, nDelFlag);
PACKET_END(PacketCommonDataDelete)

