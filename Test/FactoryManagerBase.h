#pragma once
#include <string>
#include <vector>
#include <map>
#include "Singleton.h"
#include "UtilID.h"

#define FACTORY_NEWOBJ(className) (className*)FactoryManagerBase::Instance().New(#className);
#define FACTORY_DELOBJ( pObj, id) { if(pObj) { FactoryManagerBase::Instance().Delete( pObj, id); pObj = NULL; }}

class Allocator;

class CFactoryBase_Arg0
{
public:
	CFactoryBase_Arg0() : m_id(-1){}
	~CFactoryBase_Arg0(){}

	virtual void ClassID(int32 nID) { m_id = nID; }
	int32 ClassID() { return m_id; }

	const char* ClassName() { return m_name.c_str(); }
	void ClassName(const char* pName)
	{
		if (!pName)
			return;

		m_name = pName;
	}

	virtual void* New() = 0;
	virtual void Free(void* ptr) = 0;
protected:
	int32		m_id;
	std::string m_name;
};

template < class T>
class FactoryBase_Arg0 : public CFactoryBase_Arg0
{
public:
	FactoryBase_Arg0() : m_pAlloc(NULL){}
	FactoryBase_Arg0(Allocator* pAlloc) : m_pAlloc(pAlloc) {}
protected:
	Allocator* m_pAlloc;
};

class FactoryManagerBase : public Singleton < FactoryManagerBase >
{
public:
	FactoryManagerBase() {}
	virtual ~FactoryManagerBase() {}

	virtual void* New(int32 nClassID);
	virtual void* New(const char* pClassName);

	virtual void Delete(void* ptr);

	virtual void AddFactory(CFactoryBase_Arg0* pFunc);

	int32 GetFactoryCount() { return (int32)m_FuncID_Arg0.size(); }

protected:
	typedef std::map< UtilID, CFactoryBase_Arg0*> FactoryMap;

	std::vector<CFactoryBase_Arg0*> m_FuncID_Arg0;
	FactoryMap	m_FuncName_Arg0;
};

class FactoryManager : public FactoryManagerBase, public Singleton<FactoryManager>
{
public:
	FactoryManager(){}
	virtual ~FactoryManager(){}
};