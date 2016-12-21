#include "StdAfx.h"
#include "LogicClassManager.h"
#include "MyXml.h"

MyClass* ClassManagerBase::GetClass(int32 nIdx)
{
	if( nIdx < 0 || (nIdx >= (int32)m_vClass.size()))
		return NULL;

	return m_vClass[nIdx];
}

MyClass* ClassManagerBase::GetClass(const char* sName)
{
	if(!sName)
		return NULL;

	for (int32 i = 0; i < (int32)m_vClass.size(); ++i)
	{
		if(strcmp( m_vClass[i]->ClassName(), sName) == 0)
			return m_vClass[i];
	}

	return NULL;
}

void ClassManagerBase::AddClass(MyClass* pClass)
{
	if(pClass)
		m_vClass.push_back(pClass);
}

void ClassManagerBase::Print(const char* sFile)
{
	XmlDocument doc;
	doc.LoadFile(sFile);
}


LogicClassManager::LogicClassManager(void)
{

}


LogicClassManager::~LogicClassManager(void)
{
}