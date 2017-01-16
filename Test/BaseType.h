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
typedef unsigned long DWORD;

#define SrvID_NULL	-1
#define IPLEN		32

#define W2G_Packet_Counter 0
#define G2W_Packet_Counter 0
#define N2G_Packet_Counter 0
#define G2N_Packet_Counter 0
#define N2W_Packet_Counter 0
#define W2N_Packet_Counter 0
#define L2W_Packet_Counter 0
#define W2L_Packet_Counter 0

#define PEER_THREAD_WATCHDOG_ID 0
#define SRV_THREAD_WATCHDOG_ID	1
#define GAME_THREAD_WATCHDOG_ID	2

#define Create_Avatar_Pending_Time		100000
#define MAX_SCENE_CREATE_PENDING_TIME	30000

#define PACKET_PROCESS_COUNT_PER_TICK	10000

#define INVALID_GUILD_ID int64(0)

class TiXmlDocument;
class TiXmlElement;
class TiXmlDeclaration;
class TiXmlNode;

typedef TiXmlDocument XmlDocument;
typedef TiXmlElement XmlElement;
typedef TiXmlDeclaration XmlDeclaration;
typedef TiXmlNode XmlNode;

typedef void* SMHandle;

#define SAFE_DELETE(p) if(p) { delete (p); p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); p = NULL; }
#define SAFE_RELEASE(p) if(p) { p.Release(); p = NULL; }

#define MAKE_TYPE_ID( paramid, dataid) \
	( (paramid) << 24 + (dataid) & 0xffffff )

#define MAKE_LOGIC_ID( paramid, dataid) \
	( (paramid) << 8 + (dataid) & 0xff )

#define MAX_BIT_SIZE				512
#define BIT_SIZE					32
#define BIT_LIST_SIZE				(MAX_BIT_SIZE/BIT_SIZE+1)

#define SERVER_TICK_TIME_SHOW			1000
#define GAME_THREAD_WATCHDOG_ID			2
#define SCENE_ID_NULL					0
#define SCENE_MAIN_LINE_ID				1
#define SERVERID_NULL					-1
#define	INVALID_TEAM_ID					-1
#define SCENE_ID_DEFAULT				1
#define MAX_MANUFACTURE_RECIPE_COUNT	512
#define MY_SOCKET_LIST_SIZE				2048
#define SERVER_NEW_CONNECTION_TIMEOUT	10000
#define XIAOTUI_TIME					10000

#define SRV_HEART_BEAT_INTERVAL			30000



#define Clt_Packet_Check_Time			20
#define Clt_Packet_Send_Freq			100*Clt_Packet_Check_Time

#define MIN_ACCOUNT						5
#define MAX_ACCOUNT						50

#define MAX_TITLE						40

#define MAX_AVATAR_COUNT_ONE_USER				3

#define NET_TEST_USE_SRV_SEND_DELAY				0
#define NET_TEST_USE_SRV_SEND_DELAY_MIN			100
#define NET_TEST_USE_SRV_SEND_DELAY_MAX			150

#define NET_TEST_REDEFINE_SEND_COUNT_PER_TICK	0
#define NET_TEST_PACKET_SEND_COUNT_PER_TICK		200
#define GATE_SRV_SEND_DROP_PACKET_COUNT			1024*1000

#define WORLD_FRAME_TIME				100		// 10fps
#define GATE_FRAME_TIME					50		// 20fps
#define DBA_FRAME_TIME					100		// 10fps
#define NODE_FRAME_TIME					40		// 25fps
#define NODE_SLOW_TICK_TIME				100		// 10fps for scenario

#define INVALID_SMHANDLE NULL

#define _WINSOCKAPI_
