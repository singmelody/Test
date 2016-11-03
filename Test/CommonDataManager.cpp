#include "StdAfx.h"
#include "CommonDataManager.h"
#include "FactoryManagerBase.h"

CommonDataManager::CommonDataManager()
{
	m_pCommonDataOwner = NULL;
	memset( m_pCont, 0, sizeof(m_pCont));
}

CommonDataManager::~CommonDataManager()
{
	for (int32 i = eCommonData_Min + 1; i < eCommonData_Max; ++i)
	{
		CommonDataCont* pCont = m_pCont[i];

		if(!pCont)
			continue;

		FACTORY_DELOBJ(pCont);
		ClearCont((CommonDataType)i);
	}
}

void CommonDataManager::SetOwner(CommonDataOwner* pOwner)
{
	m_pCommonDataOwner = pOwner;
}

void CommonDataManager::AddCont(CommonDataCont* pCont)
{
	if(!pCont)
		return;

	CommonDataType type = pCont->GetType();

	if( type <= eCommonData_Min || type >= eCommonData_Max )
		return;

	m_pCont[type] = pCont;
	pCont->SetOwner(m_pCommonDataOwner);
}

void CommonDataManager::ClearCont(CommonDataType type)
{
	if( type <= eCommonData_Min || type >= eCommonData_Max )
		return;

	m_pCont[type] = NULL;
}

CommonDataCont* CommonDataManager::GetCont(CommonDataType type) const
{
	if( type <= eCommonData_Min || type >= eCommonData_Max )
		return NULL;
	
	return m_pCont[type];
}
