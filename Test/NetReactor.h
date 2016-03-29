#pragma once
#include "Time.h"

enum NetEventMask
{
	eNEM_IN			= 1 << 0,	// read event
	eNEM_OUT		= 1 << 1,	// write event
	eNEM_EXCEPTION	= 1 << 2,	// exception
	eNEM_IO			= eNEM_IN | eNEM_OUT | eNEM_EXCEPTION,
	eNEM_TICK		= 1 << 3,	// tick event
	eNEM_ALL		= eNEM_IO | eNEM_TICK
};

class NetEventHandler;

class NetReactor
{
public:
	NetReactor(void);
	virtual ~NetReactor(void);

	virtual bool Open( uint32 nMaxNumOfHandler, uint32 tickInter = 1000) = 0;
	virtual void Close() = 0;

	virtual bool Register(NetEventHandler* pEvent, int32 events = eNEM_ALL) = 0;
	virtual void RemoveHandler(NetEventHandler* pEvent, int32 events = eNEM_ALL) = 0;

	virtual bool HandleEvent(const Time& now) = 0;
	virtual int32 GetNumOfHandler() const = 0

};

