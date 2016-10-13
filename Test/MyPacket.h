#pragma once
#include "MyClass.h"

#include "ServerConfig.h"
#include "MyFactory.h"
#include "Factory.h"
#include "PacketFactory.h"
#include "MyFactory.h"
#include "MyClassManager.h"
#include "PacketBase.h"


#define PACKET_EX( subPacket, basePacket, alloc) \
	class subPacket : public basePacket\
	{ \
		DECLARE_FACTORY_ARG0_Ex( subPacket, -1, alloc, PacketFactory::Instance()) \
	public: \
		subPacket() { m_PacketID = GetClass()->ClassID(); } \
		~subPacket() {} \
		char* ReadPacket(char* pBuffer) {\
		char* tmp = basePacket::ReadPacket(pBuffer); return Read##subPacket(tmp); }\
		char* WritePacket(char* pBuffer){\
		char* tmp = basePacket::WritePacket(pBuffer); return Write##subPacket(tmp); }\
		int32 PacketClassSize() { return sizeof(subPacket); } 

#define PACKET( packet, basePacket) \
	PACKET_EX( packet, basePacket, new PoolPacketAllocator())

#define PACKET_END(packet) \
	Ref_BaseClass_End(packet) \
	};

#define PACKET_DROP( packet, basePacket) \
	PACKET_EX( packet, basePacket, new PoolPacketAllocator()) \

#define IMPLE_GetStaticAttribute(ATTRI) \
	private:\
		virtual uint32 GetStaticAttribute() const { return ATTRI; } \
	public: \

#define START_ADD_PACKET_MEMBER(packet) \
	Ref_BaseClass_Begin(packet) \

#define PACKET_IMPL(packet) \
	Ref_Class(packet) \
	FINISH_FACTORY_ARG0(packet)

#define ADD_PACKET_MEMBER( member, type, des) \
	__Add_Ref_Member__( member, type, eTB_Flag_Sec, des, MyClassMemberEx);

#define ADD_PACKET_MEMBER_STR( member, size, des) \
	Add_Ref_Member_String( member, size, eTB_Flag_Sec, des);

#define PACKET_CLT_2_GATE( packet, basePacket) \
	PACKET( packet, basePacket) \
	IMPLE_GetStaticAttribute( PktAttr_IsClt2Srv | PktAttr_ShouldEncrypt ) 


#define IP_ADDRESS 32
#define KEY_LENGTH 32