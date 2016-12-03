#include "StdAfx.h"
#include "SelectReactor.h"
#include "NetEventHandler.h"
#include "MySocket.h"
SelectReactor::SelectReactor(void)
{
	m_nMaxOfHandler = 0;
	m_bOpened = false;
	m_bRemoveFlag = false;
}


SelectReactor::~SelectReactor(void)
{

}

bool SelectReactor::Open(uint32 nMaxNumOfHandler, uint32 tickInter /*= 1000*/)
{
	if(m_bOpened)
		Close();

	FD_ZERO(&m_InSet);
	FD_ZERO(&m_OutSet);
	FD_ZERO(&m_ExceptSet);

	if( nMaxNumOfHandler > FD_SETSIZE )
		nMaxNumOfHandler = FD_SETSIZE;

	m_nMaxOfHandler = nMaxNumOfHandler;
	m_tickInter = tickInter;
	m_bOpened = true;
	return true;
}

void SelectReactor::Close()
{
	if(!m_bOpened)
		return;

	m_bOpened = false;
	m_bRemoveFlag = false;

	m_nMaxOfHandler = 0;

	for (auto itr = m_handlers.begin(); itr != m_handlers.end(); ++itr)
	{
		NetEventHandler* pHandler = itr->second;
		if(!pHandler)
			continue;

		pHandler->HandleClose();
	}

	m_handlers.clear();
}

bool SelectReactor::RegisterHandler(NetEventHandler* pHandler, int32 events /*= eNEM_ALL*/)
{
	events &= eNEM_ALL;

	if( !m_bOpened || NULL == pHandler || 0 == events)
		return false;

	SOCKET socket = pHandler->GetSocket();
	if( socket < 0 )
		return false;

	HANDLER_MAP::iterator itr = m_handlers.find(socket);
	if( itr == m_handlers.end() )
	{
		if(m_handlers.size() >= m_nMaxOfHandler )
			return false;

		pHandler->SetEventMask(0);
		pHandler->SetTimeout(0);
		pHandler->SetReactor(this);

		m_handlers.insert(std::make_pair( socket, pHandler));
	}

	uint32 nOldEvents = pHandler->GetEventMask();
	events &= ~nOldEvents;
	pHandler->SetEventMask( nOldEvents |= events );

	if( events & eNEM_IN )
		FD_SET( socket, &m_InSet);

	if( events & eNEM_OUT )
		FD_SET( socket, &m_OutSet);

	if( events & eNEM_EXCEPTION )
		FD_SET( socket, &m_ExceptSet);

	return true;
}

void SelectReactor::RemoveHandler(NetEventHandler* pHandler, int32 events /*= eNEM_ALL*/)
{
	events &= eNEM_ALL;

	if(!m_bOpened || !pHandler || 0 == events)
		return;

	SOCKET socket = pHandler->GetSocket();
	if( socket < 0)
		return;

	HANDLER_MAP::iterator itr = m_handlers.find(socket);
	if( itr == m_handlers.end() )
		return;

	uint32 nOldEvents = pHandler->GetEventMask();
	events &= nOldEvents;

	if( events & eNEM_IN )
		FD_CLR( socket, &m_InSet);

	if( events & eNEM_OUT )
		FD_CLR( socket, &m_OutSet);

	if( events & eNEM_EXCEPTION )
		FD_CLR( socket, &m_ExceptSet);

	pHandler->SetEventMask( nOldEvents & ~events );

	if( pHandler->GetEventMask() == 0 )	// remove all event
	{
		m_bRemoveFlag = true;
		m_handlers.erase(itr);
		pHandler->HandleClose();
	}
}

bool SelectReactor::HandleEvents(const Time& now, int32 nWaitMs)
{
	if(!m_bOpened || 0 == m_handlers.size() )
		return false;

	int32 nMaxFd = 1;

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = nWaitMs * 1000;

	fd_set inset = m_InSet;
	fd_set outset = m_OutSet;
	fd_set exceptset = m_ExceptSet;

	int32 nRet = 0;
	while (true)
	{
		nRet = select( nMaxFd, &inset, &outset, &exceptset, nWaitMs >= 0 ? &timeout : 0);

		if( nRet < 0 )
		{
			nRet = Socket::GetSysError();
			if(nRet == MY_EINTR )
				continue;
			return false;
		}
		break;
	}

	bool bEvent = false;
	for (HANDLER_MAP::iterator nextItr = m_handlers.begin(), itr; nextItr != m_handlers.end(); )
	{
		itr = nextItr++;

		NetEventHandler* pHandler = itr->second;
		SOCKET socket = itr->first;
		m_bRemoveFlag = false;

		if(!m_bRemoveFlag && (pHandler->GetEventMask() & eNEM_IN) && FD_ISSET( socket, &inset))
		{
			pHandler->HandleInput();
			bEvent = true;
		}

		if(!m_bRemoveFlag && (pHandler->GetEventMask() & eNEM_OUT) && FD_ISSET( socket, &outset))
		{
			pHandler->HandleOutput();
			bEvent = true;
		}

		if(!m_bRemoveFlag && (pHandler->GetEventMask() & eNEM_EXCEPTION) && FD_ISSET( socket, &exceptset))
		{
			pHandler->HandleException();
			bEvent = true;
		}

		if(!m_bRemoveFlag && (pHandler->GetEventMask() & eNEM_TICK ))
		{
			if( now.MilliSecond() > pHandler->GetTimeout() )
			{
				pHandler->SetTimeout( now.MilliSecond() + m_tickInter );
				pHandler->HandleTick(now);
			}
		}
	}

	return bEvent;
}

int32 SelectReactor::GetNumOfHandler() const
{
	return (int32)m_handlers.size();
}

