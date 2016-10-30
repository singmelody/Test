#include "StdAfx.h"
#include "WorldServerNormal.h"


WorldServerNormal::WorldServerNormal(void)
{
	m_nGateCount = 1;
	m_bWarWorld  = false;

	m_nConnectWarState = eCWS_Waiting;
	m_nConnectWarInterval = 5000;

	m_bUseConnectionThread = true;
}


WorldServerNormal::~WorldServerNormal(void)
{
}
