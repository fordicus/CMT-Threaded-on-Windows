#pragma once

#include "cvGeneric.hpp"

using namespace cv;
using namespace std;

namespace cmt {

	float median(vector<float>& A);
	Point2f rotate(const Point2f& v, const float& angle);

	template<class T>
	int sgn(T& x)
	{
		if (x >= 0) return 1;
		else return -1;
	}

} // namespace cmt
