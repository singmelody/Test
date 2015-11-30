#pragma once

#include "BaseType.h"
#include <string>

enum TypeBaseFlag
{
	eTB_Flag_Sec	= 0x1 << 0,
	eTB_Flag_Print	= 0x1 << 1, 
};

enum TypeBasic
{
	eTB_bool = 0,
	eTB_UInt8,
	eTB_Int8,
	eTB_UInt16,
	eTB_Int16,
	eTB_UInt32,
	eTB_Int32,
	eTB_UInt64,
	eTB_Int64,
	eTB_F32,
	eTB_F64,
	eTB_Char,
	eTB_String,
	eTB_Class,
};


class TypeBase
{
public:
	TypeBase()
		: pPtr(NULL)
		, m_size(0)
		, m_offset(0)
		, m_typeFlag(eTB_Flag_Sec | eTB_Flag_Print)
	{

	}

	virtual ~TypeBase(){}

	inline const char* Name() { return mName.c_str(); }

 	void InitType(bool) { m_classType = eTB_bool; }
  	void InitType(uint8) { m_classType = eTB_UInt8; }
	void InitType(int8) { m_classType = eTB_Int8; }
	void InitType(uint16) { m_classType = eTB_UInt16; }
	void InitType(int16) { m_classType = eTB_Int16; }
	void InitType(uint32) { m_classType = eTB_UInt32; }
	void InitType(int32) { m_classType = eTB_Int32; }
	void InitType(uint64) { m_classType = eTB_UInt64; }
	void InitType(int64) { m_classType = eTB_Int64; }
	void InitType(f32) { m_classType = eTB_F32; }
	void InitType(f64) { m_classType = eTB_F64; }
	void InitType(char) { m_classType = eTB_Char; }
	void InitType(char*) { m_classType = eTB_String; }



	virtual const char* TypeName()
	{
		switch(m_classType)
		{
			case eTB_bool: 
				return "bool";
			case eTB_UInt8: 
				return "uint8";
			case eTB_Int8: 
				return "int8";
			case eTB_UInt16: 
				return "uint16";
			case eTB_Int16: 
				return "int16";
			case eTB_UInt32: 
				return "uint32";
			case eTB_Int32: 
				return "int32";
			case eTB_F32: 
				return "f32";
			case eTB_F64: 
				return "f64";
			case eTB_Char: 
				return "char";
			case eTB_String: 
				return "String";
			case eTB_Class: 
				return "Class";
		}

		return "";
	}

protected:
	void*	pPtr;
	int32	m_size;
	int32	m_offset;
	uint32	m_typeFlag;
	int32	m_classType;

	std::string mName;
	std::string mDes;
};

template <typename T>
class TAnyType : public TypeBase
{
public:
	TAnyType()
	{
		Size( sizeof(T));
		InitType(m_val);
	}

	virtual ~TAnyType(){}

	virtual const char* TypeName()
	{
		return TypeBase::TypeName();
	}

	T* GetValue()
	{
		return m_val;
	}

	void SetValue(T& value)
	{
		m_val = value;
	}

protected:
	T m_val;
};

template <typename T>
class ClassMember : public TAnyType<T>
{

};
