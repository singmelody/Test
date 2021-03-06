#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <Windows.h>
#include <assert.h>

using namespace std;

typedef unsigned long long uint64;
typedef long long int64;
typedef char Char;
typedef float f32;
typedef double f64;

typedef unsigned char uint8;
typedef signed char	int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;

#define SAFE_DELETE(p) if(p) { delete (p); p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); p = NULL; }
#define SAFE_RELEASE(p) if(p) { p.Release(); p = NULL; }

void SetColor(unsigned short attr);

class BaseTestUnit
{
public:
	BaseTestUnit(void);
	virtual ~BaseTestUnit(void);

	virtual bool Calcuate() = 0;
	virtual bool Verify() = 0;
};

