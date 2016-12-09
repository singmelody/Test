#include "StdAfx.h"
#include "DBABase.h"
#include "ParamSet.h"

DBABase::DBABase(void)
	: PeerModuleBase( eSrv_DBA )
{
}


DBABase::~DBABase(void)
{
}

bool DBABase::CreateDogPool()
{
	if(!PeerModuleBase::CreateDogPool())
		return false;

	m_pDBADogPool = ParamSet::CreateNew( eParam_DBAInfo, 0);
	if(!m_pDBADogPool)
		return false;

	m_listDogData.push_back( m_pDBADogPool );
	return true;
}

void DBABase::UpdateDogPool(int32 nFrameTime)
{
	PeerModuleBase::UpdateDogPool(nFrameTime);
}
