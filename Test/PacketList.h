#pragma once
#include "MyMutex.h"

class PacketBase;

class PacketList
{
public:
	PacketList(void);
	~PacketList(void);

	void Push_Head(PacketBase* pkt);
	void Push_Back(PacketBase* pkt);

	PacketBase* Pop_Head();
	PacketBase* Pop_Back();
	PacketBase* GetNext(PacketBase* pkt);
	PacketBase* Remove(PacketBase* pkt);

	int32 GetCount() { return m_Count; }

protected:
	PacketBase* _head;
	PacketBase* _back;
	volatile int32 m_Count;
	Mutex		m_lock;
};

class PacketListEx
{
public:
	PacketListEx(void);
	~PacketListEx(void);

	void Push_Head(PacketBase* pkt);
	void Push_Back(PacketBase* pkt);

	PacketBase* Pop_Head();
	PacketBase* Pop_Back();
	PacketBase* GetNext(PacketBase* pkt);
	PacketBase* Remove(PacketBase* pkt);

	int32 GetCount() { return m_Count; }

protected:
	PacketBase* _head;
	PacketBase* _back;
	volatile int32 m_Count;
};


