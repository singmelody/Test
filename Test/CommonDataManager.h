#pragma once

#include "CommonDataCont.h"

class CommonDataOwner;

class CommonDataManager
{
public:
	CommonDataManager();
	virtual ~CommonDataManager();

	virtual void SetOwner(CommonDataOwner* pOwner);

	void	AddCont(CommonDataCont* pCont);
	void	ClearCont(CommonDataType type);

	virtual void NotifyCreate( CommonDataObject* pObj, int32 nFlag){}
	virtual void NotifyDelete( CommonDataObject* pObj){}

	CommonDataCont* GetCont(CommonDataType type) const;

	CommonDataObject* CreateCommonDataObj( class PacketCommonDataBase* pPkt);
	CommonDataObject* CreateCommonDataObj( CommonDataType type ,int32 nDataID, int32 nIdx);

	CommonDataObject* GetCommonDataObj( class PacketCommonDataBase* pPkt);
	CommonDataObject* GetCommonDataObj( CommonDataType type ,int32 nIdx);

	virtual void DeleteCommonDataObj( CommonDataObject* pObj, bool bDestroy = true);
	
	virtual CommonDataObject* OnRecvInitPacket(class PacketCommonDataInit* pPkt);
	virtual CommonDataObject* OnRecvCreatePacket(class PacketCommonDataCreate* pPkt);
	virtual CommonDataObject* OnRecvUpdatePacket(class PacketCommonDataUpdate* pPkt);
	virtual void OnRecvDeletePacket(class PacketCommonDataDelete* pPkt);

protected:
	CommonDataOwner*	m_pCommonDataOwner;
	CommonDataCont*		m_pCont[eCommonData_Max];
};
