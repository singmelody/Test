// StringParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
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

enum eCmdParser
{
	eCmdParser_Undetermined		= 0,
	eCmdParser_Static			= 1,
	eCmdParser_ParserEveryTime	= 2,
};

enum eParamParser
{
	eParamParser_Undefined	= 0,
	eParamParser_NoParam	= 1,
	eParamParser_HasParam	= 2,
};

enum eParamType
{
	eParam_None,
	eParam_Decimal,
	eParam_Real,
	eParam_String,
	eParam_Player
};

struct SCommand;

int32 g_Common_Char = '#';
sCommand g_Cmds[] = 
{
	{ "#c",		0,			Color_CMD_Size,	g_str_Pre_Color,	g_Str_Post_Color, g_str_Tmt}
}

struct sCommandParserContext
{
	sCommandParserContext();

	
};

struct sCommand
{
	char* strCmd;
	char* strCmdTmt;
	size_t nCmdLen;
	char* strPre;
	char* strPost;
	char* strTmt;
	ParserFunc func;
	int32 nParse;
	bool Exe(char** s, sCommandParserContext& context, int32& p);
};
const static size_t g_CmdCount = sizeof(g_Cmds) / sizeof(sCommand);

struct sParam
{
	sParam() : addr(0), nLen(0), nParamType(eParam_None), n(0) {}

	char*	addr;
	size_t	nLen;
	int32   nParamType;
	sParam*	n;
};

struct sString
{
	sString();
	~sString();

	char* str;	// Ô­Ê¼×Ö·û´®
	char* pStr;	// º¬²Î×Ö·û´®
	char* cStr;	// Ä¿±ê×Ö·û´®

	int32	nParamParser;
	int32	nCmdParser;
	sParam* pParams;
};

struct sParamParser
{
	sParamParser(): pRoot(NULL), pCur(NULL), bHasParam(false){}

	void Parser(char* s)
	{
		Clear();

		if(!s)
			return;

		char* preStop = s;
		while (*s != '\0')
		{
			if(*s == g_Common_Char)
			{
				++s;

				// #%
				if(*s == '%')
				{
					if( preStop != '\0' && preStop < (s-1) )
					{
						AddParam( preStop, size_t(s - 1 - preStop), eParam_None);
					}
					preStop = s;
					++s;
				}
				continue;
			}

			if(*s == '%')
			{
				bHasParam = true;
				if( preStop != '\0' && preStop < s)
				{
					AddParam( preStop, size_t(s - preStop), eParam_None);
				}

				preStop = '\0';
				++s;

				if( *s == '\0' )
					return;

				switch(*s)
				{
				case 'd':
					{
						AddParam( 0, 0, eParam_Decimal);
					}
					break;
				case 'f':
					{
						AddParam( 0, 4, eParam_Real);
					}
					break;
				case 's':
					{
						AddParam( 0, 0, eParam_String);
					}
					break;

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
					{
						size_t pl = (*s) - '0';
						++s;
						if(*s == 0)
							return;

						if(*s == 'f')
							AddParam( 0, pl, eParam_Real);
					}
					break;

				case '@':
					{
						AddParam( 0, 0, eParam_Player);
					}
					break;

				default: break;
				}
				++s;

				if( *s == '\0')
					return;

				preStop = s;
			}
			else
				++s;
		}

		if( *preStop != '\0' && preStop < s)
			AddParam( preStop, size_t(s - preStop), eParam_None);
	}


	bool HasParam()
	{
		return bHasParam;
	}
	
	void AddParam(char* s, size_t l, eParamType nPt)
	{
		if(pCur)
		{
			if(pCur->n == 0)
				pCur->n = new sParam();

			pCur = pCur->n;
		}
		else if(pRoot)
		{
			pCur = pRoot;
		}
		else
		{
			pCur = pRoot = new sParam();
		}

		if(!pCur)
			return;

		pCur->addr = s;
		pCur->nLen = l;
		pCur->nParamType = nPt;
	}
	
	void Clear()
	{
		pCur = NULL;
		bHasParam = false;
	}

	sParam* pRoot;
	sParam* pCur;
	bool	bHasParam;
};

class StrParser
{
	static bool ParseParams(sString* sPtr)
	{
		if( !sPtr || !sPtr->str || sPtr->nParamParser != eParamParser_Undefined)
			return false;

		sParamParser parser;
		parser.Parser(sPtr->str);

		sParam* pParam = parser.pRoot;
		if (!parser.HasParam())
		{
			sPtr->nParamParser = eParamParser_NoParam;
			sPtr->pStr = sPtr->str;
			parser.pRoot = NULL;
			if(pParam)
			{
				do 
				{
					sParam* op = pParam;
					pParam = pParam->n;
					delete op;
				} while (pParam);
			}
		}
		else
		{
			sPtr->nParamParser = eParamParser_HasParam;
			sPtr->pParams = pParam;
		}
	}
	

	static bool ApplyParams(sString* sPtr, int32 nArgc, const char* argv[])
	{
		if( sPtr == '\0' || sPtr->pParams == NULL)
			return;

		assert( sPtr->nParamParser == eParamParser_HasParam );
	}

	static bool ParserCommands()
	{

	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	sParamParser parser;
	parser.Parser("111%d111#%#%");
	return 0;
}

