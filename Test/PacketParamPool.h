#pragma once

#include "PacketBase.h"
#include "MyPacket.h"
#include "PacketSender.h"

PACKET(PacketParamPoolBase, PacketBaseEx<PACKET_EX_BUFF_SIZE>)
uint32	m_nParamType;
uint8	m_nParamFlag;
START_ADD_PACKET_MEMBER(PacketParamPoolBase)
	ADD_PACKET_MEMBER( m_nParamType, uint32, m_ParamType);
	ADD_PACKET_MEMBER( m_nParamFlag, uint8, m_ParamFlag);
PACKET_END(PacketParamPoolBase)

class SyncPacketArg;
class ParamPool;
class ParamPoolOwner;
class PacketSender;

class PacketParamPool : public PacketParamPoolBase
{
public:
	enum{
		ePacketFirst	= 1 << 0,
		ePacketLast		= 1 << 1,
		ePacketFull		= 1 << 2,
	};

	PacketParamPool(void);
	~PacketParamPool(void);

	inline bool IsFirstPacket() { return CheckFlag(ePacketFirst); }
	inline bool IsLastPacket() { return CheckFlag(ePacketLast); }
	inline bool IsFullPacket() { return CheckFlag(ePacketFull); }

	bool CheckFlag(int8 bit) { return (m_nParamFlag & bit) == bit; }

	void SyncParamPool( SyncPacketArg& arg, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude = 0);
	void SyncParamPool( PacketPack* pPack, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude = 0);

	void SyncParamPool( PacketSender* pPack, SENDPARAMFUNC1 pFunc, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude = 0);
	void SyncParamPool( PacketSender* pPack, SENDPARAMFUNC2 pFunc, int32 nParam, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude = 0);
	void SyncParamPool( PacketSender* pPack, SENDPARAMFUNC3 pFunc, bool bParam, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude = 0);


	void SyncParam2Gate(class PacketSender* pSender, bool bGateProc, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude = 0);
	void SyncParam2Dog(class PacketSender* pSender, bool bGateProc, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude = 0);


	bool UpdateParamPool( ParamPoolOwner* pPool, bool bDirty = false);
	bool UpdateParamPool( ParamPool* pPool, bool bDirty = false);
};

