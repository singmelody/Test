#pragma once

#include "BaseType.h"
#include <windows.h>
class MyEvent
{
public:
	MyEvent(void);
	~MyEvent(void);

	void CreateEvent(bool bManualReset, bool bInitState);

	bool SetEvent();
	bool ResetEvent();

	bool Wait(int32 millisecond = -1);
	bool IsValid();

private:
	MyEvent( const MyEvent& );
	MyEvent& operator= (const MyEvent& );

private:
	bool m_bValid;
	HANDLE hEvent;
};

