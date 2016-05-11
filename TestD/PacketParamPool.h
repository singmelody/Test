#pragma once

#include "PacketImpl.h"
#include "PacketBase.h"

PACKET(PacketParamPoolBase, PacketBaseEx<PACKET_EX_BUFF_SIZE>)
uint32	m_ParamType;
uint8	m_ParamFlag;
START_ADD_PACKET_MEMBER(PacketParamPoolBase)
	ADD_PACKET_MEMBER( m_ParamType, uint32, m_ParamType);
ADD_PACKET_MEMBER( m_ParamFlag, uint8, m_ParamFlag);
PACKET_END(PacketParamPoolBase)


class PacketParamPool : public PacketParamPoolBase
{
public:
	PacketParamPool(void);
	~PacketParamPool(void);
};

