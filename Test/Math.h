#pragma once

#include "BaseType.h"
#include <vector>

namespace Math{

	bool PointInRegion(const Point2& point, std::vector<Point2>& regionPts);
}
