#pragma once

#include "BaseType.h"

template<class T, int32 ELEMENTCOUNT>
class CircularList
{
	T m_elements[ELEMENTCOUNT];
	int32 m_pos;
	int32 m_count;
	T m_DefVal;
public:
	CircularList(void) : m_pos(0), m_count(0)
	{

	}
	~CircularList(void){}

	void Clear()
	{
		m_pos = 0;
		m_count = 0;
		memset( m_elements, 0, sizeof(T)*ELEMENTCOUNT);
	}

	void Init(T dVal)
	{
		m_DefVal = dVal;
		for (int32 i = 0; i < ELEMENTCOUNT; ++i)
		{
			m_elements[i] = m_DefVal;
		}
	}

	int32 add(T& val)
	{
		if( m_count >= ELEMENTCOUNT)
			return -1;

		int32 nRes = m_pos;
		while (m_elements[m_pos] != m_DefVal)
		{
			m_pos++;
			if(nRes == m_pos)
				return -1;

			if(m_pos >= ELEMENTCOUNT)
				m_pos = 0;
		}
		m_elements[m_pos] = val;
		nRes = m_pos;
		m_pos++;
		m_count++;
		if(m_pos >= ELEMENTCOUNT)
			m_pos = 0;

		return nRes;
	}

	bool add(int32 nIndex, T& val)
	{
		if( nIndex < 0 || nIndex >= ELEMENTCOUNT)
			return false;

		if(m_elements[nIndex] != m_DefVal)
			return false;

		m_elements[nIndex] = val;
		m_count++;
		return true;
	}

	T get(int32 nIdx)
	{
		if( nIdx < 0 || nIdx >= ELEMENTCOUNT)
			return m_DefVal;

		return m_elements[nIdx];
	}

	T remove(int32 nIdx)
	{
		if( nIdx < 0 || nIdx >= ELEMENTCOUNT)
			return false;

		if(m_elements[nIdx] == m_DefVal)
			return false;

		m_elements[nIdx] = m_DefVal;
		m_count--;
		return true;
	}

	int32 GetCount() { return m_count; }
	bool IsFull() { return m_count >= ELEMENTCOUNT }
};

