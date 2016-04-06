#pragma once

#include <assert.h>

template <class T>
class CircularListEx
{
public:
	CircularListEx(int32 ELEMENTCOUNT_) : m_pos(0), m_count(0), ELEMENTCOUNT(ELEMENTCOUNT_), m_pElements(NULL), m_DefVal(T())
	{

	}

	~CircularListEx(void)
	{
		SAFE_DELETE_ARRAY(m_pElements)
	}

	void Clear()
	{
		m_pos = 0;
		m_count = 0;
		memset( m_pElements, 0, sizeof(T)*ELEMENTCOUNT);

		for (int32 i = 0; i < ELEMENTCOUNT; ++i)
		{
			m_pElements[i] = m_DefVal;
		}
	}

	bool Init(T dVal)
	{
		m_DefVal = dVal;
		m_pElements = new T[ELEMENTCOUNT];
		if(!m_pElements)
			return false;

		for (int32 i = 0; i < ELEMENTCOUNT; ++i)
		{
			m_pElements[i] = m_DefVal;
		}

		return true;
	}

	int32 add(T& val)
	{
		assert(m_pElements);

		if(m_count >= ELEMENTCOUNT)
			return -1;

		int32 nRes = m_pos;
		while (m_pElements[m_pos] != m_DefVal)
		{
			m_pos = (m_pos + 1)%ELEMENTCOUNT;
		}

		m_pElements[m_pos] = val;
		nRes = m_pos;
		m_pos++;
		m_count++;

		if(m_pos >= ELEMENTCOUNT)
			m_pos = 0;

		return nRes;
	}

	inline T XGet(int32 nIdx)
	{
		return m_pElements[nIdx];
	}

	inline bool remove(int32 index)
	{
		assert(m_pElements);

		if( index < 0 || index >= ELEMENTCOUNT)
			return false;

		if(m_pElements[index] == m_DefVal)
			return false;

		m_pElements[index] = m_DefVal;
		m_count--;
		return true;
	}

	inline int32 GetMaxElement() const { return ELEMENTCOUNT; }
	int32 GetCount() { return m_count; }
	bool IsFull() { return m_count >= ELEMENTCOUNT; }
private:
	CircularListEx(const CircularListEx&);
	CircularListEx& operator=(const CircularListEx&);

	T*				m_pElements;
	int32			m_pos;
	volatile int32	m_count;
	T				m_DefVal;
	const int32		ELEMENTCOUNT;
};

