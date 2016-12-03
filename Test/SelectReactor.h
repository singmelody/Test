#pragma once

#include <WinSock2.h>
#include "NetReactor.h"
#include <map>

class NetEventHandler;

class SelectReactor : public NetReactor
{
	typedef std::map< SOCKET, NetEventHandler*>	HANDLER_MAP;
public:
	SelectReactor(void);
	virtual ~SelectReactor(void);

	virtual bool Open( uint32 nMaxNumOfHandler, uint32 tickInter = 1000);
	virtual void Close();

	virtual bool RegisterHandler(NetEventHandler* pEvent, int32 events = eNEM_ALL);
	virtual void RemoveHandler(NetEventHandler* pEvent, int32 events = eNEM_ALL);

	virtual bool HandleEvents(const Time& now, int32 nWaitMs);
	virtual int32 GetNumOfHandler() const;

protected:
	fd_set m_InSet;
	fd_set m_OutSet;

	fd_set m_ExceptSet;

	HANDLER_MAP	m_handlers;

	uint32	m_tickInter;
	uint32	m_nMaxOfHandler;
	bool	m_bOpened;
	bool	m_bRemoveFlag;
};

