#pragma once

#include "PacketBase.h"
#include "MyPacket.h"
#include "PacketParamPool.h"
#include "MyFactory.h"

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

PACKET(PacketGateCreateAvatar,PacketBase)
uint8 nAvatarTitleLen;
char  avatarTitle[MAX_TITLE];
START_ADD_PACKET_MEMBER(PacketGateCreateAvatar)
ADD_PACKET_MEMBER( nAvatarTitleLen, uint8, nAvatarTitleLen)
ADD_PACKET_MEMBER_STR( avatarTitle, MAX_TITLE, avatarTitle)
PACKET_END(PacketGateCreateAvatar)

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

// dba 2 world
PACKET(PacketUserSelectData, PacketParamPool)
int64 nAvatarDID;
START_ADD_PACKET_MEMBER(PacketUserSelectData)
	ADD_PACKET_MEMBER( nAvatarDID, int64, nAvatarDID)
PACKET_END(PacketUserSelectData)

PACKET(PacketCommonDataRequest, PacketBase)
int64 nAvatarDID;
START_ADD_PACKET_MEMBER(PacketCommonDataRequest)
	ADD_PACKET_MEMBER( nAvatarDID, int64, nAvatarDID)
PACKET_END(PacketCommonDataRequest)

PACKET(PacketCommonDataReqFinish, PacketBase)
int32 nDataType;
START_ADD_PACKET_MEMBER(PacketCommonDataReqFinish)
	ADD_PACKET_MEMBER( nDataType, int32, nDataType)
PACKET_END(PacketCommonDataReqFinish)

PACKET(PacketGateCreateAvatarRst, PacketBase)
uint8 succ;
START_ADD_PACKET_MEMBER(PacketGateCreateAvatarRst)
	ADD_PACKET_MEMBER( succ, uint8, "")
PACKET_END(PacketGateCreateAvatarRst)

PACKET(PacketManufactureDataInit, PacketBaseEx<MAX_MANUFACTURE_RECIPE_COUNT*sizeof(int32)>)
START_ADD_PACKET_MEMBER(PacketManufactureDataInit)
PACKET_END(PacketManufactureDataInit)

PACKET(PacketChangeSceneFail, PacketBase)
int32 nErrorID;	// 1->scene not existing 2->srv busy 3->retry 4->create scene failed
START_ADD_PACKET_MEMBER(PacketChangeSceneFail)
PACKET_END(PacketChangeSceneFail)

PACKET(PacketAvatarCommission, PacketBase)
int32 info;
START_ADD_PACKET_MEMBER(PacketAvatarCommission)
	ADD_PACKET_MEMBER( info, int32, "")
PACKET_END(PacketAvatarCommission)

PACKET(PacketGateRouteNotify, PacketBase)
enum 
{
	eOpen = 0, eClose = 1,
};

int32 nNodeID;
uint8 nFlag;
START_ADD_PACKET_MEMBER(PacketGateRouteNotify)
	ADD_PACKET_MEMBER( nNodeID, int32, nNodeID)
	ADD_PACKET_MEMBER( nFlag, uint8, nFlag)
PACKET_END(PacketGateRouteNotify)


PACKET(PacketCltLogout, PacketBase)
START_ADD_PACKET_MEMBER(PacketCltLogout)
PACKET_END(PacketCltLogout)

enum
{
	eCreateAvatarRes_Fail	= 0,
	eCreateAvatarRes_Succ,
	eCreateAvatarRes_Fail_MaxCount,
	eCreateAvatarRes_Fail_NameExists,
};

PACKET(PacketCreateAvatarRes, PacketBase)
int32 nFlag;
int32 nIdx;
int32 nTypeID;
START_ADD_PACKET_MEMBER(PacketCreateAvatarRes)
ADD_PACKET_MEMBER( nFlag, int32 ,nFlag);
ADD_PACKET_MEMBER( nIdx, int32, nIdx);
ADD_PACKET_MEMBER( nTypeID, int32, nTypeID);
PACKET_END(PacketCreateAvatarRes)

