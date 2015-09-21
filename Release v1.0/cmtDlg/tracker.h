#pragma once


#include "common.h"

namespace cmt {

	class Tracker {
	//-----------------------------------------------------------------
	public:
	//-----------------------------------------------------------------

		Tracker() : thrPointsFly(30) {};

		BOOL track(
			const Mat&				imPrev,
			const Mat&				imGray,
			const vector<Point2f>&	pointsPrev,
			vector<Point2f>&		pointsTracked,
			vector<UCHAR>&			status
			);


	//-----------------------------------------------------------------
	private:
	//-----------------------------------------------------------------

		float thrPointsFly;

	};

} // namespace CMT
