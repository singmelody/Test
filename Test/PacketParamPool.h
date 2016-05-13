#pragma once

#include "PacketBase.h"
#include "MyPacket.h"

PACKET(PacketParamPoolBase, PacketBaseEx<PACKET_EX_BUFF_SIZE>)
uint32	m_ParamType;
uint8	m_ParamFlag;
START_ADD_PACKET_MEMBER(PacketParamPoolBase)
	ADD_PACKET_MEMBER( m_ParamType, uint32, m_ParamType);
	ADD_PACKET_MEMBER( m_ParamFlag, uint8, m_ParamFlag);
PACKET_END(PacketParamPoolBase)

class SyncPacketArg;
class ParamPool;
class ParamPoolOwner;

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

	bool CheckFlag(int8 flag) { }

	void SyncParamPool( SyncPacketArg& arg, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude);

	bool UpdateParamPool( ParamPoolOwner* pPool, bool bDirty = false);
	bool UpdateParamPool( ParamPool* pPool, bool bDirty = false);
};

