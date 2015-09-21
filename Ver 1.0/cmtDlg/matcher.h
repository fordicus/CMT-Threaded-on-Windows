#pragma once


#include "common.h"

namespace cmt {

	class Matcher {

	//-----------------------------------------------------------------
	public:
	//-----------------------------------------------------------------
		
		Matcher() : thrDist(0.25), thrRatio(0.8), thrCutoff(20) {};
		
		void initialize(
			const vector<Point2f>&	vectorsFG,
			const MatND&			database,
			const vector<int>&		classes,
			const int&				numPointsBG
			);

		BOOL matchGlobal(
			const vector<KeyPoint>& keypoints,
			const MatND&			descriptors,
			vector<Point2f>&		pointsMatched,
			vector<int>&			classesMatched
			);

		BOOL matchLocal(
			const vector<KeyPoint>& keypoints,
			const MatND&			descriptors,
			const float&			scale,
			const float&			rotation,
			const Point2f&			center,
			vector<Point2f>&		pointsMatched,
			vector<int>&			classesMatched
			);


	//-----------------------------------------------------------------
	private:
	//-----------------------------------------------------------------

		vector<Point2f> vectorsFG;
		MatND database;
		vector<int> classes;
		int descLength;
		int numPointsBG;
		Ptr<DescriptorMatcher> bfmatcher;
		float thrDist;
		float thrRatio;
		float thrCutoff;

	};

} // namespace CMT
