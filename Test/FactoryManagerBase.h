#pragma once
#include <string>
#include <vector>
#include <map>
#include "Singleton.h"
#include "UtilID.h"
#include "BaseType.h"

#define FACTORY_NEWOBJ(className) (className*)FactoryManager::Instance().New(#className);
#define FACTORY_DELOBJ( pObj) { if(pObj) { FactoryManager::Instance().Delete( pObj); pObj = NULL; }}

class Allocator;

class FactoryBase_Arg0
{
public:
	FactoryBase_Arg0() : m_id(-1){}
	~FactoryBase_Arg0(){}

	virtual void ClassID(int32 nID) { m_id = nID; }
	int32 ClassID() { return m_id; }

	const char* ClassName() { return m_name.c_str(); }
	void ClassName(const char* pName)
	{
		if (!pName)
			return;

		m_name = pName;
	}

	virtual Allocator* GetAllocator() { return NULL; }

	virtual void* New() { return NULL; }
	virtual void  Free(void* ptr) {}

	virtual int32 ClassSize() { return 0; }
protected:
	int32		m_id;
	std::string m_name;
};

template < class T>
class Factory_Arg0 : public FactoryBase_Arg0
{
public:
	Factory_Arg0() : m_pAlloc(NULL){}
	Factory_Arg0(Allocator* pAlloc) : m_pAlloc(pAlloc) {
		Allocator* ptr = static_cast<Allocator*>(pAlloc);
		if (!ptr)
			return;
		ptr->Init(sizeof(T));
	}

	virtual void ClassID(int32 nID) { 
		m_id = nID; 

		if (!m_pAlloc)
			return;

		MemoryHead head;
		head.AllocInfo = nID;
		m_pAlloc->SetMemoryInfo(head);
	}
	int32 ClassID() { return m_id; }

	const char* ClassName() { return m_name.c_str(); }
	void ClassName(const char* pName)
	{
		if (!pName)
			return;

		m_name = pName;
	}

	virtual Allocator* GetAllocator() { return m_pAlloc; }


	virtual void* New()
	{
		if(!m_pAlloc)
			return NULL;

		T* p = (T*)m_pAlloc->TMalloc(sizeof(T));
		if(!p)
			return NULL;

		::new(p)T();

		return p;
	}
	virtual void Free(void* ptr)
	{
		if ( !ptr || !m_pAlloc || !m_pAlloc->IsValidMemory(ptr))
			return;

		m_pAlloc->TFree(ptr);
	}
protected:
	Allocator* m_pAlloc;
};


template <typename tArg>
class FactoryBase_Arg1
{
public:
	FactoryBase_Arg1() : m_id(-1){}
	~FactoryBase_Arg1(){}

	virtual void ClassID(int32 nID) { m_id = nID; }
	int32 ClassID() { return m_id; }

	const char* ClassName() { return m_name.c_str(); }
	void ClassName(const char* pName)
	{
		if (!pName)
			return;

		m_name = pName;
	}

	virtual void* New(tArg arg){ return NULL; }
	virtual void  Free(void* ptr) {}

	virtual Allocator* GetAllocator() { return NULL; }
protected:
	int32		m_id;
	std::string m_name;
};

template < class T, class tArg>
class Factory_Arg1 : public FactoryBase_Arg1<tArg>
{
public:
	Factory_Arg1() : m_pAlloc(NULL){}
	Factory_Arg1(Allocator* pAlloc) : m_pAlloc(pAlloc) {
		Allocator* ptr = static_cast<Allocator*>(pAlloc);
		if (!ptr)
			return;
		ptr->Init(sizeof(T));
	}

	virtual void ClassID(int32 nID) { 
		m_id = nID; 

		if (!m_pAlloc)
			return;

		MemoryHead head;
		head.AllocInfo = nID;
		m_pAlloc->SetMemoryInfo(head);
	}
	int32 ClassID() { return m_id; }

	const char* ClassName() { return m_name.c_str(); }
	void ClassName(const char* pName)
	{
		if (!pName)
			return;

		m_name = pName;
	}

	virtual Allocator* GetAllocator() { return m_pAlloc; }


