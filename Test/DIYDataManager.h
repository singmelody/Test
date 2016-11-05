#pragma once
#include "LoadTemplate.h"
#include "Singleton.h"
class DIYDataManager : public LoadTemplate, public Singleton<DIYDataManager>
{
public:
	DIYDataManager(void);
	~DIYDataManager(void);

	bool CheckDIYDataValid(class PacketCreateAvatar* pPkt);
};

