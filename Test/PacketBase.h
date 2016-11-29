#pragma once

#include "BaseType.h"
#include "PacketSender.h"
#include <assert.h>

#define PACKET_BASE_SIZE					(2*sizeof(int32) + sizeof(uint8))

#define PACKET_EX_SIZE						(sizeof(uint16) + PACKET_BASE_SIZE)

#define PACKET_EX_BUFF_SIZE					512

#define PACKET_EX_BUFF_MAX_SIZE				2048

#define PACKET_MAX_SIZE						(PACKET_EX_SIZE + PACKET_EX_BUFF_MAX_SIZE) * 2

#define PACKET_SEND_LIST_BUFFER_SIZE		512

#define SOCKET_MY_MAX_DATA_BLOCK_SIZE		(PACKET_MAX_SIZE * 2)

#define PACKET_USE_INDEX_DATA				1


enum 
{
	ePacketType_Default			= 0,
	ePacketType_BroadCast		= 1 << 2,
	ePacketType_GateProc		= 1 << 3
};

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

	int32 GetAvatarID() const { return m_AvatarID; }
	void SetAvatarID(int32 nID) { m_AvatarID = nID; }

	int32 GetSocketID() { return m_SocketID; }
	void  SetSocketID(int32 nID) { m_SocketID = nID;} 
	virtual class MyClass* GetClass() { return NULL; }

	const char* GetClassName();

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

	static bool IsClient2Srv(uint32 attr) { return 0 != (attr & PktAttr_IsClt2Srv); }
	static bool ShouldEncrypt(uint32 attr) { return 0 != (attr & PktAttr_ShouldEncrypt); }
	static bool IsEvent(uint32 attr) { return 0 != (attr & PktAttr_IsEvent); }
	static bool IsDisconnectCommand(uint32 attr) { return 0 != ( attr & PktAttr_DisconnectCommand);}

	virtual int32 GetPacketID() const { return m_PacketID; }
	virtual void SetPacketID(int32 nID) { m_PacketID = nID; }

	inline void SetPacketType(int8 type) { m_SendType |= type; }
	inline void UnSetPacketType(int8 type) { m_SendType &= ~type; }
	inline uint8 GetPacketType() { return m_SendType; }
	inline bool CheckPacketType(uint8 nType) { return (m_SendType & nType) > 0; }
	inline void ResetPacketType() { m_SendType = 0; }

	virtual int32 PacketClassSize() { return sizeof(*this); }

#define _BIT32_(X) (1<<X)
	static const uint32 PktAttr_IsClt2Srv		=	_BIT32_(0);
	static const uint32 PktAttr_ShouldEncrypt	=	_BIT32_(1);

	static const uint32 PktAttr_IsEvent			=	_BIT32_(2);
	static const uint32 PktAttr_DisconnectCommand =	_BIT32_(3);

	static const uint32 PktAttr_IsClt2Node		=	_BIT32_(4);
	static const uint32 PktAttr_IsClt2World		=	_BIT32_(5);
	static const uint32 PktAttr_IsClt2Login		=	_BIT32_(6);

	static const uint32 PktAttr_NoNeedLZOSend	=	_BIT32_(7);
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

class PacketSender;
class SyncPacketArg;
class CasterTrunkGate;


class PacketPackBase : public PacketBase
{
public:
	enum
	{
		MaxCnt = 100,
		MaxContentSize = PACKET_MAX_SIZE - PACKET_EX_BUFF_MAX_SIZE - MaxCnt*4,
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
	void SendPacket( PacketBase& pPkt, int32 nSocketID);
	void SendPacket();

	SyncPacketArg*	m_pSyncPacketArg;
	
	void	SetSyncArg( SyncPacketArg* pArg);
	
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

template <int32 nPacketSize = PACKET_EX_BUFF_MAX_SIZE>
class PacketBaseEx : public PacketBase
{
public:
	PacketBaseEx() : m_buffSize(0)
	{ memset( m_buffer, 0, sizeof(m_buffer)); }

	virtual ~PacketBaseEx(){}

	virtual char* ReadPacket( char* pBuff)
	{
		char* tmp = PacketBase::ReadPacket(pBuff);
		m_buffSize = *((uint16*)tmp);
		if( m_buffSize > (uint16)nPacketSize)
		{
			m_buffSize = (uint16)nPacketSize;
			return tmp;
		}

		tmp += sizeof(uint16);
		memcpy( m_buffer, tmp, m_buffSize);
		return tmp + m_buffSize;
	}

	virtual char* WritePacket( char* pBuff)
	{
		char* tmp = PacketBase::WritePacket(pBuff);
		if( m_buffSize > nPacketSize)
			m_buffSize = nPacketSize;

		*((uint16*)tmp) = m_buffSize;
		tmp += sizeof(uint16);
		memcpy( tmp, m_buffer, m_buffSize);
		return tmp + m_buffSize;
	}

	template <class T>
	char* WriteData()
	{
		
	}

