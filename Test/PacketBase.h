#pragma once

#include "BaseType.h"

#define PACKET_BASE_SIZE					(2*sizeof(int32) + sizeof(uint8))

#define PACKET_EX_SIZE						(sizeof(uint16) + PACKET_BASE_SIZE)

#define PACKET_EX_BUFF_SIZE					512

#define PACKET_EX_BUFFER_MAX_SIZE			2048

#define PACKET_MAX_SIZE						(PACKET_EX_SIZE + PACKET_EX_BUFFER_MAX_SIZE) * 2

#define PACKET_SEND_LIST_BUFFER_SIZE		512

#define SOCKET_MY_MAX_DATA_BLOCK_SIZE		(PACKET_MAX_SIZE * 2)

#define PACKET_USE_INDEX_DATA				1


class PacketPack;
class MyClass;

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

	static const uint32 PktAttr_IsEvent			=	_BIT32_(2);
	static const uint32 PktAttr_Disconnect		=	_BIT32_(3);

	static const uint32 PktAttr_IsClient2Node	=	_BIT32_(4);
	static const uint32 PktAttr_IsClient2World	=   _BIT32_(5);
	static const uint32 PktAttr_IsClient2Login	=	_BIT32_(6);

	static const uint32	PktAttr_NoNeedLZOSend	=	_BIT32_(7);
#undef  _BIT32_

	virtual char* ReadPacket(char* buffer);
	virtual char* WritePacket(char* buffer);

#if PACKET_USE_INDEX_DATA
	inline void IsUseIndex(bool v) { m_bUseIdx = v; }
	inline bool IsUseIndex() const { return m_bUseIdx; }
	inline void SetPacketIndex(uint16 n) { m_nIdx = n; }
	inline uint16 GetPacketIndex() { return m_nIdx; }
#endif

	virtual bool IsClient2Srv() { return false; }

	virtual uint32 GetStaticAttribute() const { return PktAttr_ShouldEncrypt; }

	static bool IsClient2Srv(uint32 attr) { return 0 != (attr & PktAttr_IsClient2Srv); }
	static bool ShouldEncrypt(uint32 attr) { return 0 != (attr & PktAttr_ShouldEncrypt); }
	static bool IsEvent(uint32 attr) { return 0 != (attr & PktAttr_IsEvent); }
	static bool IsDisconnectCommand(uint32 attr) { return 0 != ( attr & PktAttr_Disconnect);}

	virtual int32 GetPacketID() const { return m_PacketID; }
	virtual void SetPacketID(int32 nID) { m_PacketID = nID; }

	virtual MyClass* GetClass() { return NULL; }
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

class PacketSender;
class SyncPackArg;
class CasterTrunkGate;

class PacketPackBase : public PacketBase
{
public:
	enum
	{
		MaxCnt = 100,
		MaxContentSize = PACKET_MAX_SIZE - PACKET_EX_BUFFER_MAX_SIZE - MaxCnt*4,
	};

	PacketPackBase();
	virtual ~PacketPackBase();

	virtual char* ReadPacket(char* buffer);
	virtual char* WritePacket(char* buffer);

	bool AddupPacket(PacketBase* pPkt);
	void Reset();

	bool IsEmpty() { return m_PacketCnt == 0; }

	int8 GetPacketCnt() { return m_PacketCnt; }

	PacketBase* GetPacket(int8 index)
	{
		if( index < 0 || index >= m_PacketCnt)
			return NULL;

		return m_pPacket[index];
	}

	bool FillPacket( PacketBase* pPkt);
	void SendPacket();


	SyncPackArg*	m_pSyncPacketArg;
	
	void	SetSyncArg( SyncPackArg* pArg);
	
	void	SetDBASyncArg( PacketSender* pSender);
	void	SetWorldSyncArg( PacketSender* pSender);	
	void	SetGateSyncArg( PacketSender* pSender, CasterTrunkGate* pTrunk);	
	void	SetNodeSyncArg( PacketSender* pSender, int32 nNodeSrvID);
	void	SetCltSyncArg( PacketSender* pSender);

protected:
	int8			m_PacketCnt;
	PacketBase*		m_pPacket[MaxCnt];

	int32			m_ContentSize;	// 合并子包大小
};