#pragma once

#include "ParamHelperDefine.h"

#define PARAM_STRING_SIZE 32

class ParamNameIndexHelper
{
public:
	ParamNameIndexHelper(void);
	virtual ~ParamNameIndexHelper(void);

	static int32 GetParamIndex( int32 nDef, int32 nIdx)
	{
		if( nDef < 0 || nDef >= MAX_PARAM_NAME_DEFINE_SIZE )
			return -1;

		if( nIdx < 0 || nIdx >= MAX_PARAM_NAME_INDEX )
			return -1;

		return Param_Name_Table[nDef][nIdx];
	}

	static int32 GetParamIndexByName( const char* sName)
	{
		if(!sName)
			return -1;

		for (int32 i = 0; i < MAX_PARAM_NAME_INDEX; ++i)
		{
			if( strcmp( Param_Index_List[i], sName) == 0)
				return i;
		}

		return -1;
	}
};

