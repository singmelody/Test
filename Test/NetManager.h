#pragma once

class NetChannelBase;

class NetManager
{
public:
	NetManager(void);
	~NetManager(void);

	void NetRun();

	void Accept( char* sAddr, int32 nPort, bool bSrv);
	int32 Connect( char* sAddr, int32 nPort, bool bSingleThread = false);

protected:
	NetChannelBase* CreateNewChannel();

	void ProcNewConnection();
	void ProcPendingClose();
	void ProcPendingDestroy();
};

