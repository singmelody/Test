#include "StdAfx.h"
#include "MyMath.h"
namespace Math{


	bool PointInPloygon(const Point2& point, std::vector<Point2>& regionPts)
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

	bool LineCrossPloygon( const ShapeLine2D& line, const std::vector<Point2>& regionPts)
	{
		if(regionPts.size() <= 0)
			return false;

		int32 nCount = regionPts.size();

		ShapeLine2D temp;
		for (int32 i = 0; i < nCount; ++i)
		{
			temp.p0 = regionPts[i];
			temp.p1 = regionPts[(i+1)%nCount];

			if(LineCross( line, temp) && LineCross( temp, line))
				return true;
		}

		return false;
	}

	bool LineCross(const ShapeLine2D& line0, const ShapeLine2D& line1)
	{
		const Vector2& vStart = line0.p0;
		Vector2 vS = line0.p1 - vStart;
		Vector2 v1 = line1.p0 - vStart;
		Vector2 v2 = line1.p1 - vStart;

		vS.Normalize();
		v1.Normalize();
		v2.Normalize();

		float ret1 = vS.Cross(v1);
		float ret2 = vS.Cross(v2);

		if( ( ret1 > 0 && ret2 >= 0) || (ret1 <0 && ret2 <= 0))
			return false;

		return true;
	}

}