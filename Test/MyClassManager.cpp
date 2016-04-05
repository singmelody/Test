#include "StdAfx.h"
#include "MyClassManager.h"
#include "MyClass.h"
#include "MyXml.h"

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

void MyClassManagerBase::Print(const char* sFile)
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
		if( !pClass || !pElement )
			continue;

		pElement->SetValue("Class");
		pElement->SetAttribute("ClassName", pClass->ClassName());
		pElement->SetAttribute("ClassID", pClass->ClassID());
		pElement->SetAttribute("MemeberCount", pClass->MemberCount());

		std::string sParName = "BaseClass";
		if(pClass->Parent())
			sParName = pClass->Parent()->ClassName();

		pElement->SetAttribute("Parent", sParName.c_str());
		
		int32 nMemCount = pClass->MemberCount();
		for (int32 memIdx = 0; memIdx < nMemCount; ++memIdx)
		{
			TypeBase* pMem = pClass->GetMember(i);
			if(!pMem)
				continue;

			XmlElement el("Member");
			el.SetAttribute("Name", pMem->Name());
			el.SetAttribute("Type", pMem->TypeName());
			el.SetAttribute("Size", pMem->Size());
			el.SetAttribute("Flag", pMem->GetTypeFlag());
			el.SetAttribute("Des", pMem->Des());

			pElement->InsertEndChild(el);
		}
	}
}
