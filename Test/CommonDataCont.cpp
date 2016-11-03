#include "StdAfx.h"
#include "CommonDataCont.h"


CommonDataCont::CommonDataCont(void)
{
}


CommonDataCont::~CommonDataCont(void)
{
}

CommonDataObject* CommonDataCont::CreateCommonDataObj(int32 nDataID, int32 nIdx /*= -1*/)
{
	CommonDataObject* pObj = NewObject(nDataID);
	if(!pObj)
		return NULL;

	int32 nPos = Add( pObj, nIdx);
	if( nPos == -1)
	{
		FreeObject(pObj);
		return NULL;
	}

	return pObj;
}
