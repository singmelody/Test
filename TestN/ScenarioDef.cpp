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

ScenarioParam::ScenarioParam(DBInfo dbInfo, DBRow& row)
{
	row.Fill( ParamName, dbInfo.Col_ParamName, "");
	std::transform( ParamName.begin(), ParamName.end(), ParamName.begin(), ::tolower);

	row.Fill( ParamInt1, dbInfo.Col_ParamInt1, -1);
	row.Fill( ParamInt2, dbInfo.Col_ParamInt2, -1);

	row.Fill( ParamFloat, dbInfo.Col_ParamFloat, -1);
	row.Fill( ParamString, dbInfo.Col_ParamString, "");
}


