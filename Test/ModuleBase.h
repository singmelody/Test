#pragma once

#include <list>
#include "ServerGrpInfo.h"
#include "ServerManager.h"

class ParamPool;
class LoadTemplateManager;
class ServerManager;

#define DOG_SET_VALUE(paramPtr, paramName, Val) \
	paramPtr->SetValue(ParamNameIndexHelper::GetParamIndex(paramPtr->GetParamDefineIndex(), param_name_##paramName), Val, true);

#define DOG_SET_STRING(paramPtr, paramName, Val) \
	paramPtr->SetValueString(ParamNameIndexHelper::GetParamIndex(paramPtr->GetParamDefineIndex(), param_name_##paramName), Val, true);

#define MODULEDOG_SET_VALUE( paramName, Val) DOG_SET_VALUE( m_pModuleDogPool, paramName, Val)
#define MODULEDOG_SET_STRING( paramName, Val) DOG_SET_STRING( m_pModuleDogPool, paramName, Val);

class ModuleBase
{
public:
	ModuleBase(SrvType nSrvType);
	virtual ~ModuleBase(void);

	virtual bool Init(int32 nArgc, char* argv[]);
	virtual void Exit();
	virtual void OnConfigLoaded();
	virtual void AppendLoadTemplate();

	virtual bool StartMainLoop( int32 nFrameInterval);
	virtual bool CreateDogPool();

	virtual void Broadcast2DogPool();
	virtual void Broadcast2Dogs( ParamPool* pPool){}

	enum RunState
	{
		eRS_Connecting = 0,
		eRS_Connected,
		eRS_Ready,
		eRS_Broken,
		eRS_Closing,
	};

	void SetRunState(int8 nState);

	inline void StopMainLoop(){ m_bExitLoop = true; }
	inline bool IsExiting() const { return m_bExitLoop;}

	bool IsDebug() const { return m_bDebug; }

	void SetSrvID(int32 nSrvID);
	const uint8 GetSrvType() { return m_nSrvType; }
	int32 GetSrvID() { return m_nSrvID; }

	typedef std::list<ParamPool*> DogDataList;
	DogDataList m_listDogData;
protected:
	virtual void CalculateStatistics();
	virtual void ResetStatistics();

	void UpdateModuleTitle();

	void InitLog( int32 argc, char* argv[]);

	ServerManager& Servers;
	LoadTemplateManager& Templates;

	int8		m_RunState;
	bool		m_bExitLoop;
	ParamPool*	m_pModuleDogPool;

	const uint8	m_nSrvType;
	int32		m_nSrvID;
	std::string m_strModuelName;
	int32		m_nFrameInterval;
	bool		m_bDebug;
	std::string m_strModueTitle;
	int32		m_nAvgFrameTime;

	uint64		m_nLastFrameTicks;
	uint64		m_nLastTime;

	int8		m_nRunState;
};