enum DeleteAvatar
{
	eDelete_Avatar_Succ,
	eDelete_Avatar_Fail,
};

PACKET(PacketDelCharacterFinish, PacketBase)
int32 nFlag;
int64 nAvatarDID;
START_ADD_PACKET_MEMBER(PacketDelCharacterFinish)
ADD_PACKET_MEMBER( nFlag, int32 , nFlag);
ADD_PACKET_MEMBER( nAvatarDID, int64 ,nAvatarDID);
PACKET_END(PacketDelCharacterFinish)

PACKET(PacketDelCharacater2DBA, PacketBase)
int64 nAvatarDID;
int32 nAccountID;
START_ADD_PACKET_MEMBER(PacketDelCharacater2DBA)
ADD_PACKET_MEMBER( nAvatarDID, int64 , nAvatarDID);
ADD_PACKET_MEMBER( nAccountID, int32 , nAccountID);
PACKET_END(PacketDelCharacater2DBA)

PACKET(PacketCltSelectAvatarFail, PacketBase)

enum{
	eReason_Unkown,				// unkown error
	eReason_BadIdx,				// role is not existed
	eReason_AvatarBanded,		// baned in the game
	eReason_BillingFail,		// billing failed,
	eReason_EnterSceneFailed,	// failed to enter scene
};
uint8 nReason;
uint8 nParam;
START_ADD_PACKET_MEMBER(PacketCltSelectAvatarFail)
	ADD_PACKET_MEMBER( nReason, uint8 , nReason);
	ADD_PACKET_MEMBER( nParam, uint8 , nParam);
PACKET_END(PacketCltSelectAvatarFail)

enum
{
	eNetEvent_Accept,
	eNetEvent_Connect,
	eNetEvent_Disconnect,
};

PACKET(PacketNetEvent, PacketBase)
int32 nSocketID;
int32 nFlag;
IMPLE_GetStaticAttribute(PktAttr_IsEvent);
START_ADD_PACKET_MEMBER(PacketNetEvent)
	ADD_PACKET_MEMBER( nSocketID, int32 , nReason);
	ADD_PACKET_MEMBER( nFlag, int32, nParam);
PACKET_END(PacketNetEvent)

PACKET(PacketFSMExtendData, PacketDataPack<PACKET_EX_BUFF_MAX_SIZE>)
START_ADD_PACKET_MEMBER(PacketFSMExtendData)
PACKET_END(PacketFSMExtendData)

PACKET(PacketConnectGateSrv, PacketBase)
uint8 userCNLen;
char userCN[MAX_ACCOUNT];
char key[KEY_LENGTH];
IMPLE_GetStaticAttribute(PktAttr_IsClt2Srv)
START_ADD_PACKET_MEMBER(PacketConnectGateSrv)
	ADD_PACKET_MEMBER( userCNLen, uint8, userCNLen);
	ADD_PACKET_MEMBER_STR( userCN, MAX_ACCOUNT, "");
	ADD_PACKET_MEMBER_STR( key, KEY_LENGTH, "");
PACKET_END(PacketConnectGateSrv)

PACKET(PacketNewConnectioRet, PacketBase)
START_ADD_PACKET_MEMBER(PacketNewConnectioRet)
PACKET_END(PacketNewConnectioRet)

PACKET(PacketShortChangeScene, PacketBase)
int32 info;
int32 targetScene;
f32 x,y,z;
f32 dx,dy,dz;
START_ADD_PACKET_MEMBER(PacketShortChangeScene)
ADD_PACKET_MEMBER( info, int32, info);
ADD_PACKET_MEMBER( targetScene, int32, "");
ADD_PACKET_MEMBER( x, f32, "");
ADD_PACKET_MEMBER( y, f32, "");
ADD_PACKET_MEMBER( z, f32, "");
ADD_PACKET_MEMBER( dx, f32, "");
ADD_PACKET_MEMBER( dy, f32, "");
ADD_PACKET_MEMBER( dz, f32, "");
PACKET_END(PacketShortChangeScene)

