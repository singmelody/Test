#include "StdAfx.h"
#include "MYEvent.h"
#include "MyLog.h"
#include <assert.h>

MyEvent::MyEvent(void) : m_bValid(false)
{
}

MyEvent::~MyEvent(void)
{
	if(!m_bValid)
		return;

	::CloseHandle(hEvent);
}

bool MyEvent::SetEvent()
{
	if(!m_bValid)
	{
		MyLog::warning("MYEvent::SendEvent() not valid");
		return false;
	}

	return TRUE == ::SetEvent(hEvent);
}


bool MyEvent::ResetEvent()
{
	if(!m_bValid)
	{
		MyLog::warning(" MYEvent::ResetEvent not valid");
		return false;
	}

	return true == (bool)::ResetEvent(hEvent);
}

bool MyEvent::Wait(int32 nWaitTimeInMillSeconds /*= -1*/)
{
	if(!m_bValid)
	{
		MyLog::warning("MyEvent::Wait (%d) not valid", nWaitTimeInMillSeconds);
		return true;
	}

#ifdef _WINDOWS
	DWORD nRet = WaitForSingleObject( hEvent, nWaitTimeInMillSeconds >= 0 ? nWaitTimeInMillSeconds : INFINITE);
	return nRet == WAIT_OBJECT_0;
#endif
}

bool MyEvent::IsValid()
{
	return m_bValid;
}

void MyEvent::CreateEvent(bool bManualReset, bool bInitState)
{
	assert(!m_bValid);

	hEvent = ::CreateEvent( NULL, bManualReset, bInitState, NULL);
	m_bValid = (hEvent != INVALID_HANDLE_VALUE);
}