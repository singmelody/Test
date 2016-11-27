#pragma once

#define MAX_PARAM_NAME_DEFINE_SIZE 70
#define MAX_PARAM_NAME_INDEX 911

extern char* Param_Index_List[MAX_PARAM_NAME_INDEX];
extern int32 Param_Name_Table[MAX_PARAM_NAME_DEFINE_SIZE][MAX_PARAM_NAME_INDEX];

enum
{
	param_name_avatardid	= 0,
	param_name_guildid		= 2,
	param_name_teamid		= 6,
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
	param_name_level		= 181,
	param_name_viplevel		= 189,
	param_name_title		= 207,
	param_name_accountname	= 209,

	param_name_process		= 305,
	param_name_permanent	= 308,
	param_name_leaderdid	= 404,
	param_name_teampost		= 467,
	param_name_SceneProcessBits = 595,

	param_name_ServerID			= 679,

	param_name_DBType			= 682,

	param_name_PeerIOCP			= 692,
	param_name_PeerIP			= 693,

	param_name_DBCName			= 695,

	param_name_DBAID			= 696,
	param_name_LoginID			= 697,
	param_name_GateCnt			= 698,
	param_name_LocalNodeCnt		= 699,
	param_name_RemoteNodeCnt	= 700,

	param_name_PlayerCnt		= 701,
	param_name_SceneCnt			= 702,
	param_name_TrunkCnt			= 703,
	param_name_CopyCnt			= 704,
	param_name_TeamCopyCnt		= 705,
	param_name_TrunkCopyCnt		= 706,
	param_name_RootCopyCnt		= 707,
	param_name_SceneLoadValue	= 708,
	param_name_SubWorldCnt		= 709,
	param_name_WarID			= 710,

	param_name_DBAPort			= 711,
	param_name_MotherPort		= 712,

	param_name_ZoneID			= 714,
	param_name_GroupID			= 715,

	param_name_WS_Logining		= 716,		
	param_name_WS_Logined		= 717,		
	param_name_WS_DataLoading	= 718,	
	param_name_WS_DataReady		= 719,
	param_name_WS_WaitScene		= 720,
	param_name_WS_Decommission	= 721,
	param_name_WS_EnterGame		= 722,
	param_name_WS_Jumping		= 723,
	param_name_WS_Gaming		= 724,
	param_name_WS_ExitGame		= 725,
	param_name_WS_Billing		= 726,

	param_name_GuildCopyCnt		= 728,
	param_name_WarWorld			= 729,

	param_name_DBAIP			= 730,
	param_name_MotherIP			= 731,

	param_name_CltCnt			= 734,

	param_name_DBName			= 750,
};