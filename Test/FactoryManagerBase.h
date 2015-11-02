#pragma once
#include <string>
#include <vector>
#include <map>
#include "Singleton.h"
#include "UtilID.h"

#define FACTORY_NEWOBJ(className) (className*)CFactoryManagerBase::Instance().New(#className);
#define FACTORY_DELOBJ( pObj, id) { if(pObj) { CFactoryManagerBase::Instance().Delete( pObj, id); pObj = NULL; }}

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

	virtual void* New();
	virtual void Free(void* ptr);
protected:
	int32		m_id;
	std::string m_name;
};

class CFactoryManagerBase : public Singleton < CFactoryManagerBase >
{
public:
	CFactoryManagerBase() {}
	virtual ~CFactoryManagerBase() {}

	virtual void* New(int32 nClassID);
	virtual void* New(const char* pClassName);

	virtual void Delete(void* ptr, int32 id);

	virtual void AddFactory(CFactoryBase_Arg0* pFunc);
	virtual void AddFactoryEx(CFactoryBase_Arg0* pFunc);

	int32 GetFactoryCount() { return (int32)m_FuncID_A }

protected:
	typedef std::map< UtilID, CFactoryBase_Arg0*> FactoryMap;

	std::vector<CFactoryBase_Arg0*> m_FuncID_Arg0;
};
