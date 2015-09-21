#include "stdafx.h"
#include "cmt.h"


namespace cmt {

	VOID CMT::supplyKeyPoints(const vector<KeyPoint>& keypoints)
	{
		this->keypoints = keypoints;
	}


	VOID CMT::configDist(const BOOL& distCoeff, const BOOL& g_distThresh)
	{
		this->distCoeff = distCoeff;
		this->distThresh = g_distThresh;
	}
	

	VOID CMT::initialize(const Mat& imGray, const Rect& rect)
	{

		//------------------------------------------------------------------------------------------
		// Import the first [imGray] and its [ROI information : center, and size of rect ]
		//------------------------------------------------------------------------------------------
		imPrev = imGray;
		
		xLimit = imGray.cols - 1;
		yLimit = imGray.rows - 1;

		center = Point2f(rect.x + rect.width / 2.0, rect.y + rect.height / 2.0);
		sizeInitial = rect.size();


		//------------------------------------------------------------------------------------------
		// Designate descriptor
		// this->detector : Assigned at object consturctor
		//------------------------------------------------------------------------------------------
		descriptor = BRISK::create();


		//------------------------------------------------------------------------------------------
		// Detect and Segmentate [foreground/background KeyPoints] by bounding box
		//------------------------------------------------------------------------------------------

		vector<KeyPoint> keypointsFG;
		vector<KeyPoint> keypointsBG;

		for (register UINT it = 0; it < this->keypoints.size(); ++it)
		{
			KeyPoint k = this->keypoints[it];
			Point2f pt = k.pt;

			if (pt.x > rect.x && pt.y > rect.y && pt.x < rect.br().x && pt.y < rect.br().y)
				 keypointsFG.push_back(k);
			else keypointsBG.push_back(k);
		}


		//------------------------------------------------------------------------------------------
		// Extract [foreground/background Descriptions]
		// and Build [database] & [classes] for matcher
		//------------------------------------------------------------------------------------------
		MatND descFG, descBG;
		descriptor->compute(imGray, keypointsFG, descFG);
		descriptor->compute(imGray, keypointsBG, descBG);
		
		MatND database;
		if (descBG.rows > 0 && descFG.rows > 0) vconcat(descBG, descFG, database);
		else if (descBG.rows > 0) database = descBG;
		else database = descFG;

		vector<int> classes = vector<int>(descBG.rows, -1);

		vector<int> classesFG;
		for (register UINT it = 0; it < keypointsFG.size(); ++it)
			classesFG.push_back(it);

		classes.insert(classes.end(), classesFG.begin(), classesFG.end());


		//------------------------------------------------------------------------------------------
		// Extract and Reserve [cv::Point2f] from [cv::KeyPoint]
		//------------------------------------------------------------------------------------------
		vector<Point2f> pointsFG;

		for (register UINT it = 0; it < keypointsFG.size(); ++it)
			pointsFG.push_back(keypointsFG[it].pt);


		//------------------------------------------------------------------------------------------
		// Calculate Foreground Vectors ( Constellation Origin : Center of ROI )
		//------------------------------------------------------------------------------------------		
		vector<Point2f> vectorsFG;
		for (register UINT it = 0; it < pointsFG.size(); ++it)
			vectorsFG.push_back(pointsFG[it] - center);


		//------------------------------------------------------------------------------------------
		// Initialize [matcher] and [consensus]
		//------------------------------------------------------------------------------------------
		matcher.initialize(
			vectorsFG,
			database,
			classes,
			descBG.rows
			);

		consensus.initialize(vectorsFG);


		//------------------------------------------------------------------------------------------
		// Reserve initial information into Archive
		//------------------------------------------------------------------------------------------
		imArchive = imGray;

		pointsActive.assign(pointsFG.begin(), pointsFG.end());
		pointsArchive.assign(pointsFG.begin(), pointsFG.end());
		
		classesActive.assign(classesFG.begin(), classesFG.end());
		classesArchive.assign(classesFG.begin(), classesFG.end());

		this->posPrev = this->center;

		this->isInit = TRUE;
	}


