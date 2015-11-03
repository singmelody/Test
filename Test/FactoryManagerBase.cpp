#include "stdafx.h"
#include "FactoryManagerBase.h"

void* CFactoryManagerBase::New(int32 nClassID)
{
	if (nClassID < 0 || nClassID >= (int32)m_FuncID_Arg0.size())
		return NULL;

	CFactoryBase_Arg0* pFactory = m_FuncID_Arg0[nClassID];

	if (!pFactory)
		return NULL;

	return pFactory->New();
}

void* CFactoryManagerBase::New(const char* pClassName)
{
	if (!pClassName)
		return NULL;

	FactoryMap::iterator itr = m_FuncName_Arg0.find(pClassName);

	if (itr == m_FuncName_Arg0.end())
		return NULL;

	CFactoryBase_Arg0* pFactoryBase = itr->second;
	if (!pFactoryBase)
		return NULL;

	return pFactoryBase->New();
}

void CFactoryManagerBase::AddFactory(CFactoryBase_Arg0* pFunc)
{
	if (!pFunc)
		return;

	m_FuncName_Arg0[pFunc->ClassName()] = pFunc;
	
	int32 nClassID = pFunc->ClassID();

}

void CFactoryManagerBase::AddFactoryEx(CFactoryBase_Arg0* pFunc)
{

}
