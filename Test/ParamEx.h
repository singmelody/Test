#pragma once

#include "ParamEx.h"
#include "MyClass.h"
#include <sstream>
#include "DBInterface.h"

#define Param_STRING_SIZE 32

class ParamPool;

enum ParamFlagType
{
	ePF_Server		= 1 << 0,
	ePF_ClientSelf	= 1 << 3,
	ePF_ClientAll	= 1 << 4,
	ePF_Save		= 1 << 5,
	ePF_Callback	= 1 << 8,
};

enum TypeID
{
	eTypeID_NULL = 0,
	eTypeID_int64,
	eTypeID_uint64,
	eTypeID_int32,
	eTypeID_uint32,
	eTypeID_int16,
	eTypeID_uint16,
	eTypeID_int8,
	eTypeID_uint8,
	eTypeID_f64,
	eTypeID_f32,
	eTypeID_str
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
	void Index(int32 nIdx) { m_index = nIdx; }
	
	int32 ID() { return m_id; }
	void ID(int32 nID) { m_id = nID; }

	int32 CID() { return m_cid;}
	void CID(int32 nCID) { m_cid = nCID; }

	virtual void Offset(int32 nOffset) = 0;

	virtual const char* Name() = 0;
	virtual void Name(const char* pName) = 0;

	virtual int32 ParamSize() = 0;
	virtual void SetDftValue(void *pClasObj) = 0;

	virtual const char* GetValueString(void* pClassObj) = 0;

	virtual char* ParamRead( char* pParamBuff, char* pBuff, bool& bDirty) = 0;
	virtual char* ParamWrite( char* pParamBuff, char* pBuff) = 0;

	inline bool SqlFlag_IsInsertProcParam() const { return ( m_mysqlFlag & (uint32)eSqlFlag_NotInsertProcParam) == 0; }

	enum SqlFlag
	{
		eSqlFlag_PrimaryKey				= 1 << 0,
		eSqlFlag_AutoIncrement			= 1 << 1,
		eSqlFlag_NotInsertProcParam		= 1 << 2,
		eSqlFlag_NotUpdateProcParam		= 1 << 3,
	};
protected:
	int32	m_typeid;

	int32	m_id;		// editor define id
	int32	m_index;	// after sort index
	int32	m_cid;		// util id
	uint32	m_flag;
	uint32	m_mysqlFlag;
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

	virtual const char* Name() { return m_name.c_str(); }
	virtual void Name(const char* pName) { m_name = pName; }

	virtual int32 ParamSize() { return ClassMember<T>::Size();}

	virtual void SetDftValue(void *pClasObj)
	{
		ClassMember<T>::SetValue( pClasObj, m_defaultValue);
	}

	inline void SetDftValue(T val) { m_defaultValue = val; }

	virtual void Offset(int32 nOffset)
	{
		ClassMember<T>::ClassOffset(nOffset);
	}

	void GetStringValue( void* pClassObj, char* pBuffer, int32 nCount)
	{
		ClassMember<T>::Value2String( pClassObj, pBuffer, nCount);
	}

	virtual const char* GetValueString( void* pClassObj)
	{
		T value = ClassMember<T>::GetValue( pClassObj );
		std::stringstream ss;
		ss << value;
		m_stringValue = ss.str();
		return m_stringValue.c_str();
	}

	bool ParamCompare( char* pParamBuff, char* pBuff)
	{
		return ClassMember<T>::Compare( pParamBuff, pBuff);
	}

	virtual char* ParamRead( char* pParamBuff, char* pBuff, bool& bDirty)
	{
		return ClassMember<T>::Read( pParamBuff, pBuff, bDirty);
	}

	virtual char* ParamWrite( char* pParamBuff, char* pBuff)
	{
		return ClassMember<T>::Write( pParamBuff, pBuff);
	}

protected:
	T m_defaultValue;

	bool m_bMaxValue;
	T m_maxValue;

	bool m_bMinValue;
	T m_minValue;

	std::string m_name;

	std::string m_stringValue;
};


class Param_int64 : public Param<int64>
{
public:
	Param_int64();

	virtual void LoadRowValue( void* pClassObj, DBRow& row, int32 nCol);
	virtual bool SetValueString( ParamPool* pPool, char* pValue, bool bDirty);
};

class Param_int32 : public Param<int32>
{
public:
	Param_int32();

	virtual void LoadRowValue(void* pClassObj, DBRow& row, int32 nCol);
	virtual bool SetValueString( ParamPool* pPool, char* pValue, bool bDirty);
};