	template < class T>
	char* ReadData( char* pBuff, T& val)
	{
		if(!pBuff)
			return 0;

		int32 nValSize = sizeof(T);
		if( (nValSize + m_buffSize) > nPacketSize)
			return 0;

		val = *((T*)pBuff);
		return pBuff + nValSize;
	}

	int32 ReadBuffer( char* pBuffer, size_t nReadSize) { return ReadBuffer( pBuffer, (int32)nReadSize);}
	int32 ReadBuffer( char* pBuffer, int32 nReadSize)
	{
		if(!pBuffer)
			return 0;

		int32 nRSize = m_buffSize;
		if( nReadSize > 0 && nReadSize < m_buffSize)
			nRSize = nReadSize;

		memcpy( pBuffer, m_buffer, nRSize);
		return m_buffSize;
	}

	int32 WriteBuffer(const char* pBuffer, size_t nWriteSize) { return WriteBuffer( pBuffer, (int32)nWriteSize);}
	int32 WriteBuffer( const char* pBuffer, int32 nSize)
	{
		if(!pBuffer || nSize < 0 )
			return 0;

		if( nSize > nPacketSize )
			nSize = nPacketSize;

		memcpy( m_buffer, pBuffer, nSize);
		m_buffSize = nSize;
		return nSize;
	}

	int32 SetPacketSize( uint16 nSize )
	{
		assert( nSize < nPacketSize);
		return m_buffSize = nSize;
	}

	char* GetPacketBuff(uint16 nOffset = 0)
	{
		assert( nOffset < nPacketSize);
		return m_buffer + nOffset;
	}

	uint32 GetMaxBufferSize() { return nPacketSize; }
	uint32 GetCurBufferSize() { return m_buffSize; }
	uint32 GetBufferFreeSize() { return nPacketSize - m_buffSize; }

	static const int32 MAX_PACKET_BUFFER_SIZE = nPacketSize;
protected:
	char		m_buffer[nPacketSize];
	uint16		m_buffSize;
};

template< int32 nPacketSize = PACKET_EX_BUFF_SIZE>
class PacketDataPack : public PacketBaseEx<nPacketSize>
{
public:
	enum
	{
		eMaxCnt = 100,
		eMaxContentSize = PACKET_MAX_SIZE - PACKET_EX_SIZE - eMaxCnt*4,	// 
	};

	PacketDataPack()
	{
		m_pSyncPacketArg = NULL;
		m_nSyncFlag = 0;
		m_nDataCnt = 0;
		Reset();
	}

	void Reset()
	{
		PacketBaseEx<nPacketSize>::SetPacketSize(0);
		m_nDataCnt = 0;
	}

	virtual bool SerialType() { return false; }

	bool FillData(class PacketData* pData)
	{
		// 		int32 nOffset = 0;
		// 		MemoryOStream 

		return true;
	}

	uint16			m_nSyncFlag;
	SyncPacketArg*	m_pSyncPacketArg;
	uint16			m_nDataCnt;
};

class PacketAttachBase : public PacketBase
{
public:
	PacketAttachBase();
	virtual ~PacketAttachBase();

	virtual char* ReadPacket( char* pBuffer );
	virtual char* WritePacket( char* pBuffer );

	void AttachPacket(PacketBase* pPkt);
	PacketBase* GetPacket() { return m_pPacket; }
protected:
	PacketBase*		m_pPacket;
	int32			m_nPacketType;
};

template <typename T>
class PacketBroadCastBase : public PacketAttachBase
{
public:
	PacketBroadCastBase()
	{
		m_nCastCount = 0;
	}

	virtual char* ReadPacket(char* pBuffer)
	{	
		char* tmp = PacketAttachBase::ReadPacket(pBuffer);
		if(!tmp)
			return NULL;

		m_nCastCount = *((uint16*)tmp);
		tmp += sizeof(uint16);

		int32 nSize = m_nCastCount * sizeof(T);
		memcpy( m_CastList, tmp, nSize);
		tmp += nSize;

		return tmp;
	}

	virtual char* WritePacket(char* pBuffer)
	{
		char* tmp = PacketAttachBase::WritePacket(pBuffer);

		*((uint16*)tmp) = m_nCastCount;
		tmp += sizeof(uint16);

		int32 nSize = m_nCastCount * sizeof(T);
		memcpy( tmp, m_CastList, nSize);
		tmp += nSize;

		return tmp;
	}

	void SetCastList( uint16 nCnt, T* pBuffer)
	{
		if(!pBuffer)
			return;

		int32 nSize = nCnt * sizeof(T);
		if( nSize > PACKET_SEND_LIST_BUFFER_SIZE )
			return;

		m_nCastCount = nCnt;
		memcpy( m_CastList, (char*)pBuffer, nSize);
	}

	uint16 GetCastList(T* pBuffer)
	{
		if(!pBuffer)
			return 0;

		int32 nSize = m_nCastCount * sizeof(T);
		if( nSize > PACKET_SEND_LIST_BUFFER_SIZE )
			return 0;

		memcpy( (char*)pBuffer, m_CastList, nSize);
		return m_nCastCount;

	}
protected:
	int32		m_nCastCount;
	char		m_CastList[PACKET_SEND_LIST_BUFFER_SIZE];
};