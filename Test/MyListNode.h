#pragma once

#include <list>

template <class T>
class MyListNode : public std::list<T>
{
public:
	MyListNode(void){}
	~MyListNode(void){}

	T Get() { return m_pOwner; }
	void Set(T p) { m_pOwner = p; }

protected:
	T m_pOwner;
};