enum
{
	eNode_Avatar_Transport	= 0,
	eNode_Avatar_Login		= 1,
};

PACKET(PacketNodeCreateAvatar, PacketBase)
int8  nLoginFlag;
int32 nFlag;
int32 nSceneID;
int32 nWorldID;
int32 nNodeID;
int32 nGateSrvID;
int32 nGateChannelID;
int32 nTypeID;
int64 nAvatarDID;
f32 x,y,z;
f32 dx,dy,dz;
int32 nState;
START_ADD_PACKET_MEMBER(PacketNodeCreateAvatar)

ADD_PACKET_MEMBER( nLoginFlag, int8, info);
ADD_PACKET_MEMBER( nFlag, int32, "");
ADD_PACKET_MEMBER( nSceneID, int32, "");
ADD_PACKET_MEMBER( nWorldID, int32, "");
ADD_PACKET_MEMBER( nNodeID, int32, "");
ADD_PACKET_MEMBER( nGateSrvID, int32, "");
ADD_PACKET_MEMBER( nGateChannelID, int32, "");
ADD_PACKET_MEMBER( nTypeID, int32, "");
ADD_PACKET_MEMBER( nAvatarDID, int64, "");
ADD_PACKET_MEMBER( x, f32, "");
ADD_PACKET_MEMBER( y, f32, "");
ADD_PACKET_MEMBER( z, f32, "");
ADD_PACKET_MEMBER( dx, f32, "");
ADD_PACKET_MEMBER( dy, f32, "");
ADD_PACKET_MEMBER( dz, f32, "");
ADD_PACKET_MEMBER( nState, int32, "");
PACKET_END(PacketNodeCreateAvatar)

PACKET(PacketChangeSceneNotify, PacketBase)
int32 nSceneID;
int32 nNodeID;
int32 nFlag;
enum
{
	eEnter,
	eLeave,
};
START_ADD_PACKET_MEMBER(PacketChangeSceneNotify)
ADD_PACKET_MEMBER( nSceneID, int32, "");
ADD_PACKET_MEMBER( nNodeID, int32, "");
ADD_PACKET_MEMBER( nFlag, int32, "");
PACKET_END(PacketChangeSceneNotify)

PACKET(PacketDestroyAvatar, PacketBase)
int32 nAvatarID;
START_ADD_PACKET_MEMBER(PacketDestroyAvatar)
	ADD_PACKET_MEMBER( nAvatarID, int32, "");
PACKET_END(PacketDestroyAvatar)

PACKET_EX(PacketMulticast2Avatar, PacketBroadCastBase<int32>, new PoolPacketAllocator(40960))
START_ADD_PACKET_MEMBER(PacketMulticast2Avatar)
PACKET_END(PacketMulticast2Avatar)

PACKET_EX(PacketMulticast2Channel, PacketBroadCastBase<int16>, new PoolPacketAllocator(40960))
START_ADD_PACKET_MEMBER(PacketMulticast2Channel)
PACKET_END(PacketMulticast2Channel)

