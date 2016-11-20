#include "StdAfx.h"
#include "ParamHelperDefine.h"


char* Param_Index_List[MAX_PARAM_NAME_INDEX] = {
	"avatardid",
	"guildid",
	"teamid",
	"lastlandtime",
	"create_time",
	"faceid",
	"hairid", 
	"eyeid", 
	"skincolor",
	"tattoo", 
	"beard_eyeshadow",
	"morph_1",
	"morph_2",
	"morph_3",
	"fashionid",
	"fashioncolor",
	"lipcolor",

	"ban_game_date",
	"clt_ip",
	"left_point",
	"level",
	"viplevel",
	"title",
	"accountname",

	"process",
	"permanent",
	"leaderdid",
	"teampost",
	"SceneProcessBits",
	"ServerID",
	"DBAID",
	"LoginID",
	"GateCnt",
	"LocalNodeCnt",
	"RemoteNodeCnt",

	"PlayerCnt",
	"SceneCnt",
	"TrunkCnt",
	"CopyCnt",
	"TeamCopyCnt",
	"TrunkCopyCnt",
	"RootCopyCnt",
	"SceneLoadValue",
	"SubWorldCnt",
	"WarID",

	"DBAPort",
	"MotherPort",

	"ZoneID",
	"GroupID",

	"WS_Logining",		
	"WS_Logined",
	"WS_DataLoading",
	"WS_DataReady",
	"WS_WaitScene",
	"WS_Decommission",
	"WS_EnterGame",
	"WS_Jumping",
	"WS_Gaming",
	"WS_ExitGame",
	"WS_Billing",

	"GuildCopyCnt",
	"WarWorld",

	"DBAIP",
	"MotherIP",

	"CltCnt"
};

int32 Param_Name_Table[MAX_PARAM_NAME_DEFINE_SIZE][MAX_PARAM_NAME_INDEX] = {-1};