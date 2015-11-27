#include "StdAfx.h"
#include "Math.h"

namespace Math{


	bool PointInRegion(const Point2& point, std::vector<Point2>& regionPts)
	{
		int32 nCount = regionPts.size();
		if(nCount < 3)
			return false;

		int32 cross = 0;

		for (int32 i = 0; i < nCount; ++i)
		{
			const Point2& pt0 = regionPts[i];
			const Point2& pt1 = regionPts[(i+1)%nCount];

			if( ( ((pt0.y <= point.y) && (pt1.y > point.y)) || ((pt1.y <= point.y) && (pt0.y > point.y)))
				&& (point.x < (pt0.x - pt1.x) * (point.y - pt1.y) / (pt0.y - pt1.y) + pt1.x)
				)
			{
				cross = !cross;
			}
		}

		return ( 0 != cross);
	}

}