// CLT 2 Gate
PACKET_CLT_2_GATE(PacketCreateAvatar, PacketBase)
int32	nIdx;
int16	nFlag;
uint8	nAccountLen;
char	account[MAX_ACCOUNT];
uint8	nAvatarTitleLen;
char	avatarTitle[MAX_TITLE];
int32	nParamTypeID;
int64	nAvatarDID;
int32	nFaceID;
int8	nSkinColor;
int8	nTattoo;
int8	nBeard_eyeshadow;
int8	nBrow;
int32   nHairID;
int8	nHairColor;
int32	nEyeID;
int32	nFashionID;
int32	nFashionColor;
int8	nLipColor;
int32	nMorph_1;
int32	nMorph_2;
int32	nMorph_3;
uint8	nHeight;
bool	bCheckData;
START_ADD_PACKET_MEMBER(PacketCreateAvatar)
ADD_PACKET_MEMBER( nIdx, int32 ,user_index);
ADD_PACKET_MEMBER( nFlag, int16, create_flag);
ADD_PACKET_MEMBER( nAccountLen, uint8, accountLen);
ADD_PACKET_MEMBER_STR( account, MAX_ACCOUNT, account_str);
ADD_PACKET_MEMBER( nAvatarTitleLen, uint8, nAvatarTitleLen);
ADD_PACKET_MEMBER_STR( avatarTitle, MAX_TITLE, avatarTitle);
ADD_PACKET_MEMBER( nParamTypeID, int32, nParamTypeID);
ADD_PACKET_MEMBER( nAvatarDID, int64, nAvatarDID);
ADD_PACKET_MEMBER( nFaceID, int32, nFaceID);
ADD_PACKET_MEMBER( nSkinColor, int8, nSkinColor);
ADD_PACKET_MEMBER( nTattoo, int8, nTattoo);
ADD_PACKET_MEMBER( nBeard_eyeshadow, int8, nBeard_eyeshadow);
ADD_PACKET_MEMBER( nBrow, int8, nBrow);
ADD_PACKET_MEMBER( nHairID, int32, nHairID);
ADD_PACKET_MEMBER( nHairColor, int8, nHairColor);
ADD_PACKET_MEMBER( nEyeID, int32, nEyeID);
ADD_PACKET_MEMBER( nFashionID, int32, nFashionID);
ADD_PACKET_MEMBER( nFashionColor, int32, nFashionColor);
ADD_PACKET_MEMBER( nLipColor, int8, nLipColor);
ADD_PACKET_MEMBER( nMorph_1, int8, nMorph_1);
ADD_PACKET_MEMBER( nMorph_2, int8, nMorph_2);
ADD_PACKET_MEMBER( nMorph_3, int8, nMorph_3);
ADD_PACKET_MEMBER( nHeight, uint8, nHeight);
ADD_PACKET_MEMBER( bCheckData, bool, bCheckData);
PACKET_END(PacketCreateAvatar)

PACKET_CLT_2_GATE(PacketDelCharacter, PacketBase)
int32 nIdx;
START_ADD_PACKET_MEMBER(PacketDelCharacter)
ADD_PACKET_MEMBER( nIdx, int32 ,user_index);
PACKET_END(PacketDelCharacter)


PACKET_CLT_2_GATE(PacketCltSelectAvatar, PacketBase)
uint8 nAvatarIdx;
START_ADD_PACKET_MEMBER(PacketCltSelectAvatar)
ADD_PACKET_MEMBER( nAvatarIdx, uint8, nAvatarIdx)
PACKET_END(PacketCltSelectAvatar)

PACKET_CLT_2_GATE( PacketCltReady, PacketBase)
int32 nToken;
START_ADD_PACKET_MEMBER(PacketCltReady)
	ADD_PACKET_MEMBER( nToken, uint8, nToken);
PACKET_END(PacketCltReady)


PACKET_CLT_2_GATE(PacketLogout2Login, PacketBase)
int32 nGateID;
START_ADD_PACKET_MEMBER(PacketLogout2Login)
	ADD_PACKET_MEMBER( nGateID, int32, nGateID);
PACKET_END(PacketLogout2Login)


// Packet_Clt
PACKET_CLT(PacketCltSceneReq, PacketBase)
int32 nTeleportID;
int32 nSceneID;
f32 x,y,z;
START_ADD_PACKET_MEMBER(PacketCltSceneReq)
ADD_PACKET_MEMBER( nTeleportID, int32, nTeleportID);
ADD_PACKET_MEMBER( nSceneID, int32, nSceneID);
ADD_PACKET_MEMBER( x, f32, "");
ADD_PACKET_MEMBER( y, f32, "");
ADD_PACKET_MEMBER( z, f32, "");
PACKET_END(PacketCltSceneReq)
