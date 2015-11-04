#include "stdafx.h"
#include "FactoryManagerBase.h"
#include "Allocator.h"

void* FactoryManagerBase::New(int32 nClassID)
{
	if (nClassID < 0 || nClassID >= (int32)m_FuncID_Arg0.size())
		return NULL;

	FactoryBase_Arg0* pFactory = m_FuncID_Arg0[nClassID];

	if (!pFactory)
		return NULL;

	return pFactory->New();
}

void* FactoryManagerBase::New(const char* pClassName)
{
	if (!pClassName)
		return NULL;

	FactoryMap::iterator itr = m_FuncName_Arg0.find(pClassName);

	if (itr == m_FuncName_Arg0.end())
		return NULL;

	FactoryBase_Arg0* pFactoryBase = itr->second;
	if (!pFactoryBase)
		return NULL;

	return pFactoryBase->New();
}

void FactoryManagerBase::Delete(void* ptr)
{
	if (!ptr)
		return;

	int32 nInfo = ( (MemoryHead*)((char*)ptr-sizeof(MemoryHead)))->AllocInfo;

	if (nInfo < 0 || nInfo >= (int32)m_FuncID_Arg0.size())
		return;

	FactoryBase_Arg0* pFactory = m_FuncID_Arg0[nInfo];
	if (!pFactory)
		return;

	pFactory->Free(ptr);
}

void FactoryManagerBase::AddFactory(FactoryBase_Arg0* pFunc)
{
	if (!pFunc)
		return;

	m_FuncName_Arg0[pFunc->ClassName()] = pFunc;
	
	int32 nClassID = pFunc->ClassID();
	if (nClassID == -1)
	{
		pFunc->ClassID((int32)m_FuncID_Arg0.size());
		m_FuncID_Arg0.push_back(pFunc);
		return;
	}

	if (nClassID >= (int32)m_FuncID_Arg0.size())
		m_FuncID_Arg0.resize(nClassID+1);

	m_FuncID_Arg0[nClassID] = pFunc;
}

