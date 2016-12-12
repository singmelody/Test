#include "StdAfx.h"
#include "ScenarioDef.h"

ScenarioParam::DBInfo::DBInfo(DBTable& table)
{
	static int32 nCol_ParamName = table.GetColumnIdx("ParamName");
	static int32 nCol_ParamInt1 = table.GetColumnIdx("ParamInt1");
	static int32 nCol_ParamInt2 = table.GetColumnIdx("ParamInt2");
	static int32 nCol_ParamFloat = table.GetColumnIdx("ParamFloat");
	static int32 nCol_ParamString = table.GetColumnIdx("ParamString");


}
