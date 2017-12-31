#pragma once

#include "Factory.h"
#include "MyListNode.h"

class EventFactory : public FactoryManagerBase, public Singleton<EventFactory>
{
public:
	EventFactory(){}
	virtual ~EventFactory(){}

	void* New(int32 nClassID)
	{
		FactoryBase_Arg0* pFactory = m_FuncName_Arg0[nClassID];
		if(!pFactory)
			return NULL;

		return pFactory->New();
	}
};

class EventBase
{
	friend class MyList<EventBase>;
	DECLARE_FACTORY_ARG0_Ex( EventBase, -1, new PoolAllocator, EventFactory::Instance());
public:
	EventBase(void);
	virtual ~EventBase(void){}

	virtual void Init() {}
	virtual uint32 EventID() { return m_cid; }
	virtual void EventID(int32 cid) { m_cid = cid;}

	inline EventBase* prev() { return _prev; }
	inline void prev(EventBase* _p)	{ _prev = _p; }

	inline EventBase* next() { return _next; }
	inline void next(EventBase* _n) { _next = _n; }

	virtual EventBase& Clone(EventBase* pEvent) { return *this; }
protected:
	uint32 m_cid;
	EventBase*			_next;
	EventBase*			_prev;
	MyList<EventBase>	pList;
};

#define EVENT( event, baseevent, id) \
	class event : public baseevent \
	{ \
		DECLARE_FACTORY_ARG0_Ex( event, -1, new PoolAllocator, EventFactory::Instance()); \
	public:
		event(){ EventID(id); Init(); } \
		virtual ~event() {} \
		virtual EventBase& Clone(EventBase* pEvent) \
		{\
			if(pEvent && (pEvent->GetClass()->ClassID() == GetClass()->ClassID()) ) \
			{ \
				memcpy((void*)this, (void*)pEvent, sizeof(event)); \
			}\
			return *this;\
		}\

#define Add_Event_Member_Begin(event) \
	Ref_Class_Begin( event, EventBase)

#define Add_Event_Member( event, member, type, des) \
	Add_Ref_Member( event, member type, eTB_Flag_Print | eTB_Flag_Sec, des)

#define Add_Event_Member_End(event) \
	Ref_Class_End( event, EventBase);

#define EVENT_END \
	}; \

#define EVENT_IMPL(event) \
	FINISH_FACTORY_ARG0(event); \
	Ref_Class(event);