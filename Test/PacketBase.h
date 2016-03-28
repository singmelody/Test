#pragma once

#define PACKET_BASE_SIZE					(2*sizeof(int32) + sizeof(uint8))

#define PACKET_EX_SIZE						(sizeof(uint16) + PACKET_BASE_SIZE)

#define PACKET_EX_BUFF_SIZE					512

#define PACKET_EX_BUFFER_MAX_SIZE			2048

#define PACKET_MAX_SIZE						(PACKET_EX_SIZE + PACKET_EX_BUFFER_MAX_SIZE) * 2

#define PACKET_SEND_LIST_BUFFER_SIZE		512

#define SOCKET_MY_MAX_DATA_BLOCK_SIZE		(PACKET_MAX_SIZE * 2)


class PacketPack;

class PacketBase
{
	friend class PacketList;
	friend class PacketListEx;

public:
	PacketBase()
		: m_AvatarID(-1)
		, m_SocketID(-1)
		, m_SendType(0)
		, _next(NULL)
		, _prev(NULL)
		, m_PacketID(-1)
		, m_bUseIdx(false)
	{

	}

#define _BIT32_(X) (1<<X)
	static const uint32 PktAttr_IsClient2Srv	=	_BIT32_(0);
	static const uint32 PktAttr_ShouldEncrypt	= 	_BIT32_(1);
#undef  _BIT32_

protected:
	int32		m_PacketID;
	int32		m_AvatarID;
	int32		m_SocketID;
	int32		m_SendType;
	PacketBase* _next;
	PacketBase* _prev;
	bool		m_bUseIdx;
	uint16		m_nIdx;
};