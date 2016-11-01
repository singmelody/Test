#pragma once

enum ParamFlag
{
	eParam_Flag_Server			= 1 << 0,
	eParam_Flag_CltSelf			= 1 << 1,
	eParam_Flag_CltAll			= 1 << 2,
	eParam_Flag_Save			= 1 << 3,
	eParam_Flag_NoUpdate		= 1 << 4,
	eParam_Flag_Node			= 1 << 5,
};

enum ParamSync
{
	eParam_Sync_NULL		= 0,
	eParam_Sync_All			= 1 << 0,
	eParam_Sync_ClearDirty	= 1 << 1,
	eParam_Sync_IgnoreDft	= 1 << 2,
	eParam_Sync_Default		= 1 << 3,	// default sync change
};