#include "StdAfx.h"
#include "SHMManagerBase.h"
#include "ShareMemory.h"
#include "MyLog.h"
#include "GameUtil.h"
#include "StaticMemoryPool.h"
#include "SHMAllocator.h"

SHMManagerBase::SHMManagerBase(void)
{
}


SHMManagerBase::~SHMManagerBase(void)
{
}

void SHMManagerBase::OnParamDefLoaded()
{
	InitParamPool2SqlProcessors(EnableSHM());

	RegisterProcessor();

	if(!EnableSHM())
		return;

	if(!OpenSHMs())
	{
		CloseSHMs();
		MyLog::error("SHMManagerBase::OnParamDefLoaded() OpenSHMs Failed");
	}
}


void SHMManagerBase::Init(const std::string& strSHMPrefix, bool bIsLogicProc, bool bEnableSHM, bool bCreateSHM, bool bDeleteOldSHM)
{
	m_bIsLogicProc = bIsLogicProc;
}


bool SHMManagerBase::OpenSHMs()
{
	const ParamMap& def = ParamDefManager::Instance().GetDefMap();

	{
		for (ParamMap::const_iterator itr = def.begin(); itr != def.end(); ++itr)
		{
			ParamDef* pDef = itr->second;
			assert(pDef);

			IParamPool2SqlProcessor* pIParamPool2SqlProcessor = pDef->GetIParamPool2SqlProcessor();
			if(!pIParamPool2SqlProcessor || !pIParamPool2SqlProcessor->UseSHM())
				continue;

			std::auto_ptr<ShareMemory> pSHM(new ShareMemory);
			if( NULL == pSHM.get() )
			{
				MyLog::error("New ShareMemory Failed\n");
				continue;
			}

			std::string strPath = MyPath::MainPath();
			strPath += m_strSHMPrefix;
			strPath += pDef->Name();

			if(ShouldDeleteOldSHMs())
			{
				ShareMemory::SHMRM( strPath.c_str(), 0);
			}

			const uint32 nUnitSize = HEADER_SIZE_FOR_SHARED_MEMORY + sizeof(MemoryHead) + pDef->Size() + pIParamPool2SqlProcessor->GetExtraSize();
			bool bCreate;
			if(!pSHM->Open( strPath.c_str(), 0, nUnitSize * pDef->SMUCount(), bCreate))
			{
				MyLog::error("SHMManagerBase::OpenSHMs() Failed! can not open [%s] SUMUCount=[%d] unitSize=[%d]", strPath.c_str(), pDef->SMUCount(), nUnitSize);
				return false;
			}

			if( bCreate != ShouldCreateSHMs() )
			{
				pSHM->Close();
				return false;
			}

			char* ptr = (char*)(pSHM->GetShareMemoryPtr());
			assert( ptr );
			if(!ptr)
			{
				pSHM->Close();
				MyLog::error("SHMManagerBase::OpenSHMs() FAILED! SHM [%s] GetShareMemoryPtr() Failed", strPath.c_str());
				return false;
			}

			TypeInfo info;
			info.m_nUnitSize = nUnitSize;
			info.m_nCountUnits = pDef->SMUCount();
			info.m_pMemPtr = ptr;
			info.m_pParamDef = pDef;
			info.m_nParamDefIndex = pDef->Index();

			m_vSHMs.push_back(pSHM.release());
			m_mapParamIdx2Infos.insert( std::make_pair( pDef->Index(), info));

			if(IsLogicProc())
			{
				assert( !pDef->GetBufferAlloc() );
				pDef->SetBufferAlloc(new SHMAllocator(*pDef, ptr, nUnitSize, pDef->SMUCount()));
			}
		}
	}

	return true;
}

void SHMManagerBase::CloseSHMs()
{

}
