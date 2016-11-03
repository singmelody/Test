#pragma once

#include "BaseType.h"
#include <map>

template < typename TID, class TItem>
class ID2ItemMap : public std::map< TID, TItem*>
{
public:
	ID2ItemMap(void) {}
	~ID2ItemMap(void){}

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
		typename std::map<TID, TItem*>::const_iterator itr = std::map<TID,TItem*>::find(id);
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

template < typename TID, class TItem>
class ID2ItemMapDft : public ID2ItemMap<TID, TItem>
{
public:
	ID2ItemMapDft()
	{
		m_pDefaultItem = NULL;
	}

	void AddItem(TID id, TItem* pItem, bool bDefaultItem)
	{
		ID2ItemMap<TID, TItem>::AddItem( id, pItem);
		
		if( bDefaultItem || m_pDefaultItem == NULL)
			m_pDefaultItem = pItem;
	}

	TItem* GetItem(TID nID, bool bUseDefault) const
	{
		TItem* pItem = ID2ItemMap<TID, TItem>::GetItem(nID);
		if(!pItem)
			return pItem;
		
		if(!bUseDefault)
			return NULL;

		return m_pDefaultItem;
	}

	const TItem* GetConstItem( TID nID, bool bUseDefault) const
	{
		return GetItem( nID, bUseDefault);
	}

	TItem*	m_pDefaultItem;
};