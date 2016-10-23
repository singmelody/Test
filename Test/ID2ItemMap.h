#pragma once

#include "BaseType.h"
#include <map>

template < typename TID, class TItem>
class ID2ItemMap : public std::map< TID, TItem*>
{
public:
	ID2ItemMap(void);
	~ID2ItemMap(void);

	void AddItem( TID id, TItem* pItem)
	{
		(*this)[id] = pItem;
	}

	void RemoveItem( TID id)
	{
		typename std::map<TID, TItem*>::iterator itr = std::map<TID,TItem*>::find(id);
		if( itr != std::map<TID,TItem*>::end() )
			std::map<TID, TItem*>::erase(itr);
	}

	const TItem* GetConstItem( TID id) const
	{
		return GetItem(id);
	}

	TItem* GetItem( TID id) const
	{
		typename std::map<TID, TItem*>::iterator itr = std::map<TID,TItem*>::find(id);
		if( itr != std::map<TID,TItem*>::end() )
			return itr->second;

		return NULL;
	}

	TItem* RemoveFirst()
	{
		typename std::map<TID, TItem*>::iterator itr = std::map<TID,TItem*>::find(id);
		if( itr == std::map<TID,TItem*>::end() )
			return NULL;

		TItem* pItem = itr->second;
		std::map<TID, TItem*>::erase(itr);
		return pItem;
	}
};

// template < typename TID, class TItem>
// class ID2ItemMapDft 