// StringParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <iostream>
#include <vector>
#include <sstream>
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
	eParam_Player,
	eParam_Place,
	eParam_Npc,
};

struct sCommand;
struct sCommandParserContext;

typedef void(*ParserFunc)(char* s, char* ts, sCommand& cmd, sCommandParserContext& context, int32& p);
void Test(char* s, char* ts, sCommand& cmd, sCommandParserContext& context, int32& p);


int32 g_Command_Char = '#';
char* g_Command_String = "#";
char* g_Empty_String = "";
char* g_Str_Tmt_CMD = "}";
char* g_Str_Pre_Color = "<FONT SIZE='";
char* g_Str_Post_Color = "'>";
char* g_Str_Tmt_Font = "</FONT>";

const static size_t Color_CMD_Size = 8;


struct sStringSegment
{
	sStringSegment() : pBuffer(0), nLen(0) {}
	sStringSegment(char* s, size_t l) : pBuffer(s), nLen(l) {}
	bool Equals(const char* s);

	char*	pBuffer;
	size_t	nLen;
};

struct sStringSegmentList
{
	static sStringSegment NULLSG;

	sStringSegmentList() : nLen(0){}
	size_t Count();
	void Push( char* s, size_t l)
	{
		segs.push_back(sStringSegment(s, l));
		nLen += l;
	}

	void Clear();
	char* Combine();
	sStringSegment& operator[](size_t nIdx)
	{
		if( nIdx > segs.size() - 1 )
			return NULLSG;

		return segs[nIdx];
	}

	typedef std::vector<sStringSegment> SegsVect;
	SegsVect	segs;
	size_t		nLen;
};

template <int32 nCapacity>
struct sStringBuffer
{
	typedef std::vector<char*>	DynamicBuffVect;

	sStringBuffer()
	{
		memset( stBuff, 0, sizeof(stBuff));
		nPos = 0;
	}

	~sStringBuffer()
	{
		Clear();
	}

	void Clear()
	{
		nPos = 0;
		if(dynBuff.empty())
			return;

		DynamicBuffVect::iterator lastItr = dynBuff.end();
		for( DynamicBuffVect::iterator itr = dynBuff.begin(); itr != lastItr; ++itr)
			free(*itr);

		dynBuff.clear();
	}

	char	stBuff[nCapacity];
	size_t	nPos;
	DynamicBuffVect dynBuff;

	char* Write(char* s, size_t l)
	{
		if( l < 1)
			return 0;

		if( nPos + l > nCapacity)
		{
			void* db = malloc(l);
			memcpy( db, s, l);
			dynBuff.push_back((char*)db);
			return (char*)db;
		}

		char* sb = stBuff + nPos;
		memcpy( sb, s, l);
		nPos += l;
		return sb;
	}
};


struct sCommandParserContext
{
	sCommandParserContext(){}
	sStringSegmentList	sl;
	sStringBuffer<1024>	sb;
	typedef std::vector<sCommand*>	CmdStack;
	CmdStack ss;
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
	bool Exe(char** s, sCommandParserContext& context, int32& p)
	{
		char* c = strCmd;
		char* ts = *s;

		while (*c != 0 && *ts != 0 && *c == *ts )
		{
			++c; 
			++ts;
		}

		if(*c)
			return false;

		char* tst = ts;
		char* preTst = 0;
		if(strCmdTmt)
		{
			char* ct = strCmdTmt;
			for (; (*tst) != 0; ++tst)
			{
				if(*tst == *ct)
				{
					char* ttst = tst;
					while ( *ct != 0 && *ttst != 0 && *ct == *ttst )
					{
						++ct;
						++ttst;
					}

					if(*ct == 0)
						break;

					ct = strCmdTmt;
				}
			}

			if(*tst == 0)
				return true;

			preTst = tst;
			tst = tst + strlen(strCmdTmt);
		}
		else
		{
			size_t nIdx = 0;
			while (nIdx < nCmdLen )
			{
				if( *((*s) + nIdx) == 0 )
					return true;

				++nIdx;
			}

			tst = *s + nCmdLen;
			preTst = tst;
		}

		if(strPre)
			context.sl.Push( strPre, strlen(strPre));

		int32 nDynParse = eCmdParser_Undetermined;
		if(func)
			(*func)(ts, preTst, *this, context, nDynParse);

		if(strPost)
			context.sl.Push( strPost, strlen(strPost));

		if(strTmt)
			context.ss.push_back(this);

		if(nParse == eCmdParser_Undetermined)
			p = nDynParse;
		else
			p = nParse;

		*s = tst;

		return true;
	}
};

sCommand g_Cmds[] = 
{
	{ "#c",		0,		Color_CMD_Size,		g_Str_Pre_Color,	g_Str_Post_Color,	g_Str_Tmt_Font,		Test,		eCmdParser_Static},
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
	sString() : str(NULL), pStr(NULL), cStr(NULL), nParamParser(eParamParser_Undefined), nCmdParser(eCmdParser_Undetermined), pParams(NULL)
	{

	}

	~sString(){}

	char* str;	// Ô­Ê¼×Ö·û´®
	char* pStr;	// º¬²Î×Ö·û´®
	char* cStr;	// Ä¿±ê×Ö·û´®

	int32	nParamParser;
	int32	nCmdParser;
	sParam* pParams;
};


