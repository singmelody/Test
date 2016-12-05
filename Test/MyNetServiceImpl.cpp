#include "StdAfx.h"
#include "MyNetServiceImpl.h"
#include "MyNetThread.h"
#include "MySocket.h"
#include "MyLog.h"
#include "Acceptor.h"
#include "NetReactor.h"
#include "MyNetWork.h"

MyNetServiceImpl::MyNetServiceImpl(uint32 nID, const NetServiceParam& param, INetEventListener* pListener)
{
	m_nID = nID;
	m_pListener = pListener;
	m_nState = eNSS_Init;
	m_nNumOfConnection = 0;
	m_Param.Check();

	m_pConnectThread = new MyNetThread(param.nSizeOfConnectThread);
	m_pConnectThread->SetInterval(param.nIntervalOfTick);
}

MyNetServiceImpl::~MyNetServiceImpl(void)
{
	SAFE_DELETE(m_pConnectThread);
}

bool MyNetServiceImpl::BindAddress(const SockAddr& addr)
{
	if(GetState() != eNSS_Running )
		return false;

	Socket socket;
	if(!socket.Create())
	{
		MyLog::message("[%s:%u] BindAddress() socket create error [%d]!", addr.GetIP().c_str(), addr.GetPort(), Socket::GetSysError());
		return false;
	}

	if(!socket.Bind(addr))
	{
		MyLog::message("[%s:%u] BindAddress() socket bind error [%d]!", addr.GetIP().c_str(), addr.GetPort(), Socket::GetSysError());
		socket.Close();
		return false;
	}

	if(!socket.Listen(m_Param.nBackLog))
	{
		MyLog::message("[%s:%u] BindAddress() socket Listen error [%d]!", addr.GetIP().c_str(), addr.GetPort(), Socket::GetSysError());
		socket.Close();
		return false;
	}

	if(!socket.SetNonBlocking())
	{
		MyLog::message("[%s:%u] BindAddress() socket SetNonBlocking error [%d]!", addr.GetIP().c_str(), addr.GetPort(), Socket::GetSysError());
		socket.Close();
		return false;
	}

	m_pConnectThread->RegisterHandler( new Acceptor( socket, addr, this), eNEM_EXCEPTION | eNEM_IN);
	return true;
}

NetConnectionPtr MyNetServiceImpl::Connect(uint32 nID, const SockAddr& addr)
{
	if(GetState() != eNSS_Running )
		return NULL;

	Socket socket;
	if(!socket.Create())
	{
		MyLog::message("[%s:%u] connection create error[%d]", addr.GetIP().c_str(), addr.GetPort(), Socket::GetSysError());
		return NULL;
	}

	if(!socket.Connect(addr))
	{
		MyLog::message("[%s:%u] connection create error[%d]", addr.GetIP().c_str(), addr.GetPort(), Socket::GetSysError());
		socket.Close();
		return NULL;
	}

	if(!InitSocket(socket))
	{
		socket.Close();
		return NULL;
	}
	 
	INetConnection* pConnection = m_pListener->OnCreateConnection();
	pConnection->IncRef();

	SOCKET nSocketID = socket.GetSocket();
	pConnection->Open( socket, CT_ACTIVE, this);
	MyLog::message("[%s:%u] connection opened! Socket:%u", addr.GetIP().c_str(), addr.GetPort(), nSocketID);

	if(!AddVerifiedConnection( nID, pConnection))
	{
		MyLog::error("repeat connection [%s:%u]! NodeID:%u", addr.GetIP().c_str(), addr.GetPort(), nID);
		pConnection->Close();
		pConnection->DecRef();
		return NULL;
	}

	NetConnectionPtr autoPtr = pConnection;
	ScheduleConnection(pConnection);

	return autoPtr;
}

void MyNetServiceImpl::OnNewConnection(Socket& socket, ConnectionType type, uint32 nID /*= 0*/)
{
	SockAddr addr;
	socket.GetPeerAddr(addr);

	if(GetState() != eNSS_Running )
	{
		MyLog::message("[%s:%u] OnNewConnection error : not running state !", addr.GetIP().c_str(), addr.GetPort());
		socket.Close();
		return;
	}

	if(!InitSocket(socket))
	{
		MyLog::message("[%s:%u] OnNewConnection error : initSocket failed!", addr.GetIP().c_str(), addr.GetPort());
		socket.Close();
		return;
	}

	m_mutex.Lock();
	if( m_nNumOfConnection >= m_Param.nMaxNumOfConn )
	{
		m_mutex.Unlock();
		MyLog::message("[%s:%u] OnNewConnection too many connection!", addr.GetIP().c_str(), addr.GetPort());
		socket.Close();
		return;
	}

	INetConnection* pConnection = m_pListener->OnCreateConnection();
	pConnection->IncRef();

	SOCKET socketid = socket.GetSocket();
	pConnection->Open( socket, type, this);
	MyLog::message("[%s:%u] connection opened! Socket:%u", addr.GetIP().c_str(), addr.GetPort(), socketid);

	ScheduleConnection(pConnection);
}

bool MyNetServiceImpl::InitSocket(Socket& socket)
{
	if(!socket.SetNonBlocking())
	{
		MyLog::message("socket [%u] setnonblock error [%d]", socket.GetSocket(), Socket::GetSysError());
		return false;
	}

	socket.SetRecvBuffSize(m_Param.nSizeOfSysBuffer);
	socket.SetSendBuffSize(m_Param.nSizeOfSysBuffer);

	return true;
}

void MyNetServiceImpl::ScheduleConnection(INetConnection* pConn)
{
	MyNetThread* pThread = (MyNetThread*)GetMinLoadThread();
	if(!pThread)
		return;

	pThread->RegisterHandler( pConn, eNEM_EXCEPTION | eNEM_IN | eNEM_TICK);
}
