#include "StdAfx.h"
#include "PacketList.h"
#include "PacketBase.h"

PacketList::PacketList(void)
{
	_head = _back = NULL;
	m_Count = 0;
}


PacketList::~PacketList(void)
{
}

void PacketList::Push_Head(PacketBase* pkt)
{
	AUTOLOCK(m_lock);

	if(!pkt)
		return;

	pkt->_prev = NULL;
	pkt->_next = NULL;
	pkt->_next = _head;

	if(_head)
		_head->_prev = pkt;

	_head = pkt;

	if(!_back)
		_back = _head;

	m_Count++;
}

void PacketList::Push_Back(PacketBase* pkt)
{
	AUTOLOCK(m_lock);
	if(!pkt)
		return;

	pkt->_prev = NULL;
	pkt->_next = NULL;
	pkt->_prev = _back;

	if(_back)
		_back->_next = pkt;

	_back = pkt;

	if(!_head)
		_head = _back;

	m_Count++;
}

PacketBase* PacketList::Pop_Head()
{
	AUTOLOCK(m_lock);
	if(!_head)
		return NULL;

	PacketBase* res = _head;
	_head = _head->_next;
	if(!_head)
		_back = NULL;

	m_Count--;
	return res;
}

PacketBase*	PacketList::Pop_Back()
{
	AUTOLOCK(m_lock);
	if(!_back)
		return NULL;

	PacketBase* res = _back;
	_back = _back->_prev;
	if(!_back)
		_head = NULL;

	m_Count--;
	return res;
}

PacketBase* PacketList::Remove(PacketBase* pkt)
{
	AUTOLOCK(m_lock);

	if(!pkt)
		return NULL;

	if( _head == pkt || pkt->_next || pkt->_prev)
		m_Count--;

	PacketBase* res = pkt->_next;
	if(pkt->_prev)
		pkt->_prev->_next = pkt->_next;
	else
	{
		_head = pkt->_next;
		if(_head)
			_head->_prev = NULL;
	}

	if(pkt->_next)
		pkt->_next->_prev = pkt->_prev;
	else
	{
		_back = pkt->_prev;
		if(_back)
			_back->_next = NULL;
	}

	pkt->_next = NULL;
	pkt->_prev = NULL;

	return res;
}

PacketBase* PacketList::GetNext(PacketBase* pkt)
{
	if(!pkt)
		return _head;

	return pkt->_next;
}

PacketListEx::PacketListEx(void)
{
	_head = _back = NULL;
	m_Count = 0;
}


PacketListEx::~PacketListEx(void)
{
}

void PacketListEx::Push_Head(PacketBase* pkt)
{
	if(!pkt)
		return;

	pkt->_prev = NULL;
	pkt->_next = NULL;
	pkt->_next = _head;

	if(_head)
		_head->_prev = pkt;

	_head = pkt;

	if(!_back)
		_back = _head;

	m_Count++;
}

void PacketListEx::Push_Back(PacketBase* pkt)
{
	if(!pkt)
		return;

	pkt->_prev = NULL;
	pkt->_next = NULL;
	pkt->_prev = _back;

	if(_back)
		_back->_next = pkt;

	_back = pkt;

	if(!_head)
		_head = _back;

	m_Count++;
}

PacketBase* PacketListEx::Pop_Head()
{
	if(!_head)
		return NULL;

	PacketBase* res = _head;
	_head = _head->_next;
	if(!_head)
		_back = NULL;

	m_Count--;
	return res;
}

PacketBase*	PacketListEx::Pop_Back()
{
	if(!_back)
		return NULL;

	PacketBase* res = _back;
	_back = _back->_prev;
	if(!_back)
		_head = NULL;

	m_Count--;
	return res;
}

PacketBase* PacketListEx::Remove(PacketBase* pkt)
{
	if(!pkt)
		return NULL;

	if( _head == pkt || pkt->_next || pkt->_prev)
		m_Count--;

	PacketBase* res = pkt->_next;
	if(pkt->_prev)
		pkt->_prev->_next = pkt->_next;
	else
	{
		_head = pkt->_next;
		if(_head)
			_head->_prev = NULL;
	}

	if(pkt->_next)
		pkt->_next->_prev = pkt->_prev;
	else
	{
		_back = pkt->_prev;
		if(_back)
			_back->_next = NULL;
	}

	pkt->_next = NULL;
	pkt->_prev = NULL;

	return res;
}

PacketBase* PacketListEx::GetNext(PacketBase* pkt)
{
	if(!pkt)
		return _head;

	return pkt->_next;
}

