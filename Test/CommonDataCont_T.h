#pragma once

#include "CommonDataCont.h" 
#include "CommonDataObj.h"
#include "MyLog.h"

template<int32 COMMON_DATA_TYPE, int32 MAX_SIZE>
class CommonDataCont_T : public CommonDataCont
{
public:
	CommonDataCont_T(void) : CommonDataCont(CommonDataType(COMMON_DATA_TYPE))
	{
		memset( m_Objs, 0, sizeof(m_Objs));
		m_ObjCount = 0;
		m_Itr = 0;
		m_nMaxIdx = 0;
	}

	~CommonDataCont_T(void){}

	virtual int32 Add(CommonDataObject* pObj, int32 nIdx = -1)
	{
		if(!pObj)
		{
			MyLog::error("CommonDataCont_T::Add() fail pObj is NULL");
			return -1;
		}

		if(nIdx != -1)
		{
			if( nIdx < 0 || nIdx >= MAX_SIZE || m_Objs[nIdx])
			{
				MyLog::error("CommonDataCont_T::Add() fail:index=%d,MAX_SIZE=%d or m_Objs[%d] is Not NULL",  
					nIdx, MAX_SIZE, nIdx);
				return -1;
			}
		}
		else
		{
			for (int32 i = 0; i < MAX_SIZE; ++i)
			{
				if(!m_Objs[i] && !IsIndexOccupied(i))
				{
					nIdx = i;
					break;
				}
			}

			if( nIdx == -1)
			{
				MyLog::error("CommonDataCont_T::Add() fail Objs is Full");
				return -1;
			}
		}

		m_Objs[nIdx] = pObj;

		pObj->SetIndex(nIdx);
		pObj->SetOwner(m_pOwner);
		pObj->m_nCommonDataType = this->m_Type;

		++m_ObjCount;
		m_CommonObjTickList.Add(pObj);

		if( m_nMaxIdx < nIdx )
			m_nMaxIdx = nIdx;

		return nIdx;
	}

	virtual bool		Remove(int32 nIdx, bool bDel = true)
	{
		CommonDataObject* pObj = Get(nIdx);
		if(!pObj)
			return false;

		m_Objs[nIdx] = NULL;
		--m_ObjCount;

		m_CommonObjTickList.Remove(pObj);
		if(bDel)
			FreeObject(pObj);
		else
			pObj->SetOwner(NULL);

		return true;
	}

	virtual CommonDataObject* Get(int32 nIdx) const
	{
		if( nIdx < 0 || nIdx >= MAX_SIZE)
			return NULL;

		return m_Objs[nIdx];
	}

	virtual bool IsIndexOccupied(int32 nIdx) { return false; }
protected:
	CommonDataObject*			m_Objs[MAX_SIZE];
	int32						m_ObjCount;
	int32						m_Itr;
	CommonDataObjectTickList	m_CommonObjTickList;
};