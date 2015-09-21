#pragma once


#include "common.h"

namespace cmt {

	class Fusion {

	//-----------------------------------------------------------------
	public:
	//-----------------------------------------------------------------

		BOOL preferFirst(
			const vector<Point2f>&	pointsFirst,
			const vector<int>&		classesFirst,
			const vector<Point2f>&	pointsSecond,
			const vector<int>&		classesSecond,
			vector<Point2f>&		pointsFused,
			vector<int>&			classesFused
			);

	};

} // namespace CMT
