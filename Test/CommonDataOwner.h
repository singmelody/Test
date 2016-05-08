#pragma once

#include "Avatar.h"
#include "CommonDataCont.h"
#include "PacketImpl.h"

class CommonDataObject;
class CommonDataManager;
class PacketCommonDataBase;

class CommonDataOwner : public virtual Avatar
{
public:
	CommonDataOwner(void);
	virtual ~CommonDataOwner(void);

	virtual CommonDataManager* GetCommonDataManager() { return NULL; }

	CommonDataCont* GetCommonDataCont( CommonDataType type);

	CommonDataObject* CreateCommonDataObj( PacketCommonDataBase* pPkt);
	CommonDataObject* CreateCommonDataObj( CommonDataType type, int32 nDataID = 0, int32 index = -1);

	CommonDataObject* GetCommonDataObj( PacketCommonDataBase* pPkt);
	CommonDataObject* GetCommonDataObj( CommonDataType type, int32 index = -1);

	void DeleteCommonDataObj( CommonDataObject* pObj, bool bDestroy = true);

	CommonDataObject* OnRecvUpdatePacket( PacketCommonDataUpdate* pPkt);

	CommonDataObject* OnRecvCreatePacket( PacketCommonDataCreate* pPkt);
	CommonDataObject* OnRecvInitPacket( PacketCommonDataInit* pPkt);

	void OnRecvDeletePacket( PacketCommonDataDelete* pPkt);

	virtual bool UseShareMemory() { return true; }
};

