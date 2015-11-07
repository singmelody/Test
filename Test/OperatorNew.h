#pragma once

#include <new>

class Allocator;

static Allocator*	s_pAlloc = NULL;
std::new_handler SetNewHandler( std::new_handler pfnHandler) throw();
std::new_handler GetNewHandler() throw();
void SetGlobalAlloc(Allocator* pAlloc);
Allocator* GetGlobalAlloc();

void* operator new( size_t nBytes);
void operator delete( void* ptr);

void* operator new[]( size_t nBytes );
void operator delete[]( void* ptr);

void* operator new ( size_t nBytes, const std::nothrow_t& ) throw();
void operator delete ( void* ptr, const std::nothrow_t& ) throw();

void* operator new[]( size_t nBytes, const std::nothrow_t& ) throw();
void operator delete[]( void* ptr, const std::nothrow_t& ) throw();
