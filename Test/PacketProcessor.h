#pragma once

#include "PacketList.h"
#include <map>

template < class T>
class FunctionPacketBase
{
public:
	FunctionPacketBase(){}
	virtual ~FunctionPacketBase() {}
	virtual void operator()(T arg){}
};

template < class T, class T2, class T3>
class FunctionPacket : public FunctionPacketBase<T3>
{
	typedef void( T::*Function)(T2 pkt);
public:
	FunctionPacket(T* P, Function f)
	{
		pObj = P;
		func = f;
	}

	virtual ~FunctionPacket(){}

	virtual void operator()(T3 basePkt)
	{
		if(!pObj || !func)
			return;

		T2 ptr = (T2)basePkt;
		(pObj->*func)(ptr);
	}
private:
	T*			pObj;
	Function	func;
};

class PacketBase;

class PacketProcessor
{
	typedef FunctionPacketBase<PacketBase*> FuncEntry;
public:
	PacketProcessor();
	virtual ~PacketProcessor();

	virtual bool OnPacketReceived( PacketBase* pPkt);
	bool PushPacket( PacketBase* pPkt );

	void RegDefaultHandler( FuncEntry* pFunc);
	void RegPacketHandler( int32 nPacket, FuncEntry* pFunc);
	bool ProcessPacket(PacketBase* pPkt);
	void RemovePacketHandler( int32 nPacketID, char* sFuncName);
protected:
	PacketList		m_receivingPktList;

	virtual FuncEntry* FindFuncEntry( PacketBase* pPkt);
	virtual int32 GetFuncEntryID( PacketBase* pPkt );

	typedef std::map< int32 , FuncEntry*> FuncMap;
	FuncMap	m_functionMap;

	FunctionPacketBase< PacketBase* >*	DefaultPacketFunction;

	int32 m_RecvPacketCount;
	int32 m_RecvPacketSize;
};		  