// template <class T>
// class Param_Type : public Param<T>
// {
// public:
// 	Param_Type() { SetParamType(); }
// 
// 	void LoadRowValue( void* pClassObj, DBRow& row, int32 nCol);
// 	bool SetValueString( ParamPool* pPool, char* pValue, bool bDirty);
// 
// 	void SetParamType();
// };

// template <class T>
// void Param_Type<T>::LoadRowValue(void* pClassObj, DBRow& row, int32 nCol)
// {
// 	T nValue;
// 	row.Fill( nValue, nCol, T(0));
// 	SetValue( pClassObj, nValue);
// }
// 
// template <class T>
// bool Param_Type<T>::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
// {
// 	if(!pPool)
// 		return false;
// 	
// 	T nValue = MyString::ToValue<T>(pValue);
// 	pPool->SetValue<T>( this, nValue, bDirty);
// 	
// 	return true;
// }
// 
// template <class T>
// void Param_Type<T>::SetParamType()
// {
// 	m_typeid = eTB_Null;
// }
// 
// template <>
// void Param_Type<int64>::SetParamType()
// {
// 	m_typeid = eTB_Int64;
// }
// 
// template <>
// void Param_Type<uint64>::SetParamType()
// {
// 	m_typeid = eTB_UInt64;
// 	
// }
// 
// 
// template <>
// void Param_Type<int32>::SetParamType()
// {
// 	m_typeid = eTB_Int32;
// }
// 
// 
// template <>
// void Param_Type<uint32>::SetParamType()
// {
// 	m_typeid = eTB_UInt32;	
// }
// 
// 
// template <>
// void Param_Type<int16>::SetParamType()
// {
// 	m_typeid = eTB_Int16;
// }
// 
// 
// template <>
// void Param_Type<uint16>::SetParamType()
// {
// 	m_typeid = eTB_UInt16;	
// }
// 
// 
// template <>
// void Param_Type<int8>::SetParamType()
// {
// 	m_typeid = eTB_Int8;	
// }
// 
// 
// template <>
// void Param_Type<uint8>::SetParamType()
// {
// 	m_typeid = eTB_UInt8;	
// }
// 
// template <>
// void Param_Type<f64>::SetParamType()
// {
// 	m_typeid = eTB_F64;	
// }
// 
// template <>
// void Param_Type<f32>::SetParamType()
// {
// 	m_typeid = eTB_F32;	
// }


class ParamStr : public ParamBase, public ClassMember_Char
{
public:
	ParamStr()
		: ClassMember_Char(Param_STRING_SIZE)
	{
		ClassMember_Char::AddTypeFlag(eTB_Flag_Sec);
		m_typeid = eTypeID_str;
	}

	ParamStr(int32 nSize) : ClassMember_Char(nSize)
	{
		ClassMember_Char::AddTypeFlag(eTB_Flag_Sec);
		m_typeid = eTypeID_str;
	}

	const char* TypeName() { return "string"; }

	virtual int32 GetTypeInfo()
	{
		return ClassMember_Char::GetTypeInfo();
	}

	virtual const char* GetTypeName()
	{
		return ClassMember_Char::TypeName();
	}

	void Offset(int32 nOffset)
	{
		ClassMember_Char::ClassOffset(nOffset);
	}

	int32 Offset() { return ClassMember_Char::ClassOffset(); }

	char* ParamRead(char* pParamBuff, char* pBuffer, bool& bDirty)
	{
		return ClassMember_Char::Read( pParamBuff, pBuffer, bDirty);
	}

	char* ParamWrite(char* pParamBuff, char* pBuffer)
	{
		return ClassMember_Char::Write( pParamBuff, pBuffer);
	}

	bool ParamCompare(char* pParamBuff, char* pBuff)
	{
		return false;
	}

	const char* Name() { return ClassMember_Char::Name(); }
	void Name(const char* sName) { return ClassMember_Char::Name(sName); }

	int32 ParamSize() { return ClassMember_Char::Size(); }

	void SetValue( void* pClassObj, char* pValue)
	{
		ClassMember_Char::SetValue( pClassObj, pValue);
	}

	void SetValue(void* pClassObj, const char* pValue)
	{
		ClassMember_Char::SetValue( pClassObj, pValue);
	}

	void GetStringValue(void* pClassObj, char* pBuffer, size_t size)
	{
		char* tmpBuffer = ClassMember_Char::GetValue(pClassObj);
		int32 nCopySize = ((m_charSize -1) > (int32)size) ? (int32)size : m_charSize -1;
		memcpy( pBuffer, tmpBuffer, nCopySize);
	}

	virtual void LoadRowValue( void* pClassObj, DBRow& row, int32 nCol);
	virtual const char* GetValueString(void* pClassObj);
	virtual bool SetValueString( ParamPool* , char* pValue, bool bDirty);
};