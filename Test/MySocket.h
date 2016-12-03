#pragma once

#include "NoCopyable.h"
#include "MySockAddr.h"

class SockAddr;

class Socket : private NoCopyable
{
	friend class NetChannelBase;
public:
	Socket();
	explicit Socket(SOCKET sock);
	~Socket();

	void InitSocket();

	bool Create();
	bool Close();
	bool Bind();
	bool Bind(const SockAddr& addr);

	bool Connect(const SockAddr& addr);
	bool Listen(int32 backLog = 128);

	SOCKET Accept();

	int32 Send(const char* buf, int32 nLen, int32 nFlags = 0);
	int32 Recv(char* buff, int32 len, int32 flags = 0);


	bool GetSockAddr(SockAddr& addr) const;
	bool GetPeerAddr(SockAddr& addr) const;

	int32 GetLinger() const;
	bool SetLinger(int32 nLin);

	bool SetNonBlocking(bool on = true);
	bool SetNonDelay(bool bNodelay = true);

	int32 GetRecvBuffSize() const;
	bool SetRecvBuffSize(int32 nLen);

	int32 GetSendBuffSize() const;
	bool SetSendBuffSize(int32 nLen);

	SOCKET GetSocket() const { return m_Sock; }
	void Shift(Socket& sock);

	static int32 GetSysError();
protected:
	int32 GetSockError() const;

	bool IsValid();

	int32 m_Sock;
};