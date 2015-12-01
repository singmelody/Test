#include "StdAfx.h"
#include "ParamEx.h"
#include "MyString.h"
#include "ParamPool.h"

ParamBase::ParamBase()
	: m_index(-1)
	, m_id(-1)
	, m_flag(ePF_Server | ePF_Save)
	, m_typeid(eTB_Null)
{

}

ParamBase::~ParamBase()
{

}

Param_Int64::Param_Int64()
{
	m_typeid = eTB_Int64;
}

void Param_Int64::LoadRowValue(void* pClassObj, DBRow& row, int32 nCol)
{
	int64 nValue;
	row.Fill( nValue, nCol, int64(0));
	SetValue( pClassObj, nValue);
}

bool Param_Int64::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
{
	if(!pPool)
		return false;

	int64 nValue = MyString::ToValue<int64>(pValue);
	pPool->SetValue<int64>( this, pValue, bDirty);
}

Param_Int32::Param_Int32()
{
	m_typeid = eTB_Int32;
}

void Param_Int32::LoadRowValue(void* pClassObj, DBRow& row, int32 nCol)
{
	int32 nValue;
	row.Fill( nValue, nCol, int32(0));
	SetValue( pClassObj, nValue);
}

bool Param_Int32::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
{
	if(!pPool)
		return;

	int64 nValue = MyString::ToValue<int32>(pValue);
	pPool->SetValue<int32>( this, pValue, bDirty);
}

Param_Int16::Param_Int16()
{
	m_typeid = eTB_Int32;
}

void Param_Int16::LoadRowValue(void* pClassObj, DBRow& row, int32 nCol)
{
	int32 nValue;
	row.Fill( nValue, nCol, int16(0));

	SetValue( pClassObj, nValue);
}

bool Param_Int16::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
{
	if(!pPool)
		return;

	int64 nValue = MyString::ToValue<int32>(pValue);
	pPool->SetValue<int16>( this, pValue, bDirty);
}