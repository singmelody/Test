#pragma once
#include "GameObject.h"

class CommonDataOwner;

enum ObjectFlag
{
	eObjectFlag_CltFlag		= 1 << 0,	// 同步给自己客户端 
	eObjectFlag_SaveFlag	= 1 << 1,	// 同步给数据库
	eObjectFlag_WorldNotSave= 1 << 2,	// 同步给World

	eObject_NodeTick		= 1 << 4,
	eObject_WorldTick		= 1 << 5,
};

enum ValueChangeMode
{
	eVCM_None,
	eVCM_Create,
	eVCM_Init,
	eVCM_Update,
};

class CommonDataObjectTickList;

class CommonDataObject : public GameObject
{
	friend class CommonDataObjectTickList;
public:
	CommonDataObject();
	virtual ~CommonDataObject(){}

	virtual void SetOwner( CommonDataOwner* pOwner);
	CommonDataOwner* GetOwner() { return m_pCommonDataOwner; }

	void SetObjectFlag(int32 nFlag);

	virtual bool InitObject();
	virtual void AfterInitObject(){}
	virtual void InitObjFlag();

	virtual void OnUpdateObj() {}

	virtual void NotifyCreate( int32 nFlag );
	virtual void NotifyDelete();
protected:
	int32						m_nIdx;
	int32						m_nFlag;
	CommonDataOwner*			m_pCommonDataOwner;

	bool						m_bSyncCltSelf;
	bool						m_bSyncCltAll;

	bool						m_bInit;
	int32						m_nValueChangeMode;

	CommonDataObject*			_next;
	CommonDataObject*			_prev;
	CommonDataObjectTickList*	pList;
};

class CommonDataObjectTickList
{
public:
	CommonDataObjectTickList()
		: _head(NULL)
		, _back(NULL)
		, m_nCount(0)
	{

	}

	virtual ~CommonDataObjectTickList()
	{

	}

protected:
	int32				m_nCount;
	CommonDataObject*	_head;
	CommonDataObject*	_back;
};