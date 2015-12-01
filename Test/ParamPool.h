#pragma once
#include "ParamEx.h"
#include "FunctionBase.h"
#include <assert.h>
#include <bitset>

typedef FunctionBase_Arg2<ParamPool*, ParamBase*> ParamCallback;

class ParamDef;

class ParamPool
{
public:
	ParamPool(void);
	~ParamPool(void);

	template <class T>
	void SetValue( ParamBase* pBase, T val, bool bDirty);

	template <class T>
	T GetValue( int32 nIdx, T defVal);

	template <class T>
	T GetValue( const char* sName, T defVal);

	template <class T>
	T GetValue( ParamBase* pBase, T defVal);

	template <class T>
	T GetValue( ParamBase* pBase );

	void PreSetValue( ParamBase* pBase, bool bSaveOldValue);
	void PostSetValue( ParamBase* pBase );

	void SaveOldValue( ParamBase* pBase);

	virtual bool ParamDirtyCheck(int32 nParamIdx) = 0;
	virtual void ClearParamDirty(int32 nParamIdx) = 0;
	virtual bool DirtyCheck() = 0;
	virtual void ClearDirty() = 0;
	virtual void SetAllParamDirty() = 0;
	virtual void SetParamDirty(int32 nParamIdx) = 0;

	virtual void SetAllDefault() = 0;
	virtual bool DefaultCheck(int32 nParamIdx) = 0;
	virtual void SetDefault( int32 nParamIdx, bool bDefault) = 0;
protected:
	template <class T>
	T GetValueFromBuff(ParamBase* pBase, char* pBuffer);

	int32			m_initDataID;
	int32			m_paramBuffSize;

	char*			m_pParamBuffer;
	char*			m_pTemplateBuffer;

	ParamDef*		m_pDef;

	ParamCallback*	m_callback;

	void UpdateParambit( ParamBase* pBase, bool bUpdateDirtyBit);

	uint8			m_oldDataBuff[8];
};

template <int32 MAXINDEX>
class ParamPoolEx : public ParamPool
{
public:
	ParamPoolEx()
	{
		m_dirtyBits.reset();
		m_defaultBits.reset();
	}

	virtual ~ParamPoolEx(){}

	virtual bool ParamDirtyCheck(int32 nParamIdx) { return m_dirtyBits.test(nParamIdx); }
	virtual void ClearParamDirty(int32 nParamIdx) { m_dirtyBits.reset(nParamIdx); }

	virtual bool DirtyCheck()
	{
		return m_dirtyBits.any();
	}

	virtual void ClearDirty()
	{
		m_dirtyBits.reset();
	}

	virtual void SetAllParamDirty()
	{
		m_dirtyBits.set();
	}

	virtual void SetParamDirty(int32 nParamIdx)
	{
		m_dirtyBits.set(nParamIdx);
	}

	virtual void SetAllDefault() { m_defaultBits.set(); }

	virtual bool DefaultCheck(int32 nParamIdx) { return m_defaultBits.test(nParamIdx); }

	virtual void SetDefault( int32 nParamIdx, bool bDefault)
	{
		m_defaultBits.set( nParamIdx, bDefault);
	}
protected:
	std::bitset<MAXINDEX> m_defaultBits;
	std::bitset<MAXINDEX> m_dirtyBits;
};

template <class T>
void ParamPool::SetValue( ParamBase* pBase, T val, bool bDirty)
{
	if(!pBase)
		return;

	assert( IsValidMemory( m_pParamBuffer) );

	T curVal = GetValue<T>( pBase );

	if( curVal == val )
		return;

	PreSetValue( pBase, false);

	switch(pBase->m_typeid)
	{
	case eTB_Int16:
		{
			Param_Int16* pVal = (Param_Int16*)(pBase);
			pVal->SetValue( m_pParamBuffer, (int16)val);
			break;
		}
	case eTB_Int32:
		{
			Param_Int32* pVal = (Param_Int32*)(pBase);
			pVal->SetValue( m_pParamBuffer, (int64)val);
			break;
		}
	case eTB_Int64:
		{
			Param_Int64* pVal = (Param_Int64*)(pBase);
			pVal->SetValue( m_pParamBuffer, (int64)val);
			break;
		}
	}

	PostSetValue(pBase);
	UpdateParambit( pBase, bDirty);
}


template <class T>
T ParamPool::GetValue(int32 nIdx, T defVal)
{

}


template <class T>
T ParamPool::GetValue(const char* sName, T defVal)
{

}

template <class T>
T ParamPool::GetValue( ParamBase* pBase, T defVal)
{
	if(!pBase)
		return defVal;

	return GetValue<T>(pBase);
}

template <class T>
T ParamPool::GetValue(ParamBase* pBase)
{
	return GetValueFromBuff<T>( pBase, m_pParamBuffer);
}

template <class T>
T ParamPool::GetValueFromBuff(ParamBase* pBase, char* pBuffer)
{
	switch( pBase->GetTypeID() )
	{
	case eTB_Int16:
		{
			Param_Int16* pVal = (Param_Int16*)(pBase);
			int16 nValue = pVal->GetValue(pBuffer);
			return (T)nValue;
		}
	case eTB_Int32:
		{
			Param_Int32* pVal = (Param_Int32*)(pBase);
			int32 nValue = pVal->GetValue(pBuffer);
			return (T)nValue;
		}
	case eTB_Int64:
		{
			Param_Int64* pVal = (Param_Int64*)(pBase);
			int64 nValue = pVal->GetValue(pBuffer);
			return (T)nValue;
		}
	}

	Param<T>* pVal = (Param<T>*)(pBase);
	return (T)pVal->GetValue(pBuffer);
}
