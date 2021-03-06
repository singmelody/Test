#pragma once

enum CommonDataType
{
	eCommonData_Min				= -1,
	eCommonData_Pet				= 0,
	eCommonData_Buff			= 1,
	eCommonData_Item			= 3,
	eCommonData_SceneProcess	= 10,
	eCommonData_Max
};

enum CommonDataMask
{
	eCommonDataMask_Pet		= ( 1 << eCommonData_Pet),
	eCommonDataMask_Buff	= ( 1 << eCommonData_Buff),
	eCommonDataMask_All		= ( 1 << eCommonData_Max ) - 1,
};

class CommonDataOwner;
class CommonDataObject;

class CommonDataCont
{
public:
	CommonDataCont(CommonDataType type) : m_Type(type) { m_pOwner = NULL; }
	virtual ~CommonDataCont(void){}

	virtual void				SetOwner(CommonDataOwner* pOwner) { m_pOwner = pOwner; }
	CommonDataType				GetType() { return m_Type; }

	CommonDataObject*			CreateCommonDataObj(int32 nDataID, int32 nIdx = -1);

	virtual	int32				Add(CommonDataObject* pObj, int32 nIdx = -1) = 0;
	virtual bool				Remove(int32 nIdx, bool bDel = true) = 0;


	virtual CommonDataObject*	First() = 0;
	virtual CommonDataObject*	Next() = 0;
	virtual CommonDataObject*	Get(int32 nIdx) const = 0;
	virtual CommonDataObject*	GetNext( CommonDataObject* pObj = NULL ) = 0;
	virtual int32				GetMaxSize() const = 0;

	virtual CommonDataObject*	NewObject(int32 nDataID) = 0;
	virtual void				FreeObject(CommonDataObject* pObj) = 0;
protected:
	int32				m_nMaxIdx;
	CommonDataOwner*	m_pOwner;
	CommonDataType		m_Type;

};

