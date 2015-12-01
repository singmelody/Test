#include "StdAfx.h"
#include "ParamEx.h"

ParamBase::ParamBase()
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
	int32 nValue = GetRowValue( row, col, int64(0));
	SetValue( pClassObj, nValue);
}

bool Param_Int64::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
{
	if(!pPool)
		return;

	int64 nValue = String::ToValue<int64>(pValue);
	pPool->SetValue<int64>( this, value, bDirty);
}

Param_Int32::Param_Int32()
{
	m_typeid = eTB_Int32;
}

void Param_Int32::LoadRowValue(void* pClassObj, DBRow& row, int32 nCol)
{
	int32 nValue = GetRowValue( row, col, int32(0));
	SetValue( pClassObj, nValue);
}

bool Param_Int32::SetValueString(ParamPool* pPool, char* pValue, bool bDirty)
{
	if(!pPool)
		return;

	int64 nValue = String::ToValue<int32>(pValue);
	pPool->SetValue<int64>( this, value, bDirty);
}