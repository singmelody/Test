#pragma once

#include "BaseType.h"

enum LoginAction_Type
{
	eLoginAction_Buff = 0,
	eLoginAction_Item = 1,


	eLoginAction_Max = 7,

};

struct LoginActionData
{
	int32 m_nType;
	int32 m_nData;
	int32 m_nData1;
	int32 m_nData2;
	int32 m_nData3;
	uint32 m_nCreateTime;
};

