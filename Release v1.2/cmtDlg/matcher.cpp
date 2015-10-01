#include "stdafx.h"
#include "matcher.h"


namespace cmt {
	
	void Matcher::initialize(
		const vector<Point2f>&	vectorsFG,
		const MatND&			database,
		const vector<int>&		classes,
		const int&				numPointsBG
		){
		
		this->database			= database;				// { descBG ¡ú descFG }
		this->classes			= classes;				// { -1, -1, -1, ..., 0, 1, 2.. }
		this->vectorsFG			= vectorsFG;
		this->numPointsBG		= numPointsBG;
		this->descLength		= database.cols * 8;	// ( knn::distance / descLength ) < thrDist

		bfmatcher = DescriptorMatcher::create("BruteForce-Hamming");
	}

	BOOL Matcher::matchGlobal(
		const vector<KeyPoint>& keypoints,
		const MatND&			descriptors,
		vector<Point2f>&		pointsMatched,
		vector<int>&			classesMatched
		){

		if (keypoints.size() == 0)
		{
			return FALSE;
		}

		vector<vector<DMatch>> matches;
		bfmatcher->knnMatch(descriptors, database, matches, 2);

		for (register UINT it = 0; it < matches.size(); ++it)
		{
			vector<DMatch> m = matches[it];

			float distance1 = m[0].distance / descLength;
			float distance2 =
				m.size() > 1 ? (m[1].distance / descLength) : 1;

			int matchedClass = classes[m[0].trainIdx];

			if (matchedClass == -1) continue;					// isBackground
			if (distance1 > thrDist) continue;					// nonReliable
			if (distance1 / distance2 > thrRatio) continue;		// nonReliable

			pointsMatched.push_back(keypoints[it].pt);
			classesMatched.push_back(matchedClass);
		}

		return TRUE;
	}

	BOOL Matcher::matchLocal(
		const vector<KeyPoint>& keypoints,
		const MatND&			descriptors,
		const float&			scale,
		const float&			rotation,
		const Point2f&			center,
		vector<Point2f>&		pointsMatched,
		vector<int>&			classesMatched
		){

		UINT sizePoints = keypoints.size();

		if (sizePoints == 0)
		{
			return FALSE;
		}

		//------------------------------------------------------------------------------------------
		// Deform initial points
		//------------------------------------------------------------------------------------------
		vector<Point2f> DeformsFG;

		for (register UINT it = 0; it < vectorsFG.size(); ++it)
		{	DeformsFG.push_back(scale * rotate(vectorsFG[it], +rotation));	}

		//------------------------------------------------------------------------------------------
		// Perform local matching
		//------------------------------------------------------------------------------------------
		for (register UINT it = 0; it < sizePoints; ++it)
		{
			// Normalize keypoint with respect to the center
			Point2f locRelevant = keypoints[it].pt - center;

			// Find potential indices for matching
			vector<int> idxPotential;
			for (register UINT IT = 0; IT < DeformsFG.size(); ++IT)
			{
				float l2norm = norm(DeformsFG[IT] - locRelevant);

				if (l2norm < thrCutoff)
					idxPotential.push_back(numPointsBG + IT);
			}

			// If there are no potential matches, continue
			if (idxPotential.size() == 0) continue;

			// Build descriptor matrix and classes from potential indices
			MatND databasePotential =
				MatND(idxPotential.size(), database.cols, database.type());

			for (register UINT IT = 0; IT < idxPotential.size(); ++IT){
				database.row(idxPotential[IT]).copyTo(databasePotential.row(IT));
			}

			// Find distances between descriptors
			vector<vector<DMatch>> matches;
			bfmatcher->knnMatch(descriptors.row(it), databasePotential, matches, 2);

			vector<DMatch> m = matches[0];

			float distance1 =	(m[0].distance / descLength);
			float distance2 = 
				m.size() > 1 ?	(m[1].distance / descLength) : 1;

			if (distance1 > thrDist) continue;
			if (distance1 / distance2 > thrRatio) continue;

			int matchedClass = classes[idxPotential[m[0].trainIdx]];

			pointsMatched.push_back(keypoints[it].pt);
			classesMatched.push_back(matchedClass);
		}

		return TRUE;
	}

} // namespace CMT
