#include "stdafx.h"
#include "MyLog.h"
#include "MyNetServiceImpl.h"

INetService* CreateNetService(uint32 nID, const NetServiceParam& param, INetEventListener* pListener)
{
	if(!pListener)
		return NULL;

	class NetStartupHelper
	{
	public:
		NetStartupHelper()
		{
			WSADATA wsaData;
			WORD wVersionRequested = MAKEWORD( 2, 2);

			WSAStartup( wVersionRequested, &wsaData);
		}

		~NetStartupHelper()
		{
			WSACleanup();
		}
	};

	static NetStartupHelper helper;

	MyLog::message("NetServiec [%u] created !", nID);
	return new MyNetServiceImpl( nID, param, pListener);
}

void DestroyNetService(INetService* pService)
{
	if(!pService)
		return;

	MyLog::message("NetService [%u] destroyed!", pService->GetID());
	SAFE_DELETE(pService);
}