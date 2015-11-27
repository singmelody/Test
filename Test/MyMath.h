#pragma once

#include "BaseType.h"
#include <vector>
#include "MyVector.h"
namespace Math{

	bool PointInRegion(const Point2& point, std::vector<Point2>& regionPts);
	bool LineCrossPloygon( const ShapeLine2D& line, const std::vector<Point2>& regionPts);
	bool LineCross(const ShapeLine2D& line0, const ShapeLine2D& line1);
}
