#pragma once

#include "BaseType.h"
#include <map>

template < typename key, typename value>
class GMap
{
public:
	GMap::GMap(void)
	{
		itr = std::map<key,value>::begin();
	}


	GMap::~GMap(void)
	{
	}

	void Begin()
	{
		itr = std::map<key,value>::begin();
	}

	void End()
	{
		itr = std::map<key,value>::end();
	}

	int32 Count()
	{
		return (int32)(std::map<key, value>::size());
	}

	bool Insert( key k, value v)
	{
		if(Find(k))
			return false;

		std::map<key, value>::insert(std::make_pair<key, value>( k, v));
		return true;
	}

	bool Find(key k)
	{
		itr = std::map<key,value>::find(k);
		if(itr == std::map<key,value>::end())
			return false;

		return true;
	}

	value Find(key k, bool)
	{
		itr = std::map<key,value>::find(k);
		if(itr == std::map<key,value>::end())
			return (value)NULL;

		return itr->second;
	}

	void Next()
	{
		if( std::map<key, value>::empty() || itr == std::map<key, value>::end())
			return;

		++itr;
	}

	void Remove()
	{
		std::map< key, value>::erase(itr++);
	}

	void Remove(key k)
	{
		if(Find(k))
			Remove();
	}

	value& Get()
	{
		return itr->second;
	}

	void Set(value& v)
	{
		itr->second = v;
	}

	const key GetKey()
	{
		return itr->first;
	}

	bool IsEnd()
	{
		return itr == std::map< key, value>::end();
	}
protected:
	typedef typename std::map<key, value>::iterator iterator;
	iterator itr;
};

