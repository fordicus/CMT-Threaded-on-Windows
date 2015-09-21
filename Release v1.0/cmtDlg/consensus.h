#pragma once


#include "common.h"

namespace cmt {

	class Consensus {
	
	public:
	
		Consensus() : isScale(FALSE), isRotation(TRUE), thrCutoff(20) {};

		VOID configDeform(const BOOL& isScale, const BOOL& isRotation);

		VOID initialize(
			const vector<Point2f>& vectorsFG
			);

		BOOL estimateScaleRotation(
			const vector<Point2f>&	points,
			const vector<int>&		classes,
			float&					scale,
			float&					rotation
			);

		BOOL findConsensus(
			const vector<Point2f>&	points,
			const vector<int>&		classes,
			const float&			scale,
			const float&			rotation,
			Point2f&				center,
			vector<Point2f>&		pointsInlier,
			vector<int>&			classesInlier
			);


	private:

		BOOL isScale, isRotation;
		float thrCutoff;
		vector<Point2f> vectorsFG;
		MatND normsPairwise;
		MatND anglesPairwise;

	};

} // namespace cmt
