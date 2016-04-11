#pragma once

#include "Singleton.h"

class ServerManager : public Singleton<ServerManager>
{
public:
	ServerManager(void);
	~ServerManager(void);

	static int32 GetZoneID(int32 nSrvID);
	static int32 GetGroupID(int32 nSrvID);
	static int32 GetSrvType(int32 nSrvID);
	static int32 GetSrvIndex(int32 nSrvID);

	static int32 MakeSrvID( int32 nZoneID, int32 nGrpID, int32 nSrvType, int32 nIdx);


};

