#pragma once

#include "CommonDataCont.h"
#include <vector>

class CommonDataOwner;

class CommonDataObserver
{
public:
	virtual void OnCreate(CommonDataObject* pObj){}
	virtual void OnInit(CommonDataObject* pObj){}
	virtual void OnPreUpdate(CommonDataObject* pObj){}
	virtual void OnUpdate(CommonDataObject* pObj){}
	virtual void OnDelete(CommonDataObject* pObj){}
};


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

	virtual void AddObserver( CommonDataType type, CommonDataObserver* pObserver){}
protected:
	virtual void OnCreateObj(CommonDataObject* pObj){}
	virtual void OnInitObj(CommonDataObject* pObj){}
	virtual void OnPreUpdateObj(CommonDataObject* pObj){}
	virtual void OnUpdateObj(CommonDataObject* pObj){}
	virtual void OnDeleteObj(CommonDataObject* pObj){}

	CommonDataOwner*	m_pCommonDataOwner;
	CommonDataCont*		m_pCont[eCommonData_Max];
};

typedef void(CommonDataObserver::*NOTIFYFUNC)(CommonDataObject* pObj);

class CommonDataManagerGame : public CommonDataManager
{
public:
	virtual void AddObserver(CommonDataType type, CommonDataObserver* pObserver);

protected:
	void NotifyOberserver( CommonDataObject* pObj, NOTIFYFUNC pFunc);

	virtual void OnCreateObj(CommonDataObject* pObj);
	virtual void OnInitObj(CommonDataObject* pObj);
	virtual void OnPreUpdateObj(CommonDataObject* pObj);
	virtual void OnUpdateObj(CommonDataObject* pObj);
	virtual void OnDeleteObj(CommonDataObject* pObj);

	typedef std::vector<CommonDataObserver*> ObserverList;

	ObserverList m_vObserverList[eCommonData_Max];
};