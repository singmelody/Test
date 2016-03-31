#pragma once
#include <string>
#include <WinSock2.h>
#include "BaseType.h"

typedef int32 socklen_t;

#pragma comment( lib, "ws2_32")

#define MY_EINTR			WSAEINTR
#define MY_EWOULDBLOCK		WSAEWOULDBLOCK
#define MY_ENOBUFS			WSAENOBUFS
#define MY_ETIMEOUT			WSAETIMEDOUT
#define MY_ECONNRESET		WSAECONNRESET
#define MY_EAGAIN			-1

class SockAddr
{
	
public:
	SockAddr(void);
	SockAddr(const std::string& ip, uint16 port) : m_IP(ip)
	{
		m_port =  port;
	}

	~SockAddr(void) {}


	uint16 GetPort() { return m_port; }
	void SetPort(uint16 port) { m_port = port; }

	const std::string& GetIP()
	{
		return m_IP;
	}

	void SetIP(const std::string& ip)
	{
		m_IP = ip;
	}

	void IPA2N(const std::string& strIp, uint32& nIp)
	{
		nIp = inet_addr(strIp.c_str());
	}

	const SockAddr& operator=(const SockAddr& addr)
	{
		m_IP = addr.m_IP;
		m_port = addr.m_port;

		return *this;
	}

	void GetAddr(const sockaddr_in& saddr);
	void GetAddr(sockaddr_in& saddr) const;

	void SetAddr(const sockaddr_in& saddr);

protected:
	std::string m_IP;
	int32 m_port;
	uint16 port;
};

