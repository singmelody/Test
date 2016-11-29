#pragma once

#include "BaseType.h"
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

	inline int32 GetParamTypeID() const { return m_nParamTypeID; }
	inline void SetParamTypeID(int32 nTypeID){ m_nParamTypeID = nTypeID; }

	inline int32 GetParamDefID() const { return m_nParamDefID; }
	inline void SetParamDefID(int32 nDefID){ m_nParamDefID = nDefID; }

	inline int32 GetParamDataID() const { return m_nParamDataID; }
	inline void SetParamDataID(int32 nDataID){ m_nParamDataID = nDataID; }
protected:
	ParamPool*	m_pParamPool;
	int32		m_nParamTypeID;

	int32		m_nParamDefID;
	int32		m_nParamDataID;

	ParamCallBack*	m_pParamCallBackPost;
};