	BOOL CMT::processFrame(const Mat& imGray)
	{
		//------------------------------------------------------------------------------------------
		// Track keypoints
		//------------------------------------------------------------------------------------------
		vector<Point2f> pointsTracked;
		vector<UCHAR> status;

		this->isVisible = tracker.track(
			imPrev,
			imGray,
			pointsActive,
			pointsTracked,
			status
			);

		if (!this->isVisible)
		{
			this->isRetrying = TRUE;
			return this->isVisible;
		}
				

		//------------------------------------------------------------------------------------------
		// Keep successful classes
		//------------------------------------------------------------------------------------------
		vector<int> classesTracked;
		for (register UINT it = 0; it < classesActive.size(); ++it)
		{
			if (status[it]) classesTracked.push_back(classesActive[it]);
			// else if(status[it] == 0) classesActive[it] : failed to track
		}

		//------------------------------------------------------------------------------------------
		// Compute Descriptors
		// this->keypoints : Supplied by thread
		//------------------------------------------------------------------------------------------				
		MatND descriptors;
		descriptor->compute(imGray, this->keypoints, descriptors);


		//------------------------------------------------------------------------------------------
		// Match keypoints globally
		//------------------------------------------------------------------------------------------
		vector<Point2f> pointsMatchedGlobal;
		vector<int> classesMatchedGlobal;

		this->isVisible = matcher.matchGlobal(
			this->keypoints,
			descriptors,
			pointsMatchedGlobal,
			classesMatchedGlobal
			);
		
		if (!this->isVisible)
		{
			this->isRetrying = TRUE;
			return this->isVisible;
		}

		//------------------------------------------------------------------------------------------
		// pointsFused = { pointsTracked ¡ú pointsMatchedGlobal }
		//------------------------------------------------------------------------------------------
		vector<Point2f> pointsFused;
		vector<int> classesFused;

		if (this->isRetrying)
		{
			pointsTracked.clear();
			classesTracked.clear();
		}


		//-----------------------------------
		// Matches First
		//-----------------------------------
		this->isVisible = fusion.preferFirst(
			pointsMatchedGlobal,
			classesMatchedGlobal,
			pointsTracked,
			classesTracked,
			pointsFused,
			classesFused
			);

		if (!this->isVisible)
		{
			this->isRetrying = TRUE;
			return this->isVisible;
		}

		//------------------------------------------------------------------------------------------
		// Estimate the changes in Scale and Rotation of pointsFused from the initial Constellation
		//------------------------------------------------------------------------------------------
		float scale, rotation;
		
		this->isVisible = consensus.estimateScaleRotation(
			pointsFused,
			classesFused,
			scale,
			rotation
			);

		if (!this->isVisible)
		{
			this->isRetrying = TRUE;
			return this->isVisible;
		}
		

		//------------------------------------------------------------------------------------------
		// Find inliers and the center of their votes
		//------------------------------------------------------------------------------------------
		// votes : pointsFused - DeformsFG
		// votes = pointsFused - scale * rotate(vectorsFG[classes[it]], rotation);
		//------------------------------------------------------------------------------------------
		vector<Point2f> pointsInlier;
		vector<int> classesInlier;
				
		this->isVisible = consensus.findConsensus(
			pointsFused,
			classesFused,
			scale,
			rotation,
			center,
			pointsInlier,
			classesInlier
			);

		if (!this->isVisible)
		{
			this->isRetrying = TRUE;
			return this->isVisible;
		}

		
		//------------------------------------------------------------------------------------------
		// Match keypoints locally
		//------------------------------------------------------------------------------------------
		vector<Point2f> pointsMatchedLocal;
		vector<int> classesMatchedLocal;

		this->isVisible = matcher.matchLocal(
			this->keypoints,
			descriptors,
			scale,
			rotation,
			center,
			pointsMatchedLocal,
			classesMatchedLocal
			);
		
		if (!this->isVisible)
		{
			this->isRetrying = TRUE;
			return this->isVisible;
		}


		//------------------------------------------------------------------------------------------
		// pointsActive = { pointsInlier ¡ú pointsMatchedLocal }
		//------------------------------------------------------------------------------------------
		pointsActive.clear();
		classesActive.clear();

		this->isVisible = fusion.preferFirst(
			pointsMatchedLocal,
			classesMatchedLocal,
			pointsInlier,
			classesInlier,
			pointsActive,
			classesActive
			);
			
		if (!this->isVisible)
		{
			this->isRetrying = TRUE;
			return this->isVisible;
		}


		//------------------------------------------------------------------------------------------
		// Finalization
		//------------------------------------------------------------------------------------------
		bbRotated = RotatedRect(center, sizeInitial * scale, rotation / CV_PI * 180);
		imPrev = imGray;

		//// determineFailure with { verticle && center }
		//Point2f verticle[4];
		//this->bbRotated.points(verticle);
		//BOOL rangeValid = determineFailure(verticle);

		BOOL rangeValid = determineFailure(this->center);

		if (!rangeValid)
		{
			#ifdef DEBUG_PROGRAM
				CString cERROR;
				cERROR.Format(_T("(%.2f, %.2f)(%.2f, %.2f)(%.2f, %.2f)(%.2f, %.2f)(%.2f, %.2f)"),
					this->center.x, this->center.y,
					verticle[0].x, verticle[0].y,
					verticle[1].x, verticle[1].y,
					verticle[2].x, verticle[2].y,
					verticle[3].x, verticle[3].y
					);
				AfxMessageBox(cERROR);
			#endif

			return rangeValid;
		}

		this->distJournal();
		this->isRetrying = FALSE;
		return this->isVisible;
	}


