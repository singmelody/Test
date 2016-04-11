#pragma once

class PacketBase;
class PeerModuleBase;
class ServerManager;

class ModuleFuncManager
{
public:
	ModuleFuncManager( PeerModuleBase& peerModule);
	virtual ~ModuleFuncManager(void);

protected:
	void Send2Node( PacketBase& pkt, int32 nSrvID);
	void Send2Node( PacketBase* pkt, int32 nSrvID);

	void Send2Gate( PacketBase& pkt, int32 nSrvID, bool bGateProc = false);
	void Send2Gate( PacketBase* pkt, int32 nSrvID, bool bGateProc = false);

	void Send2Login( PacketBase& pkt);
	void Send2Login( PacketBase* pkt);

	void Send2DBA( PacketBase& pkt);
	void Send2DBA( PacketBase* pkt);

	void Send2GMI( PacketBase& pkt);
	void Send2GMI( PacketBase* pkt);

	void PeerSend( int32 nSocketID, PacketBase& pkt);
	void PeerSend( int32 nSocketID, PacketBase* pkt);

	void SendBySrvID( int32 nSrvID, PacketBase& pkt);
	void SendBySrvID( int32 nSrvID, PacketBase* pkt);

	int32 GetSrvID();

	PeerModuleBase& PeerModule;
	ServerManager& Servers;
};

