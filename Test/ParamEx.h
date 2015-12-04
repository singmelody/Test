#pragma once

#include "ParamEx.h"
#include "MyClass.h"

#include "DBInterface.h"

class ParamPool;

enum ParamFlagType
{
	ePF_Server		= 1 << 0,
	ePF_ClientSelf	= 1 << 3,
	ePF_ClientAll	= 1 << 4,
	ePF_Save		= 1 << 5,
	ePF_Callback	= 1 << 8,
};

class ParamBase
{
public:
	ParamBase();
	virtual ~ParamBase();

	inline bool CheckFlag(uint32 flag) { return (m_flag & flag) != 0 ? true : false; }

	virtual bool ParamCompare( char* pParamBuff, char* pBuffer) = 0;

	int32 TypeID() { return m_typeid; }
	int32 Index(){ return m_index; }
	int32 ID() { return m_id; }
	const char* Name() { return m_name.c_str();}

	virtual int32 ParamSize() = 0;
protected:
	int32	m_typeid;

	int32	m_id;		// editor define id
	int32	m_index;	// after sort index
	uint32	m_flag;

	std::string m_name;
};

template <class T>
class Param : public ParamBase, public ClassMember<T>
{
public:
	void SetValue( void* pClassObj, T value)
	{
		if( !pClassObj)
			return;
	}

	inline void SetMaxValue(T value)
	{
		m_bMaxValue = true;
		m_maxValue = value;
	}

	inline void SetMinValue(T value)
	{
		m_bMaxValue = true;
		m_minValue = value;
	}

	virtual int32 ParamSize() { return ClassMember<T>::Size();}

	void GetStringValue( void* pClassObj, char* pBuffer, int32 nCount)
	{
		ClassMember<T>::Value2String( pClassObj, pBuffer, nCount);
	}

	bool ParamCompare( char* pParamBuff, char* pBuff)
	{
		return ClassMember<T>::Compare( pParamBuff, pBuff);
	}
protected:
	T m_defaultValue;

	bool m_bMaxValue;
	T m_maxValue;

	bool m_bMinValue;
	T m_minValue;

	std::string m_stringValue;
};


class Param_Int64 : public Param<int64>
{
public:
	Param_Int64();

	void LoadRowValue( void* pClassObj, DBRow& row, int32 nCol);
	bool SetValueString( ParamPool* pPool, char* pValue, bool bDirty);

};

class Param_Int32 : public Param<int32>
{
public:
	Param_Int32();

	void LoadRowValue( void* pClassObj, DBRow& row, int32 nCol);
	bool SetValueString( ParamPool* pPool, char* pValue, bool bDirty);

};

class Param_Int16 : public Param<int16>
{
public:
	Param_Int16();

	void LoadRowValue( void* pClassObj, DBRow& row, int32 nCol);
	bool SetValueString( ParamPool* pPool, char* pValue, bool bDirty);

};