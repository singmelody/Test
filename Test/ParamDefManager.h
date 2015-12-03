#pragma once
#include "Singleton.h"
#include "LoadTemplate.h"
#include <map>

class ParamBase;
class ParamDef;

class ParamDefManager_LoadHelper
{
public:
	bool LoadFromDB( ParamDefManager* pMgr, DBInterface* pDBI);

	bool InitParamDefine( ParamDefManager* pMgr, DBInterface* pDBI);
	bool InitParamColumn( ParamDefManager* pMgr, DBInterface* pDBI);
	bool InitParamData( ParamDefManager* pMgr, DBInterface* pDBI);

};

class ParamDefManager : public LoadTemplate, public Singleton<ParamDefManager>
{
public:
	struct IListener{
		virtual ~IListener(){}
		virtual void OnParamDefLoaded() = 0;
	};

	ParamDefManager(void);
	~ParamDefManager(void);

	virtual bool LoadDataFromDB( DBInterface* pDBI);

	ParamDef* GetParamDef( int32 nParamID, bool bCreate = false);
	void AddParamDef(int32 nIdx, ParamDef* pDef);
protected:
	void InitParamMD5();
	ParamBase* CreateParam( const char* sParamType, const char* sDft, const char* sMax, const char* sMin);

	typedef std::map< int32, ParamDef*> ParamMap;
	typedef ParamMap::iterator ParamMapItr;
	ParamMap m_paramDefMap;

	std::string m_paramDefClassType[eParam_Count];
};

