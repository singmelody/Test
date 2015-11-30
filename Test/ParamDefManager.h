#pragma once
#include "LoadTemplate.h"
#include <map>

class ParamBase;
class ParamDef;

class ParamDefManager : public LoadTemplate
{
public:
	struct IListener{
		virtual ~IListener(){}
		virtual void OnParamDefLoaded() = 0;
	};

	ParamDefManager(void);
	~ParamDefManager(void);

	virtual bool LoadDataFromDB( DBInterface* pDBI);

protected:
	void InitParamMD5();
	ParamBase* CreateParam( const char* sParamType, const char* sDft, const char* sMax, const char* sMin);

	typedef std::map< int32, ParamDef*> ParamMap;
	typedef ParamMap::iterator ParamMapItr;
	ParamMap m_paramDefMap;
};

