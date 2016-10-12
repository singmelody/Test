#pragma once

#include "CommonDataCont.h" 

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

	virtual int32		Add(CommonDataObject* pObj, int32 nIdx = -1);
	virtual bool		Remove(int32 nIdx, bool bDel = true);

protected:
	CommonDataObject*			m_Objs[MAX_SIZE];
	int32						m_ObjCount;
	int32						m_Itr;
	CommonDataObjectTickList	m_CommonObjTickList;
};

