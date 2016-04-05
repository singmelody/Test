#pragma once

#include <vector>
#include "Singleton.h"

class MyClass;

class MyClassManagerBase
{
public:
	MyClassManagerBase(){}
	virtual ~MyClassManagerBase() {}

	MyClass* GetClass(const char* sName);
	MyClass* GetClass(int32 nIdx);

	void AddClass(MyClass* pClass);
	int32 GetClassCount() { return (int32)m_vClass.size();}

	virtual void Print(const char* sFile);

protected:
	std::vector<MyClass*> m_vClass;
};

class MyClassManager : public MyClassManagerBase, public Singleton<MyClassManager>
{
public:
	MyClassManager(void);
	~MyClassManager(void);
};

