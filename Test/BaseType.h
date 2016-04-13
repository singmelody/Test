#pragma once

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
typedef unsigned long SM_KEY;

#define SrvID_NULL	-1
#define IPLEN		32

#define SAFE_DELETE(p) if(p) { delete (p); p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); p = NULL; }
#define SAFE_RELEASE(p) if(p) { p.Release(); p = NULL; }

#define MAKE_TYPE_ID( paramid, dataid) \
	( (paramid) << 24 + (dataid) & 0xffffff )

#define MAKE_LOGIC_ID( paramid, dataid) \
	( (paramid) << 8 + (dataid) & 0xff )

#define _WINSOCKAPI_
