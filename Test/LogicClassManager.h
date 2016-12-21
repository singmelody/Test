#pragma once

#include <vector>
#include "MyClass.h"
#include "Singleton.h"

class ClassManagerBase
{
public:
	ClassManagerBase(){}
	virtual ~ClassManagerBase() {}

	MyClass* GetClass(const char* sName);
	MyClass* GetClass(int32 nIdx);
	void AddClass(MyClass* pClass);
	int32 GetClassCount() { return (int32)m_vClass.size(); }

	virtual void Print(const char* sFile);

protected:
	std::vector<MyClass*> m_vClass;
};

class LogicClassManager : public ClassManagerBase, public Singleton<LogicClassManager>
{
public:
	LogicClassManager(void);
	virtual ~LogicClassManager(void);
};

