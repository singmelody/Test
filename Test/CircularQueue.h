#pragma once

#include "BaseType.h"

template <class T>
class Helper
{
public:
	enum { eNeedInit = 0 };
};

template<class T>
class Helper<T*>
{
public:
	enum { eNeedInit = 1 };
};

template <class T, int32 ELEMENTCOUNT>
class CircularQueue
{
	T		m_elements[ELEMENTCOUNT];
	int32	_push;
	int32	_pop;
	int32	_count;
public:
	CircularQueue(void)
	{
		clear();
	}
	~CircularQueue(void){}

	bool push(T& val)
	{
		if( _count >= ELEMENTCOUNT)
			return false;

		m_elements[_push] = val;
		_push++;
		if(_push >= ELEMENTCOUNT)
			_push = 0;
		_count++;
		return true;
	}

	bool pop(T& val)
	{
		if(_count == 0)
			return false;

		val = m_elements[_pop];
		_pop++;
		if(_pop >= ELEMENTCOUNT)
			_push = 0;
		_count--;
		return true;
	}

	bool pop_back(T& val)
	{
		if(_count == 0)
			return false;

		_push = ( _push == 0 ? (ELEMENTCOUNT-1):(_push-1));
		val = m_elements[_push];
		_count--;
		return true;
	}

	T& get(int32 pos) { return m_elements[pos]; }

	int32 count() { return _count; }

	int32 start() { return _pop; }
	int32 next(int32 nPos)
	{
		if( pos == _push )
			return -1;

		pos++;
		if( pos >= ELEMENTCOUNT)
			pos = 0;
		return pos;
	}

	void clear()
	{
		_push = _pop = _count = 0;

		if( Helper<T>::eNeedInit )
			memset( m_elements, 0, sizeof(m_elements));
	}
};

