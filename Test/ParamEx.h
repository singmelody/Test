#pragma once

#include "ParamEx.h"
#include "MyClass.h"

class ParamBase
{
public:
	ParamBase();
	virtual ~ParamBase();

protected:
	int32 m_typeid;
};

template <class T>
class Param : public ParamBase, public ClassMember<T>
{
	
};

class Param_Int64 : public Param<int64>
{
public:
	Param_Int64();

	void LoadRowValue( void* pClassObj, DBRow& row, int32 nCol);
	bool SetValueString( ParamPool* pPool, char* pValue, bool bDirty);

};