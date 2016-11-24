#pragma once
#include "NetEventHandler.h"
#include "MySocket.h"
#include "MYEvent.h"
#include "PacketList.h"
#include <WinSock2.h>
#include "PacketBase.h"
#include "CircularBufferEx.h"

typedef uint16 BlockHeadT;

class NetEventHandler;
class PacketBase;

class NetChannelBase : public NetEventHandler
{
	friend class NetManager;
	friend class NetManagerReactor;
public:
	NetChannelBase(void);
	~NetChannelBase(void);

	virtual void AppendPacket( PacketBase* pPkt);
	virtual bool InitChannel( NetManager* pMgr, int32 nSockRcBuffSize, int32 nStreamRcSize, int32 nSockSnBuffSize, int32 nStreamSnSize,Socket* pSocket = 0);

	int32 GetID() const { return m_ID;}
	void SetID(int32 nID) { m_ID = nID; }


	virtual bool OnWritePackets2Stream();
	virtual bool OnWritePacket2Stream(PacketBase& pkt) { return false; }


	virtual bool OnParsePacketsFromStream() = 0;

	virtual bool FlushStream();
	bool Fetch2Stream();

	inline bool GetAddr(SockAddr& addr) { return m_socket.GetSockAddr(addr); }
	inline bool GetRemoteAddr(SockAddr& addr) { return m_socket.GetPeerAddr(addr); }
protected:
	void AppendData2Stream(char* pBuff, int32 nLen);

	int32 RecvData( char* buffer, int32 nLen);

	virtual void HandleInput();
	virtual void HandleOutput();
	virtual void HandleException();
	virtual void HandleClose();

	enum
	{
		CBBUFF = SOCKET_MY_MAX_DATA_BLOCK_SIZE * 4
	};

	virtual bool StartChannel();

	int32 m_RefCount;

	volatile bool m_bIsClosing;

	bool Listen(char* sAddr, int32 nPort);
	bool Connect( char* sAddr, int32 nPort);

	virtual void DisConnect();
	virtual void CloseChannel();

	void SetSocket(SOCKET socket);
	SOCKET GetSocket() const { return m_socket.GetSocket(); }


	int32		m_state;
	int32		m_sockRcBuffSize;
	int32		m_sockRnBuffSize;

	Socket		m_socket;
	NetManager* m_pMgr;

	PacketList	m_queueSendingPacket;	// 私有数据包队列

	CircularBufferEx		m_StreamIn;
	CircularBufferEx		m_StreamOut;

	class LZOCompressor*	m_pLZOCompressor;

	int32		m_ID;
	uint32		m_PacketIdx;
	uint32		m_nTotalSendByte;
	uint32		m_nTotalRecvByte;

	char		m_sendPacketBuffer[CBBUFF];
	char		m_recvPacketBuffer[CBBUFF];

	bool		m_bCreateByAccept;
	MYEvent		m_hProcNewNotify;
};

