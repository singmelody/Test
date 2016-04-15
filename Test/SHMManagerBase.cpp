#include "StdAfx.h"
#include "SHMManagerBase.h"
#include "ShareMemory.h"
#include "MyLog.h"

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

			std::string strPath = Path::MainPath();
			strPath += m_strSHMPrefix;
			strPath += pDef->Name();

			if(ShouldDeleteOldSHMs())
			{
				ShareMemory::SHMRM( strPath.c_str(), 0);
			}

			const uint32 nUintSize = HEADER_SIZE_FOR_SHARED_MEMORY + sizeof() blablabla
		}
	}
}

void SHMManagerBase::CloseSHMs()
{

}
