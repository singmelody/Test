#pragma once

#include "BaseType.h"
#include "UtilID.h"
#include <string>
#include <map>

enum TypeBaseFlag
{
	eTB_Flag_Sec	= 0x1 << 0,
	eTB_Flag_Print	= 0x1 << 1, 
};

enum TypeBasic
{
	eTB_Null = 0,
	eTB_Bool,
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

 	void InitType(bool) { m_classType = eTB_Bool; }
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

	inline int32 Size(){ return m_size; }
	inline void Size(int32 nSize) { m_size = nSize; }

	inline int32 ClassOffset() { return m_offset;}
	inline void ClassOffset(int32 nOffset) { m_offset = nOffset; }

	inline bool CheckTypeFlag(uint32 flag) { return m_typeFlag & flag; }

	virtual char* Read(void* pClassObj, char* pBuffer);
	virtual char* Read(void* pClassObj, char* pBuffer, bool& dirty);
	virtual char* Write(void* pClassObj, char* pBuffer);

	virtual const char* TypeName()
	{
		switch(m_classType)
		{
			case eTB_Bool: 
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

// 	T* GetValue()
// 	{
// 		return m_val;
// 	}
// 
// 	void SetValue(T& value)
// 	{
// 		m_val = value;
// 	}

protected:
	T m_val;
};

template <typename T>
class ClassMember : public TAnyType<T>
{
public:
	ClassMember(){}
	virtual ~ClassMember(){}

	T& GetValue(void* pClassObj)
	{
		return *((T*)((char*)pClassObj) + this->m_offset);
	}

	void SetValue( void* pClassObj, T value)
	{
		*((T*)((char*)pClassObj) + this->m_offset) = value;
	}

	bool Compare( char* pBuff1, char* pBuff2)
	{
		if(!pBuff1 || !pBuff2)
			return false;

		T value1 = *((T*)(pBuff1 + this->m_offset));
		T value2 = *((T*)(pBuff2 + this->m_offset));

		return value1 == value2;
	}
};

#define Ref_BaseClass_Begin(className) \
private: \
class _TClass_##className : public TClass \
{ \
public: \
	_TClass_##className(){} \
	_TClass_##className(){ \
		ClassName(#className); \
		TClassManager::Instance().AddClass(this); \
		className* pClassObj; pClassObj = NULL;

#define Ref_BaseClass_End(className) \
	}}; \
	static _TClass_##className mClass_##className; \
public:\
	static TClass* GetClassStatic() { return (TClass*)&mClass_##className; } \
	virtual TClass* GetClass() { return (TClass*)&mClass_##className; } \
	virtual char* Read(char* buffer, int32 nFlag = TypeBase_Flag_Ser) {return mClass_##className.Read( (void*)this, buffer, nFlag); } \
	virtual char* Write(char* buffer, int32 nFlag = TypeBase_Flag_Ser) { return mClass_##className.Write( (void*)this, buffer, nFlag); } \
private:\
	char* Read##className( char* buffer, int32 nFlag = TypeBase_Flag_Ser) { return mClass_##className.Read( (void*)this, buffer, nFlag); } \
	char* Write##className( char* buffer, int32 nFlag = TypeBase_Flag_Ser) { return mClass_##className.Write( (void*)this, buffer, nFlag); }

class MyClass
{
	typedef std::map<UtilID, TypeBase*> MemberMap;
public:
	MyClass();
	virtual ~MyClass();

	bool IsChild(int32 nClassID);
	bool IsChild(char* className);
	bool IsChild(MyClass* pClass);
	void AddMember( TypeBase* pType);
	inline MyClass* Parent() { return _parent; }
	inline void Parent(MyClass* pParent) { _parent = pParent; }
	
	inline const char* ClassName() { return mClassName.c_str(); }
	inline void ClassName(const char* sName)
	{
		if(!sName)
			return;

		mClassName = sName;
		nClassID = UtilID::CreateFromString(sName);
	}

	inline int32 ClassID() { return nClassID; }
	inline void ClassID(int32 classID) { nClassID = classID; }

	inline int32 GetClassSize() { return nClassSize; }
	inline void SetClassSize(int32 nSize) { nClassSize = nSize; }

	inline int32 GetClassSerSize() { return nClassSerSize; }
	inline void SetClassSerSize(int32 nSize) { nClassSerSize = nSize; }

	inline int32 MemberCount() { return (int32)mMemberMap.size(); }
	TypeBase* GetMember(int32 nIdx);
	TypeBase* GetMember(char* sName);

	char* Read( void* pClassObj, char* buffer, int32 nFlag = eTB_Flag_Sec);
	char* Write( void* pClassObj, char* buffer, int32 nFlag = eTB_Flag_Sec);
protected:
	int32 nClassID;
	int32 nClassSize;
	int32 nClassSerSize;

	MyClass*	_parent;
	MemberMap	mMemberMap;
	std::string	mClassName;
};
