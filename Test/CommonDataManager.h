#pragma once

#include "CommonDataCont.h"

class CommonDataOwner;

class CommonDataManager
{
public:
	CommonDataManager();
	virtual ~CommonDataManager();

	virtual void SetOwner(CommonDataOwner* pOwner);

	void	AddCont(CommonDataCont* pCont);
	void	ClearCont(CommonDataType type);

	virtual void NotifyCreate( CommonDataObject* pObj, int32 nFlag){}
	virtual void NotifyDelete( CommonDataObject* pObj){}

	CommonDataCont* GetCont(CommonDataType type) const;
protected:
	CommonDataOwner*	m_pCommonDataOwner;
	CommonDataCont*		m_pCont[eCommonData_Max];
};
