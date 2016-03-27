#include "StdAfx.h"
#include "MYEvent.h"


MYEvent::MYEvent(void) : m_bValid(false)
{
}

MYEvent::~MYEvent(void)
{
	if(!m_bValid)
		return;

	::CloseHandle(hEvent);
}

bool MYEvent::SendEvent()
{
	if(!m_bValid)
	{
		MyLog::warning("MYEvent::SendEvent() not valid");
		return false;
	}

	return 
}


void MYEvent::CreateEvent(bool bManualReset, bool bInitState)
{
	assert(!m_bValid);

	hEvent = ::CreateEvent( NULL, bManualReset, bInitState, NULL);
	m_bValid = (hEvent != INVALID_HANDLE_VALUE);
}