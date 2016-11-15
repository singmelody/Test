#pragma once

#pragma pack( push, 1)
struct ParamPoolExInfo_Node_Avatar
{
	enum EStatus
	{
		eStatus_Unknow	= 0,
		eStatus_Valid	= 1,
	};
};

struct ParamPoolExInfo_Node_CommonData
{
	int64	m_nAvatarDID;
	int32	m_nCommonDataIdx;
	int32	m_nCommonDataFlag;
};
#pragma pack(pop)
