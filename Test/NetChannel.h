#pragma once

#include "NetChannelBase.h"
#include "MYEvent.h"
#include "MyMutex.h"

#define LZO_COMPRESS 1
#define USE_MY_BLOWFISH 0

class PacketBase;
class LZOCompressor;
class BlowFishCipher;

class NetChannel : public NetChannelBase
{
	friend class NetManager;
public:
	NetChannel(void);
	virtual ~NetChannel(void);

	virtual bool InitChannel( NetManager* pMgr, int32 nSockRcBuffSize, int32 nStreamRcSize, int32 nSockSnBuffSize, int32 nStreamSnSize,Socket* pSocket = 0);
	virtual bool StartChannel();

	static void CALLBACK AsynIoCompleteProc( DWORD dwErrCode, DWORD dwTransed, LPOVERLAPPED param);

	virtual void CloseChannel();
	virtual void AppendPacket(PacketBase* pPkt);
	virtual void OnPacketParsed(PacketBase* pPkt);

	void AsynSend( PBYTE pData, long lDataSize);
	bool AsynRecv(PBYTE pData, long lDataSize);

	void TryStartSending();
	bool TryAsynSendPackets();

	bool OnParsePacketsFromStream();

	virtual void OnExitSending();
	virtual void OnExitReceiving();
protected:
	class DataBufferArg
	{
	public:
		DataBufferArg()
		{
			cbBuffer = 0;
			pBuffer = NULL;
			cbData = 0;
		}

		uint32	cbBuffer;
		char*	pBuffer;
		uint32	cbData;
	};

	bool FillPackets2Block( DataBufferArg& arg);
	bool FetchBlockFromStreamAndProcess();
	bool ParsePacketsInBlock( BlockHeadT blockHead, char* pBuffer, uint32 nDataLen);

	bool StartNewRecv(long lDataSize);

	virtual bool OnAsynSendComplete(DWORD dwTransed);
	virtual bool OnAsynRecvComplete(DWORD dwTransed);

	static const BlockHeadT BLOCK_SIZE_MASK = 0x3fff;
	static const BlockHeadT BLOCK_FLAG_LZO = 0x8000;
	static const BlockHeadT Block_FLAG_ENC = 0x4000;

	bool		m_bHasStartSending;
	Mutex		m_MutexSending;

	char		m_SendPacketBuffer2[CBBUFF];
	char		m_RecvPacketBuffer2[CBBUFF];

	uint16		m_nIndexOfSend;
	uint16		m_nIndexOfRecv;

	BlockHeadT	m_BlockHead;
	PBYTE		m_pRecvBuffer;
	PBYTE		m_pSendBuffer;

	DWORD		m_cbData2Recv;
	DWORD		m_cbDataRecved;

	DWORD		m_cbData2Send;
	DWORD		m_cbDataSended;

	MYEvent		m_hExitSending;
	MYEvent		m_hExitReceiving;

	struct MYOVERLAPPED
	{
		OVERLAPPED	overlapped;

		NetChannel* pChannel;
		bool		bSend;
	};

	MYOVERLAPPED	m_OLPRecv;
	MYOVERLAPPED	m_OLPSend;

#if LZO_COMPRESS
	LZOCompressor*		m_pGLZOCompressor;
#endif

#if USE_MY_BLOWFISH
	BlowFishCipher*		m_pBlowFishCipher;
#endif
};

