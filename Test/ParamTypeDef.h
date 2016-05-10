#pragma once

enum ParamFlag
{
	eParamFlag_Server		= 1 << 0,
	eParamFlag_CltSelf		= 1 << 1,
	eParamFlag_CltAll		= 1 << 2,
	eParamFlag_Save			= 1 << 3,
	eParamFlag_NoUpdate		= 1 << 4,
	eParamFlag_Node			= 1 << 5,
};

enum ParamSync
{
	eParam_Sync_NULL		= 0,
	eParam_Sync_All			= 1 << 0,
	eParam_Sync_ClearDirty	= 1 << 1,
	eParam_Sync_IgnoreDft	= 1 << 2,
	eParam_Sync_Default		= 1 << 3,	// default sync change
};