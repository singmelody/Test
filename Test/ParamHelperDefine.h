#pragma once

#define MAX_PARAM_NAME_DEFINE_SIZE 70
#define MAX_PARAM_NAME_INDEX 911

extern char* Param_Index_List[MAX_PARAM_NAME_INDEX];
extern int32 Param_Name_Table[MAX_PARAM_NAME_DEFINE_SIZE][MAX_PARAM_NAME_INDEX];

enum
{
	param_name_avatardid	= 0,
	param_name_lastlandtime = 46,
	param_name_create_time	= 49,

	param_name_faceid		= 112,
	param_name_hairid		= 113, 
	param_name_eyeid		= 114, 
	param_name_skincolor	= 115,
	param_name_tattoo		= 116, 
	param_name_beard_eyeshadow	= 117,
	param_name_morph_1		= 118,
	param_name_morph_2		= 119,
	param_name_morph_3		= 120,
	param_name_fashionid	= 121,
	param_name_fashioncolor	= 122,
	param_name_lipcolor		= 123,

	param_name_ban_game_date= 132,
	param_name_clt_ip		= 136,
	param_name_left_point	= 137,
	param_name_viplevel		= 189,
	param_name_title		= 207,
	param_name_accountname	= 209,

	param_name_permanent	= 308,
	param_name_ServerID		= 679,
	param_name_SceneLoadValue = 708,
	param_name_CltCnt		= 734,
};