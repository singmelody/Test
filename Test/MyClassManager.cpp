#include "StdAfx.h"
#include "MyClassManager.h"
#include "MyClass.h"

MyClassManager::MyClassManager(void)
{
}


MyClassManager::~MyClassManager(void)
{
}

MyClass* MyClassManagerBase::GetClass(const char* sName)
{
	if(!sName)
		return NULL;

	for (int32 i = 0; i < (int32)m_vClass.size();)
	{
		if( strcmp( m_vClass[i]->ClassName(), sName) == 0 )
			return m_vClass[i];
	}

	return NULL;
}

MyClass* MyClassManagerBase::GetClass(int32 nIdx)
{
	if( nIdx < 0 || nIdx >= (int32)m_vClass.size() )
		return NULL;

	return m_vClass[nIdx];
}

void MyClassManagerBase::AddClass(MyClass* pClass)
{
	if(!pClass)
		return;

	m_vClass.push_back(pClass);
}

void MyClassManager::Print(const char* sFile)
{
	XMLDocument doc;
	doc.LoadFile(sFile);
}