	virtual void* New(tArg arg)
	{
		if(!m_pAlloc)
			return NULL;

		T* p = (T*)m_pAlloc->TMalloc(sizeof(T));
		if(!p)
			return NULL;

		::new(p)T(arg);

		return p;
	}
	virtual void Free(void* ptr)
	{
		if ( !ptr || !m_pAlloc || !m_pAlloc->IsValidMemory(ptr))
			return;

		m_pAlloc->TFree(ptr);
	}
protected:
	Allocator* m_pAlloc;
};


class FactoryManagerBase
{
public:
	FactoryManagerBase() {}
	virtual ~FactoryManagerBase() {}

	virtual void* New(int32 nClassID);
	virtual void* New(const char* pClassName);


	virtual void Delete(void* ptr);
	virtual void AddFactory(FactoryBase_Arg0* pFunc);

	int32 GetFactoryCount() { return (int32)m_FuncID_Arg0.size(); }
	FactoryBase_Arg0* GetFactory(int32 nIdx) { return m_FuncID_Arg0[nIdx]; }
protected:
	typedef std::map< UtilID, FactoryBase_Arg0*> FactoryMap;

	std::vector<FactoryBase_Arg0*> m_FuncID_Arg0;
	FactoryMap	m_FuncName_Arg0;
};

class FactoryManager : public FactoryManagerBase, public Singleton<FactoryManager>
{
public:
	FactoryManager(){}
	virtual ~FactoryManager(){}
};

template <typename tArg>
class FactoryManager_Arg1_Base
{
	typedef std::map< UtilID, FactoryBase_Arg1<tArg>* > FactoryArg1Map;
public:
	FactoryManager_Arg1_Base(){}
	virtual ~FactoryManager_Arg1_Base(){}

	virtual void* New(int32 nClassID, tArg arg)
	{
		if (nClassID < 0 || nClassID >= (int32)m_FuncID_Arg1.size())
			return NULL;

		FactoryBase_Arg1<tArg>* pFactory = m_FuncID_Arg1[nClassID];

		if (!pFactory)
			return NULL;

		return pFactory->New(arg);
	}

	virtual void* New(const char* pClassName, tArg arg)
	{
		if (!pClassName)
			return NULL;

		FactoryMap::iterator itr = m_FuncName_Arg1.find(pClassName);

		if (itr == m_FuncName_Arg1.end())
			return NULL;

		FactoryBase_Arg1<tArg>* pFactoryBase = itr->second;
		if (!pFactoryBase)
			return NULL;

		return pFactoryBase->New(arg);
	}

	virtual void Delete(void* ptr)
	{
		if (!ptr)
			return;

		int32 nInfo = ( (MemoryHead*)((char*)ptr-sizeof(MemoryHead)))->AllocInfo;

		if (nInfo < 0 || nInfo >= (int32)m_FuncID_Arg1.size())
			return;

		FactoryBase_Arg1<tArg>* pFactory = m_FuncID_Arg1[nInfo];
		if (!pFactory)
			return;

		pFactory->Free(ptr);
	}


	virtual void AddFactory(FactoryBase_Arg1<tArg>* pFunc)
	{
		if (!pFunc)
			return;

		m_FuncName_Arg1[pFunc->ClassName()] = pFunc;

		int32 nClassID = pFunc->ClassID();
		if (nClassID == -1)
		{
			pFunc->ClassID((int32)m_FuncID_Arg1.size());
			m_FuncID_Arg1.push_back(pFunc);
			return;
		}

		if (nClassID >= (int32)m_FuncID_Arg1.size())
			m_FuncID_Arg1.resize(nClassID+1);

		m_FuncID_Arg1[nClassID] = pFunc;
	}

	int32 GetFactoryCount() { return (int32)m_FuncID_Arg1.size(); }

protected:
	typedef std::map< UtilID, FactoryBase_Arg1<tArg>*> FactoryMap;

	std::vector<FactoryBase_Arg1<tArg>*> m_FuncID_Arg1;
	FactoryMap	m_FuncName_Arg1;
};

template <typename tArg>
class FactoryManager_Arg1 : public FactoryManager_Arg1_Base<tArg>, public Singleton<FactoryManager_Arg1<tArg> >
{
public:
	FactoryManager_Arg1(){}
	virtual ~FactoryManager_Arg1(){}
};