#pragma once

#include <list>

class ParamPool;
class PacketBase;

class ModuleBase
{
public:
	ModuleBase(void);
	virtual ~ModuleBase(void);

	virtual bool Init(int32 nArgc, char* argv[]);
	virtual void Exit();
	virtual void OnConfigLoaded();
	virtual void AppendLoadTemplate();

	virtual bool StartMainLoop( int32 nFrameInterval);
	virtual bool CreateDogPool();

	typedef std::list<ParamPool*>	DogDataList;
	DogDataList	m_listDogData;

	enum RunState
	{
		eRS_Connecting = 0,
		eRS_Connected,
		eRS_Ready,
		eRS_Broken,
		eRS_Closing,
	};

	void SetRunState(int8 nState);

	inline void StopMainLoop(){ m_bExitLoop = true; }
	inline bool IsExiting() const { return m_bExitLoop;}

	typedef std::list<ParamPool*> DogDataList;

	DogDataList m_listDogData;

	void Send2Login(PacketBase* pPkt);

protected:
	ServerManager& Servers;

	int8		m_RunState;
	bool		m_bExitLoop;
	ParamPool*	m_pModuleDogPool;
};

