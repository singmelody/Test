#include "StdAfx.h"
#include "SceneInfo.h"
#include "SceneManagerBase.h"
#include "BaseType.h"

SceneCreateArg::SceneCreateArg()
{
	m_nSceneSID = SCENE_ID_NULL;
	m_SceneID = SCENE_ID_NULL;
	m_SceneLevel = 1;
	m_NodeSrvID = SERVERID_NULL;
	m_ArenaID = SCENE_ID_NULL;
	m_nSceneCustomData = 0;
}

int16 SceneCreateArg::TemplateID()
{
	return SceneInfo::GetSceneInstanceID( m_SceneID );
}

int16 SceneCreateArg::InstanceID()
{
	return SceneInfo::GetSceneSID( m_SceneID );
}

SceneInfo::SceneInfo(void)
{
	m_nNextSceneSID = SCENE_ID_NULL;
	m_nSceneSID = SCENE_ID_NULL;
	m_bDynEnterSwitch = true;

	m_SceneAttribute = 0;
	m_bForceEnter = false;
	m_nParalleCnt = 1;

}


SceneInfo::~SceneInfo(void)
{
}

bool SceneInfo::LoadSceneInfo(DBRow& row)
{
	return true;
}

uint16 SceneInfo::GetSceneSID(uint32 nSceneID)
{
	nSceneID >>= 16;
	return uint16(nSceneID);
}

uint16 SceneInfo::GetSceneInstanceID(uint32 nSceneID)
{
	nSceneID &= 0xffff;
	return uint16(nSceneID);
}

uint32 SceneInfo::GenSceneID(uint16 nSceneSID, uint16 nInstanceID /*= 0*/)
{
	uint32 nVal = nSceneSID << 16;
	return nVal | nInstanceID;
}

const SceneInfo* SceneInfo::GetSceneInfo(uint16 nSceneSID)
{
	return SceneManagerBase::GetSceneInfo( nSceneSID );
}



const EnterPointInfo* SceneInfo::GetEnterPointInfo(uint16 from_scenesid, int32 nEnterType /*= eEnterType_Default*/) const
{
	EnterInfo* pEnterInfo = (EnterInfo*)GetEnterInfo(from_scenesid);
	if(!pEnterInfo)
		return NULL;

	return pEnterInfo->Map.GetItem( nEnterType, true);
}

void SceneInfo::FillSceneCreateArg(SceneCreateArg& obj) const
{
	obj.m_nSceneSID = m_nSceneSID;
	obj.m_SceneID = GenSceneID(m_nSceneSID);
}

EnterPointInfo::EnterPointInfo(DBRow& row)
{
	static const int32 nCol_ID = row.GetColumnIdx("ID");
	static const int32 nCol_FromScene = row.GetColumnIdx("FromScene");
	static const int32 nCol_EnterType = row.GetColumnIdx("EnterType");
	static const int32 nCol_EnterMode = row.GetColumnIdx("EnterMode");
	static const int32 nCol_X = row.GetColumnIdx("x");
	static const int32 nCol_Y = row.GetColumnIdx("y");
	static const int32 nCol_Z = row.GetColumnIdx("z");
	static const int32 nCol_Range = row.GetColumnIdx("range");
	static const int32 nCol_Dx = row.GetColumnIdx("dx");
	static const int32 nCol_Dy = row.GetColumnIdx("dy");
	static const int32 nCol_Dz = row.GetColumnIdx("dz");

	row.Fill( nSceneID, nCol_ID, 0);
	row.Fill( nFromSceneID, nCol_FromScene, 0);
	row.Fill( nEnterType, nCol_EnterType, eEnterType_Default);
	row.Fill( nEnterMode, nCol_EnterMode, eEnterMode_PosDir);

	row.Fill( vPos, nCol_X, nCol_Y, nCol_Z);
	row.Fill( fRange, nCol_Range, 0.0f);
	row.Fill( vDir, nCol_Dx, nCol_Dy, nCol_Dz);
}

Vector3 PointInfo::GetRandPos() const
{
	f32 x = 0;
	f32 y = 0;

	if( fRange > 0)
	{
		x = (double)rand()/(RAND_MAX +1) * 2.0 * fRange - fRange;
		y = (double)rand()/(RAND_MAX +1) * 2.0 * fRange - fRange;
	}

	return Vector3( vPos.x + x, vPos.y + y, vPos.z);
}


EnterInfo::EnterInfo(DBRow& row)
{

}

EnterInfo::~EnterInfo()
{

}
