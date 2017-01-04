#pragma once

#include "AccountStateBase.h"
#include "Singleton.h"

class AccountState_Init : public AccountStateBase, public Singleton<AccountState_Init>
{
public:
	AccountState_Init(void)
		: m_strGameResVersion("BAD_VERSION")
	{
		char verBuffer[PACKET_VERSION_LEN];
		sprintf( verBuffer, "%d-%s", MY_VERSION, MY_PARAM_MD5);

		std::string strPath = std::string(Path::MainPath())  + "Data/game_res_version.txt";

		FILE* pFile = fopen( strPath.c_str(), "rt");
		if(pFile)
		{
			char buff[256];
			size_t nRet = fread( buff, 1, sizeof(buff), pFile);

			if(nRet > 0 )
				m_strGameResVersion = std::string( buff, nRet);
			else
				m_strGameResVersion = "Empty_Version";

			fclose(pFile);
		}

		m_strGameResVersion = std::string(verBuffer) + m_strGameResVersion;
		MyLog::message("Full Version[%s]", m_strGameResVersion.c_str());
	}

	virtual ~AccountState_Init(void);

	virtual bool HandlePacket( LoginAccount& account, PacketConnectLoginServer& pkt)
	{
		{
			MyMD5 md5;
			md5.Init();
			md5.Update(pkt.m_macAddress, pkt.MAC_ADDRESS_LEN);
			md5.Final();

			const byte* res = md5.GetDigestStream();
			char pwdhash[MD5_SIZE+1];

		}
	}
protected:
	virtual bool HasExpiration() { return false; }

	std::string	m_strGameResVersion;
};

