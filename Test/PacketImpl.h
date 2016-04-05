#pragma once
#include "PacketBase.h"
#include "FactoryManagerBase.h"
#include "MyFactory.h"
#include "PacketFactory.h"
#include "MyClass.h"
#include "MyClassManager.h"
#include "MyPacket.h"
#include "Singleton.h"
#include "Factory.h"

PACKET( PacketPack, PacketBase)
START_ADD_PACKET_MEMBER(PacketPack)
PACKET_END(PacketPack)