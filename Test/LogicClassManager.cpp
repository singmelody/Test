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

	XmlElement el("ClassList");
	XmlElement* pEl = (XmlElement*)doc.InsertEndChild(el);

	for (int32 i = 0; i < (int32)m_vClass.size(); ++i)
	{
		MyClass* pClass = m_vClass[i];
		XmlElement elclass("Class");
		XmlElement* pElement = (XmlElement*)pEl->InsertEndChild(elclass);
		if(!pClass || !pElement)
			continue;

		pElement->SetValue("Class");
		pElement->SetAttribute("ClassName", pClass->ClassName());
		pElement->SetAttribute("ClassID", pClass->ClassID());
		pElement->SetAttribute("MemberCount", pClass->MemberCount());
		std::string sParName = "BaseClass";
		if(pClass->Parent())
			sParName = pClass->Parent()->ClassName();

		pElement->SetAttribute( "Parent", sParName.c_str());

		int32 nMemCount = pClass->MemberCount();
		for (int32 i = 0; i < nMemCount; ++i)
		{
			TypeBase* pMem = pClass->GetMember(i);
			if(!pMem)
				continue;

			XmlElement el("Member");
			el.SetAttribute( "Name", pMem->Name());
			el.SetAttribute( "Type", pMem->TypeName());
			el.SetAttribute( "Size", pMem->Size());
			el.SetAttribute( "Flag", pMem->GetTypeFlag());
			el.SetAttribute( "Des", pMem->Des());
			pElement->InsertEndChild(el);
		}
	}
	doc.SaveFile();
}


LogicClassManager::LogicClassManager(void)
{

}


LogicClassManager::~LogicClassManager(void)
{
}