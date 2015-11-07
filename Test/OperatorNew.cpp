#include "StdAfx.h"
#include "OperatorNew.h"
#include "DLAllocator.h"
#include "DLMalloc.h"
#include <assert.h>

std::new_handler SetNewHandler(std::new_handler pfnHandler) throw()
{
	return std::set_new_handler(pfnHandler);
}

std::new_handler GetNewHandler() throw()
{
	std::new_handler pfnHandler = std::set_new_handler(0);
	std::set_new_handler(pfnHandler);
	return pfnHandler;
}

void SetGlobalAlloc(Allocator* pAlloc)
{
	s_pAlloc = pAlloc;
}

Allocator* GetGlobalAlloc()
{
	return s_pAlloc;
}

void* operator new( size_t nBytes)
{
	for (;;)
	{
		try
		{
			if(!s_pAlloc)
			{
				s_pAlloc = (Allocator*)dlmalloc( sizeof(DLAllocator));
				if(s_pAlloc)
				{
					new(s_pAlloc)DLAllocator();
					MemoryHead head;
					head.MemInfo = (int)nBytes;
					head.AllocInfo = 20151110;
					s_pAlloc->SetMemoryInfo(head);
				}
				else
					throw;
			}

			return s_pAlloc->TMalloc((int32)nBytes);
		}
		catch ( const std::bad_alloc& )
		{
			std::new_handler pfnNewHandler = GetNewHandler();
			if(!pfnNewHandler)
				throw;

			pfnNewHandler();
		}
	}
}

void operator delete(void* ptr)
{
	if(!s_pAlloc)
	{
		printf("Bad ::delete, Global Allocator is NULL");
		return;
	}

	return s_pAlloc->TFree(ptr);
}

void* operator new[]( size_t nBytes )
{
	return operator new(nBytes);
}

void operator delete[]( void* ptr)
{
	operator delete(ptr);
}

void* operator new ( size_t nBytes, const std::nothrow_t& ) throw()
{
	assert(false);
	return NULL;
}
void operator delete ( void* ptr, const std::nothrow_t& ) throw()
{
	assert(false);
}

void* operator new[]( size_t nBytes, const std::nothrow_t& ) throw()
{
	assert(false);
	return NULL;
}
void operator delete[]( void* ptr, const std::nothrow_t& ) throw()
{
	assert(false);
}
