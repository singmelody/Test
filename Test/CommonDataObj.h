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


	virtual bool InitObject();
	virtual void AfterInitObject(){}
	virtual void InitObjFlag();

	virtual void OnUpdateObj() {}

	virtual void NotifyCreate( int32 nFlag );
	virtual void NotifyDelete();

	bool CheckObjectFlag(int32 nFlag) { return (nFlag & m_nFlag) != 0; }
	void SetObjectFlag(int32 nFlag);

	bool NeedSync2World() { return CheckObjectFlag( eObjectFlag_WorldNotSave | eObjectFlag_SaveFlag  );}
	bool NeedSync2DBA() { return CheckObjectFlag( eObjectFlag_SaveFlag );}
	bool NeedSync2WClt() { return CheckObjectFlag( eObjectFlag_CltFlag );}

	void SetIndex(int32 nIdx);

	inline CommonDataObject* prev() { return _prev; }
	inline void prev(CommonDataObject* _p) { _prev = _p; }

	inline CommonDataObject* next() { return _next; }
	inline void next(CommonDataObject* _p) { _next = _p; }

	int32	m_nCommonDataType;
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

	CommonDataObjectTickList& operator=(CommonDataObjectTickList& rh)
	{
		m_nCount = rh.m_nCount;
		_head = rh._head;
		_back = rh._back;

		CommonDataObject* tmp = GetNext(NULL);
		while (tmp)
		{
			tmp->pList = this;
			tmp = GetNext(tmp);
		}

		return *this;
	}

	// add to head
	void Add(CommonDataObject* ptr)
	{
		if (!ptr || NULL != ptr->pList )
			return;

		ptr->prev(NULL);
		ptr->next(_head);
		if(_head)
		{
			_head->prev(ptr);
			_head = ptr;
		}
		else
		{
			_head = _back = ptr;
		}

		ptr->pList = this;
		m_nCount++;
	}

	// return next
	CommonDataObject* Remove(CommonDataObject* ptr)
	{
		if(!ptr || (ptr->pList != this))
			return NULL;

		CommonDataObject* res = ptr->next();
		if(ptr->prev())
		{
			ptr->prev()->next(ptr->next());
		}
		else
		{
			_head = ptr->next();
			if(_head)
				_head->prev(NULL);
		}

		if( ptr->next())
			ptr->next()->prev(ptr->prev());
		else
		{
			_back = ptr->prev();
			if(_back)
				_back->next(NULL);
		}

		ptr->next(NULL);
		ptr->prev(NULL);
		ptr->pList = NULL;
		m_nCount--;
		return res;
	}

	void Push_head(CommonDataObject* ptr)
	{
		Add(ptr);
	}

	void Push_back(CommonDataObject* ptr)
	{
		if(!ptr || NULL != ptr->pList )
			return;

		ptr->next(NULL);
		ptr->prev(_back);
		if(_back)
		{
			_back->next(ptr);
			_back = ptr;
		}
		else
		{
			_head = _back = ptr;
		}

		ptr->pList = this;
		m_nCount++;
	}

	CommonDataObject* Pop_head()
	{
		CommonDataObject* pRes = _head;
		Remove(_head);
		return pRes;
	}

	CommonDataObject* Pop_back()
	{
		CommonDataObject* res = _back;
		Remove(_back);
		return res;
	}

	CommonDataObject* GetNext(CommonDataObject* ptr) const
	{
		if(!ptr)
			return _head;

		if( this != ptr->pList )
			return NULL;

		return ptr->next();
	}

	CommonDataObject* GetPrev(CommonDataObject* ptr) const
	{
		if(!ptr)
			return _back;

		if( this != ptr->pList )
			return NULL;

		return ptr->prev();
	}

	// ptr->p->c->ptr
	void InsertAfter( CommonDataObject* p, CommonDataObject* c)
	{
		if( !p || this != p->pList || !c || NULL != c->pList)
			return;

		if(!p->next())
		{
			Push_back(c);
			return;
		}

		c->next(p->next());
		c->prev(p);
		p->next()->prev(c);
		p->next(c);
		p->pList = this;
		m_nCount++;
	}

	// ptr->c->p->ptr
	void InsertBefore(CommonDataObject* p, CommonDataObject* c)
	{
		if( !p || this != p->pList || !c || NULL != c->pList)
			return;

		if(!p->prev())
		{
			Push_head(c);
			return;
		}

		c->prev(p->prev());
		c->next(p);
		c->prev()->next(c);
		p->prev(c);
		c->pList = this;
		m_nCount++;
	}

	int32 GetCount() const { return m_nCount; }
protected:
	int32				m_nCount;
	CommonDataObject*	_head;
	CommonDataObject*	_back;
};