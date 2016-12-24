#include "StdAfx.h"
#include "AISerializeBase.h"

Ref_Class(AISerializeBase);
AISerializeBase::AISerializeBase(void)
{
}


AISerializeBase::~AISerializeBase(void)
{
}

void AISerializeBase::SetMemberValue(const char* sString)
{
	if(!sString)
		return;

	const int32 nBuffer_Size = 512;
	char paramStr[BUFF_SIZE];
	memset( paramStr, 0, BUFF_SIZE);
	strncpy( paramStr, sString, BUFF_SIZE);

	std::string sParam, sName, sValue;
	char* t = paramStr;

	while (ParamString::GetParamString( t, sParam))
	{
		char tmpStr[512];
		memset( tmpStr, 0, 512);
		memcpy( tmpStr, sParam.c_str(), sParam.length());
		char* tx = tmpStr;
		if()
	}
}

void AISerializeBase::SetMemberValue(const char* sName, const char* sValue)
{
	MyClass* pClass = GetClass();
	if(!pClass)
		return;

	TypeBase* pMember = pClass->GetMember((char*)sName);
	if(!pMember)
		return;

	if(pMember->CheckType(eTB_Int16))
	{
		int16 nValue = (int16)atoi(sValue);
		ClassMember<int16>* pCM = static_cast<ClassMember<int16>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_UInt16))
	{
		uint16 nValue = (uint16)atoi(sValue);
		ClassMember<uint16>* pCM = static_cast<ClassMember<uint16>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_Int32))
	{
		int32 nValue = (int32)atoi(sValue);
		ClassMember<int32>* pCM = static_cast<ClassMember<int32>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_UInt32))
	{
		uint32 nValue = (uint32)atoi(sValue);
		ClassMember<uint32>* pCM = static_cast<ClassMember<uint32>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_Int64))
	{
		int64 nValue = (int64)atoi(sValue);
		ClassMember<int64>* pCM = static_cast<ClassMember<int64>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_UInt64))
	{
		uint64 nValue = (uint64)atoi(sValue);
		ClassMember<uint64>* pCM = static_cast<ClassMember<uint64>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_F32))
	{
		f32 nValue = (f32)atoi(sValue);
		ClassMember<f32>* pCM = static_cast<ClassMember<f32>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_F64))
	{
		f64 nValue = (f64)atoi(sValue);
		ClassMember<f64>* pCM = static_cast<ClassMember<f64>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_F64))
	{
		f64 nValue = (f64)atoi(sValue);
		ClassMember<f64>* pCM = static_cast<ClassMember<f64>*>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
	else if(pMember->CheckType(eTB_String))
	{
		ClassMember_Char* pCM = static_cast<ClassMember_Char>(pMember);
		if(pCM)
			pCM->SetValue( this, nValue);
	}
}