typedef unsigned long long uint64;
typedef long long int64;
typedef unsigned int uint32;
typedef int	int32;
typedef char Char;

#define SAFE_DELETE(p) if(p) { delete (p); p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); p = NULL; }
#define SAFE_RELEASE(p) if(p) { p.Release(); p = NULL; }