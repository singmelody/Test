#include "StdAfx.h"
#include "MySockAddr.h"


SockAddr::SockAddr(void)
{
	m_port = 0;
}

void SockAddr::GetAddr(const sockaddr_in& saddr)
{
	char* pStr = inet_ntoa(saddr.sin_addr);
	if(pStr)
		m_IP = pStr;

	m_port = ntohs(saddr.sin_port);
}

void SockAddr::GetAddr(sockaddr_in& addr) const
{
	memset( &addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(m_IP.c_str());
	addr.sin_port = htons(m_port);
}

void SockAddr::SetAddr(const sockaddr_in& saddr)
{
	char* pStr = inet_ntoa(saddr.sin_addr);
	if(pStr)
		m_IP = pStr;

	m_port = ntohs(saddr.sin_port);
}
