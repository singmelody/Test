#pragma once

#include "BaseType.h"
#include <vector>
#include "MyVector.h"
namespace Math{

#define MIN_FLOAT (0.0001)
#define FLOAT_EQUAL( a, b) (fabs((f32)((a)-(b))) < MIN_FLOAT)
#define FLOAT_EQUAL_ZERO(a) (FLOAT_EQUAL(a,0))

#define MAX( a, b ) (((a) > (b)) ? (a) : (b))
#define MIN( a, b ) (((a) < (b)) ? (a) : (b))

	bool PointInRegion(const Point2& point, std::vector<Point2>& regionPts);
	bool LineCrossPloygon( const ShapeLine2D& line, const std::vector<Point2>& regionPts);
	bool LineCross(const ShapeLine2D& line0, const ShapeLine2D& line1);
}
