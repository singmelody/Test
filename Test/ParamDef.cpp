#include "StdAfx.h"
#include "ParamDef.h"


ParamDef::ParamDef(void)
{
}


ParamDef::~ParamDef(void)
{
}

void ParamDef::LoadParamDef(DBRow& row)
{
	static int32 nCol_ParamID = row.GetColumnIdx("ParamID");
	static int32 nCol_ParamName = row.GetColumnIdx("ParamName");

	row.Fill( m_index, nCol_ParamID, -1);
	row.Fill( m_name, nCol_ParamName, "");
}

bool ParamDef::AddParam(ParamBase* pBase)
{
	return true;
}

ParamBase* ParamDef::GetParam(int32 nIdx)
{
	return NULL;
}
