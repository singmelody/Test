#pragma once

#include "BaseType.h"
#include <windows.h>
class MYEvent
{
public:
	MYEvent(void);
	~MYEvent(void);

	void CreateEvent(bool bManualReset, bool bInitState);

	bool SetEvent();
	bool ResetEvent();

	bool Wait(int32 millisecond = -1);
	bool IsValid();

private:
	MYEvent( const MYEvent& );
	MYEvent& operator= (const MYEvent& );

private:
	bool m_bValid;
	HANDLE hEvent;
};

