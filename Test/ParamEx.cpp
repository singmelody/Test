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

Param_int64::Param_int64()
{
	m_typeid = eTB_Int64;
}

void Param_int64::LoadRowValue(void* pClassObj, DBRow& row, int32 nCol)
{
	int64 nValue = GetRowValue( row, nCol, int64(0));
	SetValue( pClassObj, nValue);
}

bool Param_int64::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
{
	if(!pPool)
		return false;

	int64 nValue = MyString::ToValue<int64>(pValue);
	pPool->SetValue<int64>( this, nValue, bDirty);

	return true;
}

Param_int32::Param_int32()
{
	m_typeid = eTB_Int32;
}

void Param_int32::LoadRowValue(void* pClassObj, DBRow& row, int32 nCol)
{
	int32 nValue = GetRowValue( row, nCol, int32(0));
	SetValue( pClassObj, nValue);
}

bool Param_int32::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
{
	if(!pPool)
		return false;

	int64 nValue = MyString::ToValue<int32>(pValue);
	pPool->SetValue<int32>( this, nValue, bDirty);

	return true;
}
