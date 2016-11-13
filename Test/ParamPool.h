#pragma once
#include "ParamEx.h"
#include "FunctionBase.h"
#include "ParamDebugHelper.h"
#include "ParamNameIndexHelper.h"
#include "ParamDef.h"
#include <assert.h>
#include <bitset>

#define PARAM_ID(typeid) (typeid >> 24)
#define PARAM_DATA_ID(typeid) (typeid & 0x00ffffff)

#define MAX_PARAM_COUNT_IN_POOL 1024
#define MAX_PARAM_COUNT_IN_BLOCK (sizeof(uint32)*8)

#define PARAM_GET_VALUE( paramPtr, paramName, defVal) \
	paramPtr->GetValue( ParamNameIndexHelper::GetParamIndex( paramPtr->GetParamDefineIndex(), param_name_##paramName), defVal)

#define PARAM_GET_STRING( paramPtr, paramName, defVal ) \
	paramPtr->GetValueString( ParamNameIndexHelper::GetParamIndex( paramPtr->GetParamDefineIndex(), param_name_##paramName), defVal);

#define PARAM_SET_VALUE( paramPtr, paramName, Val, dirty) \
	paramPtr->SetValue( ParamNameIndexHelper::GetParamIndex( paramPtr->GetParamDefineIndex(), param_name_##paramName), Val, dirty);

#define PARAM_SET_STRING( paramPtr, paramName, Val, dirty ) \
	paramPtr->SetValueString( ParamNameIndexHelper::GetParamIndex( paramPtr->GetParamDefineIndex(), param_name_##paramName), Val, dirty);

typedef FunctionBase_Arg2<ParamPool*, ParamBase*> ParamCallback;

class ParamDef;
class DataBufferBase;

class ParamPool
{
public:
	class ParamStreamInfo
	{
	public:
		uint32	nBlockFlag;
		uint8	nBlockIdx;
		static int32 Size() { return sizeof(uint32) + sizeof(uint8); }
	};

	ParamPool(void);
	~ParamPool(void);

	template <class T>
	void SetValue( int32 nIdx, T val, bool bDirty = true);

	template <class T>
	void SetValue( const char* sName, T val, bool bDirty = true);

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

	const char* GetValueString( int32 nIdx, const char* defVal);
	const char* GetValueString( const char* sName, const char* defVal);
	const char* GetValueString( ParamBase* pParam, const char* defVal);


	void PreSetValue( ParamBase* pBase, bool bSaveOldValue);
	void PostSetValue( ParamBase* pBase );

	bool InitParamPool( int32 nParamID, int32 nDataID, char* pBuff = NULL);
	bool InitParamPool( ParamDef* pDef, int32 nDataID, char* pBuff = NULL);
	bool InitParamPool( int32 nDataID );

	void NodeServer_WriteSHM_AvatarDID(int64 nAvatarDID);
	void NodeServer_WriteSHM_CommonDataIdx(int32 nCommonDataIdx);
	void NodeServer_WriteSHM_CommonDataFlag(int32 nCommonDataFlag);

	void SaveOldValue( ParamBase* pBase);

	virtual void SetAllDefault() = 0;
	virtual bool DefaultCheck(int32 nParamIdx) = 0;
	virtual void SetDefault( int32 nParamIdx, bool bDefault) = 0;

	void SetParamDefine(ParamDef* pDef);
	void SetParamBuffer(char* pBuff);
	bool AllocParamBuffer();

	int32 GetDataID() { return m_initDataID; }
	void SetDataID(int32 nID) { m_initDataID = nID; }

	ParamDef* GetParamDef() const;

	int32 GetParamDefineIndex();
	int32 GetParamTypeID();

	virtual bool ParamDirtyCheck( int32 nParamIdx) = 0;
	virtual void ClearParamDirty( int32 nParamIdx) = 0;
	virtual void SetParamDirty( int32 nParamIdx) = 0;

	virtual bool DirtyCheck() = 0;
	virtual void ClearDirty() = 0;
	virtual void SetAllParamDirty() = 0;

	void* SHMGetExtraMemory();

	char* Read( char* pBuffer, bool bDirty = false);
	bool Write( char* pBuffer, int32& nBufferSize, int32& nStartParamPos, uint32 paramFlag, uint32 nSyncFlag, uint32 nParamFlagExclude = 0);
protected:
	char* Data2Buffer(char* pBuffer);
	char* Buffer2Data(char* pBuffer);

	template <class T>
	T GetValueFromBuff(ParamBase* pBase, char* pBuffer);

	void UpdateParambit( ParamBase* pBase, bool bUpdateDirtyBit);

	int32			m_initDataID;
	int32			m_paramBuffSize;
	int32			m_paramBlockCount;

	bool			m_bUserBuff;
	char*			m_pParamBuffer;
	char*			m_pTemplateBuffer;

	ParamDebugUion	m_debugBuffer;

	ParamDef*		m_pDef;

	ParamCallback*	m_callback;


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

	virtual void SetParamDirty(int32 nParamIdx) { m_dirtyBits.set(nParamIdx); }
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


class ParamSet : public ParamPoolEx<1024>
{
	DECLARE_FACTORY_ARG0( ParamSet, -1, new PoolAllocator);
public:
	static ParamPool* CreateNew( int32 nParamType, char* pBuff = 0);
	static ParamPool* CreateNew( int32 nDefID, int32 nDataID, char* pBuff = 0);
};

class ParamSetEx : public ParamPoolEx<1024>
{
	DECLARE_FACTORY_ARG0( ParamSet, -1, new PoolAllocatorEx);
public:
	static ParamPool* CreateNew( int32 nParamType, char* pBuff = 0);
	static ParamPool* CreateNew( int32 nDefID, int32 nDataID, char* pBuff = 0);
};

class ParamSyncCounter : public Singleton<ParamSyncCounter>
{
	typedef std::map<std::string, int32> ParamCountMap;
public:
	ParamSyncCounter(){}
	~ParamSyncCounter(){}

	void AddCount(const char* sName, int32 flag);
	void Clear();
	void Print();

	ParamCountMap	m_CltSelfMap;
	ParamCountMap	m_CltAllMap;
};

template <class T>
void ParamPool::SetValue(int32 nIdx, T val, bool bDirty)
{
	if(!m_pDef)
		return;

	ParamBase* pBase = m_pDef->GetParam(nIdx);
	SetValue<T>( pBase, val, bDirty);
}

template <class T>
void ParamPool::SetValue(const char* sName, T val, bool bDirty)
{
	if(!m_pDef)
		return;

	ParamBase* pBase = m_pDef->GetParam(sName);
	SetValue<T>( pBase, val, bDirty);
}

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

	switch(pBase->TypeID())
	{
	case eTB_Int16:
		{
			Param_Type<int16>* pVal = (Param_Type<int16>*)(pBase);
			pVal->SetValue( m_pParamBuffer, (int16)val);
			break;
		}
	case eTB_Int32:
		{
			Param_Type<int32>* pVal = (Param_Type<int32>*)(pBase);
			pVal->SetValue( m_pParamBuffer, (int32)val);
			break;
		}
	case eTB_Int64:
		{
			Param_Type<int64>* pVal = (Param_Type<int64>*)(pBase);
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
	if(!m_pDef)
		return defVal;

	ParamBase* pBase = m_pDef->GetParam(nIdx);
	return GetValue<T>( pBase, defVal);
}

template <class T>
T ParamPool::GetValue(const char* sName, T defVal)
{
	if(!m_pDef)
		return defVal;

	ParamBase* pBase = m_pDef->GetParam(sName);
	return GetValue<T>( pBase, defVal);
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
	switch( pBase->TypeID() )
	{
	case eTB_Int16:
		{
			Param_Type<int16>* pVal = (Param_Type<int16>*)(pBase);
			int16 nValue = pVal->GetValue(pBuffer);
			return (T)nValue;
		}
	case eTB_Int32:
		{
			Param_Type<int32>* pVal = (Param_Type<int32>*)(pBase);
			int32 nValue = pVal->GetValue(pBuffer);
			return (T)nValue;
		}
	case eTB_Int64:
		{
			Param_Type<int64>* pVal = (Param_Type<int64>*)(pBase);
			int64 nValue = pVal->GetValue(pBuffer);
			return (T)nValue;
		}
	}

	Param<T>* pVal = (Param<T>*)(pBase);
	return (T)pVal->GetValue(pBuffer);
}
