#pragma once
#include "MyListNode.h"
#include "EventBase.h"

class EventHandlerBase;

template <class T>
class EventProcessorBase
{
public:
	EventProcessorBase(void)
	{
		m_ProcPtr = NULL;
		m_ProcPtr = static_cast<T*>(this);
	}

	EventProcessorBase(T* pPtr)
	{
		m_ProcPtr = pPtr;
	}

	~EventProcessorBase(void)
	{

	}

	virtual void TickEvent()
	{
		EventBase* pEvent = m_EventList.Pop_Head();
		while (pEvent)
		{
			ProcEvent(pEvent);
			EventFactory::Instance().Delete(pEvent);
			pEvent = m_EventList.Pop_head();
		}
	}

	virtual void PushEvent(EventBase* pEvent)
	{
		if(!pEvent)
			return;

		EventBase* pNew = (EventBase*)EventFactory::Instance().New(pEvent->GetClass()->ClassID());
		if(!pNew)
			return;

		pNew->Clone(pEvent);
		m_EventList.Push_back(pNew);
	}

protected:
	T*	m_ProcPtr;
	MyList<EventBase>	m_EventList;
};

