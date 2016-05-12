#pragma once
#include "FunctionBase.h"

class ParamPool;
class ParamBase;

typedef FunctionBase_Arg2< ParamPool*, ParamBase*> ParamCallBack;

class ParamPoolOwner
{
public:
	ParamPoolOwner(void);
	virtual ~ParamPoolOwner(void);

	bool CreateParamPool( int32 nTypeID );
	bool CreateParamPool( int32 nDefID, int32 nDataID);

	virtual void SetParamPool( ParamPool* pPool);
	virtual void BindParamPool(){}

	ParamPool* GetParamPool() { return m_pParamPool; }

protected:
	ParamPool*	m_pParamPool;
	int32		m_nParamTypeID;

	int32		m_nParamDefID;
	int32		m_nParamDataID;

	ParamCallBack*	m_pParamCallBackPost;
};

