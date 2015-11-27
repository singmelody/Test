#pragma once

typedef unsigned long long uint64;
typedef long long int64;
typedef unsigned int uint32;
typedef int	int32;
typedef char Char;

#define SAFE_DELETE(p) if(p) { delete (p); p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); p = NULL; }
#define SAFE_RELEASE(p) if(p) { p.Release(); p = NULL; }

struct float2
{
	float2():x(0),y(0){}
	float x,y;
};

struct float3
{
	float3():x(0),y(0),z(0){}
	float x,y,z;
};

typedef float2 Point2;
typedef float2 Vector2;

typedef float3 Point3;
typedef float3 Vector3;