	BOOL CMT::determineFailure(Point2f& center)
	{
		BOOL rangeValid;

		if	(
			(center.x < 0) || (center.x > this->xLimit) ||
			(center.y < 0) || (center.y > this->yLimit)
			)
		{
			rangeValid = FALSE;
		}

		else { rangeValid = TRUE; }

		return rangeValid;
	}


	BOOL CMT::determineFailure(Point2f verticle[])
	{
		BOOL rangeValid;

		if	(
			(this->center.x < 0) || (this->center.x > this->xLimit) ||
			(this->center.y < 0) || (this->center.y > this->yLimit) ||
			(verticle[0].x < 0) || (verticle[0].x > this->xLimit) ||
			(verticle[0].y < 0) || (verticle[0].y > this->yLimit) ||
			(verticle[1].x < 0) || (verticle[1].x > this->xLimit) ||
			(verticle[1].y < 0) || (verticle[1].y > this->yLimit) ||
			(verticle[2].x < 0) || (verticle[2].x > this->xLimit) ||
			(verticle[2].y < 0) || (verticle[2].y > this->yLimit) ||
			(verticle[3].x < 0) || (verticle[3].x > this->xLimit) ||
			(verticle[3].y < 0) || (verticle[3].y > this->yLimit)
			)
		{
			rangeValid = FALSE;
		}

		else rangeValid = TRUE;

		return rangeValid;
	}


	VOID CMT::recoverArchive(VOID)
	{
		this->pointsActive.clear();
		this->pointsActive.assign(this->pointsArchive.begin(), this->pointsArchive.end());

		this->classesActive.clear();
		this->classesActive.assign(this->classesArchive.begin(), this->classesArchive.end());

		this->imPrev = this->imArchive;
	}


	VOID CMT::distJournal(VOID)
	{
		this->posCurrent = this->center;
		this->posChange = this->distCoeff * norm(posCurrent - posPrev);
		if (posChange < this->distThresh) { posChange = 0; }
		this->posGrowth += this->posChange;
		this->posPrev = this->posCurrent;

		this->noteFile << LocalTime(TRUE);
		if (posChange == 0) { this->noteFile << "\n"; }
		else { this->noteFile << "\t" << to_string(posChange) << "\n"; }
	}

} // namespace CMT
