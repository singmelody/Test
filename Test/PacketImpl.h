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

PACKET( PacketSrvConnect, PacketBase)
int32	nID;
uint8	nType;
int8	nModuleIndex;
uint8	nFlag;
int32	nListenPortPeer;
int32	ListenIpPeer[IPLEN];
int32	nListenPortClt;
char	ListenIpClt[IPLEN];

bool CheckFlag( uint8 mask)
{
	return ( nFlag & mask) != 0;
}

START_ADD_PACKET_MEMBER(PacketSrvConnect)
ADD_PACKET_MEMBER( nID, int32, nID)
ADD_PACKET_MEMBER( nType, uint8, nType)
ADD_PACKET_MEMBER( nModuleIndex, int8, nModuleIndex)
ADD_PACKET_MEMBER( nFlag, uint8, nFlag)
ADD_PACKET_MEMBER( nListenPortPeer, int32, nListenPortPeer)
ADD_PACKET_MEMBER_STR( ListenIpPeer, IPLEN, NetIPPeer)
ADD_PACKET_MEMBER( nListenPortClt, int32, listenPortClt)
ADD_PACKET_MEMBER_STR( ListenIpClt, IPLEN, NetIPClt)
PACKET_END(PacketSrvConnect)

PACKET( PacketAddSrvInfo, PacketBase)
int32 nSrvType;
int32 nSrvID;
int32 nListenPortPeer;
int32 ListenIpPeer[IPLEN];
int32 nListenPortClt;
char  ListenIpClt[IPLEN];
START_ADD_PACKET_MEMBER(PacketAddSrvInfo)
	ADD_PACKET_MEMBER( nSrvType, int32, SrvType)
	ADD_PACKET_MEMBER( nSrvID, int32, SrvID)
	ADD_PACKET_MEMBER( nListenPortPeer, int32, nListenPortPeer)
	ADD_PACKET_MEMBER_STR( ListenIpPeer, IPLEN, NetIPPeer)
	ADD_PACKET_MEMBER( nListenPortClt, int32, listenPortClt)
	ADD_PACKET_MEMBER_STR( ListenIpClt, IPLEN, NetIPClt)
PACKET_END(PacketAddSrvInfo)

PACKET(PacketOnlineUserInfo, PacketBase)
int32 nMaxCount;
int32 nCount;
START_ADD_PACKET_MEMBER(PacketOnlineUserInfo)
	ADD_PACKET_MEMBER( nMaxCount, int32, nMaxCount)
	ADD_PACKET_MEMBER( nCount, int32, nCount)
PACKET_END(PacketOnlineUserInfo)


PACKET(PacketUserData, PacketParamPool)
int64 nAvatarDID;
int32 nIndex;
START_ADD_PACKET_MEMBER(PacketUserData)
	ADD_PACKET_MEMBER( nAvatarDID, int64, nAvatarDID)
	ADD_PACKET_MEMBER( nIndex, int32, nIndex)
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
ADD_PACKET_MEMBER( nReason, uint8, nReason)
ADD_PACKET_MEMBER( nParam, uint8, nParam)
PACKET_END(PacketCltSelectAvatarFailed)

PACKET(PacketCommonDataInit, PacketBase)
START_ADD_PACKET_MEMBER(PacketCommonDataInit)
PACKET_END(PacketCommonDataInit)

PACKET(PacketCommonDataCreate, PacketBase)
int32 nFlag;
START_ADD_PACKET_MEMBER(PacketCommonDataCreate)
	ADD_PACKET_MEMBER( nFlag, int32, nFlag)
PACKET_END(PacketCommonDataCreate)

PACKET(PacketCommonDataUpdate, PacketBase)
START_ADD_PACKET_MEMBER(PacketCommonDataUpdate)
PACKET_END(PacketCommonDataUpdate)

PACKET(PacketCommonDataDelete, PacketBase)
int32 nDelFlag;
START_ADD_PACKET_MEMBER(PacketCommonDataDelete)
ADD_PACKET_MEMBER( nDelFlag, uint8, nDelFlag)
PACKET_END(PacketCommonDataDelete)

PACKET(PacketGateDestroyAvatar,PacketBase)
START_ADD_PACKET_MEMBER(PacketGateDestroyAvatar)
PACKET_END(PacketGateDestroyAvatar)

PACKET(PacketSystemConfigRequest,PacketBase)
uint8 nAccountLen;
char account[MAX_ACCOUNT];
int64 nAvatarDID;
START_ADD_PACKET_MEMBER(PacketSystemConfigRequest)
ADD_PACKET_MEMBER( nAccountLen, uint8, nAccountLen)
ADD_PACKET_MEMBER_STR( account, MAX_ACCOUNT, account)
ADD_PACKET_MEMBER( nAvatarDID, int64, nAvatarDID)
PACKET_END(PacketSystemConfigRequest)

PACKET(PacketRoleListRequest,PacketBase)
uint8 nAccountNameLen;
char AccountName[MAX_ACCOUNT];
START_ADD_PACKET_MEMBER(PacketRoleListRequest)
	ADD_PACKET_MEMBER( nAccountNameLen, uint8, nAccountNameLen)
	ADD_PACKET_MEMBER_STR( AccountName, MAX_ACCOUNT, AccountName)
PACKET_END(PacketRoleListRequest)

enum
{
	eAvatarData_Select = 0,
	eAvatarData_Normal = 1,
	eAvatarData_Destroy = 2,
};
PACKET_EX( PacketAvatarData2DBA, PacketParamPool, new PoolPacketAllocator(2048))
uint32 nParamType;
int32 nSrcAvatarID;
int32 nFlag;
int64 nAvatarDID;
START_ADD_PACKET_MEMBER(PacketAvatarData2DBA)
	ADD_PACKET_MEMBER( nParamType, uint32, nParamType)
	ADD_PACKET_MEMBER( nSrcAvatarID, int32, nSrcAvatarID)
	ADD_PACKET_MEMBER( nFlag, int32, nFlag)
	ADD_PACKET_MEMBER( nAvatarDID, int64, nAvatarDID)
PACKET_END(PacketAvatarData2DBA)

// CLT 2 Gate
PACKET_CLT_2_GATE(PacketCltSelectAvatar, PacketBase)
uint8 nAvatarIdx;
START_ADD_PACKET_MEMBER(PacketCltSelectAvatar)
ADD_PACKET_MEMBER( nAvatarIdx, uint8, nAvatarIdx)
PACKET_END(PacketCltSelectAvatar)

