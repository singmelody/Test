#include "StdAfx.h"
#include "SHMAllocator.h"
#include "ParamDef.h"

SHMAllocator::SHMAllocator(ParamDef& def, void* pMemoryBlock, uint32 nUintSize, uint32 nUintCount)
	: m_memoryPool(pMemoryBlock, nUintSize, nUintCount, def.ExtraSize())
	, m_nObjSize()
{

}

SHMAllocator::~SHMAllocator(void)
{
}
