#pragma once

#define MAX_PARAM_NAME_DEFINE_SIZE 70
#define MAX_PARAM_NAME_INDEX 911

extern char* Param_Index_List[MAX_PARAM_NAME_INDEX];
extern int32 Param_Name_Table[MAX_PARAM_NAME_DEFINE_SIZE][MAX_PARAM_NAME_INDEX];

enum
{
	param_name_avatardid = 0,
};