char* sStringSegmentList::Combine()
{
	if( nLen < 1)
		return g_Empty_String;

	char* s = new char[nLen + 1];
	size_t cp = 0;
	SegsVect::iterator itr = segs.end();
	for (SegsVect::iterator itr = segs.begin(); itr != segs.end(); ++itr)
	{
		sStringSegment& seg = *itr;
		memcpy( s+cp, seg.pBuffer, seg.nLen);
		cp += seg.nLen;
	}

	s[nLen] = '\0';
	return s;
}




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
			if(*s == g_Command_Char)
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

std::string GetS()
{
	const char* str = "ÈÕ»ª³Ç";
	return str;
}

class StrParser
{
public:
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

		return true;
	}
	

	static bool ApplyParams(sString* sPtr, int32 nArgc, const char* argv[])
	{
		if( !sPtr || !(sPtr->pParams) )
			return false;

		assert( sPtr->nParamParser == eParamParser_HasParam );

		sParam* p = sPtr->pParams;

		sStringSegmentList	sl;
		sStringBuffer<256>	sb;

		std::stringstream ss;

		int32 nIdx = 0;
		while (p)
		{
			switch(p->nParamType)
			{
			case eParam_None:
				{
					sl.Push( p->addr, p->nLen);
				}
				break;
			case eParam_Decimal:
			case eParam_Real:
			case eParam_String:
				{
					if( nIdx < nArgc )
					{
						const char* val = argv[nIdx];
						size_t l = strlen(val);
						sl.Push(const_cast<char*>(val), l);
					}

					++nIdx;
				}
				break;
			case eParam_Player:
				{
					if( nIdx < nArgc )
					{
						int32 nCnt = *(argv[nIdx] - '0');
						++nIdx;

						int32 i = 0;
						for (; i < nCnt && ( nIdx < nArgc); ++i, ++nIdx)
						{
							const char* pArgStr = argv[nIdx];
							if(!pArgStr)
								break;

							size_t l = strlen(pArgStr);
							sl.Push(const_cast<char*>(pArgStr), l);
						}
					}
				}
				break;
			default:break;
			}
			p = p->n;
		}

		char* s = sl.Combine();
		if(sPtr->pStr)
		{
			assert( sPtr->str != sPtr->pStr);
			if(sPtr->pStr == sPtr->cStr)
				sPtr->cStr = NULL;

			delete [] sPtr->pStr;
		}

		sPtr->pStr = s;
		return true;
	}

	static bool ParserCommands(sString* pPtr)
	{
		if( !pPtr || !pPtr->pStr)
			return false;

		if( pPtr->nCmdParser == eCmdParser_Static )
			return false;
	
		char* s = pPtr->pStr;
		int32 p = eCmdParser_Static;
		sCommandParserContext context;

		char* preS = '\0';
		bool bHasCmd = false;
		for (;(*s) != '\0';)
		{
			if(!IsCommandChar(*s))
			{
				if(!preS)
					preS = s;
				++s;
				continue;
			}

			if(preS != '\0' && preS < s)
				context.sl.Push( preS, size_t(s - preS));

			preS = '\0';
			bHasCmd = true;
			char* oldS = s;
			for (size_t i = 0; i < g_CmdCount; ++i)
			{
				int32 nCmdParse = eCmdParser_Undetermined;
				if( g_Cmds[i].Exe( &s, context, nCmdParse))
				{
					if( nCmdParse == eCmdParser_ParserEveryTime)
						p = eCmdParser_ParserEveryTime;

					break;
				}
			}

			if( oldS == s)
				++s;
		}

		if( preS != '\0' && preS < s)
			context.sl.Push( preS, size_t(s-preS));

		preS = 0;

		if(pPtr->nParamParser == eParamParser_HasParam )
			p = eCmdParser_ParserEveryTime;

		pPtr->nCmdParser = p;
		if(!bHasCmd)
		{
			pPtr->cStr = pPtr->pStr;
			return true;
		}

		// clear stack
		while (!context.ss.empty() )
		{
			sCommandParserContext::CmdStack::iterator fiter = context.ss.begin();
			if(fiter == context.ss.end() )
				return false;

			sCommand* tCmd = *fiter;
			if(!tCmd)
				continue;

			context.sl.Push(tCmd->strTmt, strlen(tCmd->strTmt));
			context.ss.erase(fiter);
		}

		// combine string
		char* destS = context.sl.Combine();

		// save rst
		if(pPtr->cStr)
			delete [] pPtr->cStr;

		pPtr->cStr = destS;

		return true;
	}

	static bool IsCommandChar(char c)
	{
		return (c == g_Command_Char);
	}
	//int32 GenPlayerLink(std::vector<>)
};

void Test(char* s, char* ts, sCommand& cmd, sCommandParserContext& context, int32& p)
{
	size_t nParamSize = Color_CMD_Size - strlen(cmd.strCmd);
	if( strlen(s) < nParamSize)
		return;

	char* wb = context.sb.Write( s, nParamSize);
	if(wb)
		context.sl.Push( wb, nParamSize);
}


int _tmain(int argc, const char* argv[])
{
	sString sptr;

	const char* myArgv[] = {
			"1002",
			//"200"
	};

	int32 nArgc = sizeof(myArgv)/sizeof(char*);

	sptr.str = "#cffffffß÷ß÷ß÷";

	//StrParser::ParseParams(&sptr);
	StrParser::ApplyParams(&sptr, nArgc, myArgv);
	StrParser::ParserCommands(&sptr);

	StrParser::ParseParams(&sptr);
	StrParser::ApplyParams(&sptr, nArgc, myArgv);
	StrParser::ParserCommands(&sptr);

	std::cout<<sptr.cStr<<std::endl;

	return 0;
}

