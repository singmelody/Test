#pragma once

#include "ParamDefManager.h"
#include "ParamPool2SqlProcessor.h"

class ShareMemory;

class SHMManagerBase : public ParamDefManager::IListener, public ParamPool2SqlProcessorManagerBase
{
public:
	SHMManagerBase(void);
	virtual ~SHMManagerBase(void);

	virtual void OnParamDefLoaded();

	void Init( const std::string& strSHMPrefix, bool bIsLogicProc, bool bEnableSHM, bool bCreateSHM, bool bDeleteOldSHM);
	bool OpenSHMs();
	void CloseSHMs();

	bool EnableSHM() { return m_bEnableSHM; }

protected:
	struct TypeInfo
	{
		char*		m_pMemPtr;
		uint32		m_nUnitSize;
		uint32		m_nCountUnits;
		int32		m_nParamDefIndex;
		ParamDef*	m_pParamDef;
	};

	std::vector<ShareMemory*>	m_vSHMs;
	std::map< int32, TypeInfo>	m_mapParamIdx2Infos;

	std::string m_strSHMPrefix;
	bool m_bIsLogicProc;

	bool m_bEnableSHM;
	bool m_bCreateSHM;
	bool m_bShouldDeleteOldSHM;
};

