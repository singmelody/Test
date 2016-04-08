#include "StdAfx.h"
#include "ParamEx.h"
#include "MyString.h"
#include "ParamPool.h"

ParamBase::ParamBase()
	: m_index(-1)
	, m_id(-1)
	, m_flag(ePF_Server | ePF_Save)
	, m_typeid(eTB_Null)
{

}

ParamBase::~ParamBase()
{

}