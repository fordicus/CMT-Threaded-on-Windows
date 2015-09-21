#pragma once


#include "common.h"
#include "matcher.h"
#include "tracker.h"
#include "fusion.h"
#include "consensus.h"

namespace cmt {

	class CMT {

	public:

		CMT(const UINT& myID, const Ptr<FeatureDetector>& detector,
			const DOUBLE& distCOEFF, const DOUBLE& distThresh,
			const string& notePath
			){
			this->myID = myID;
			this->detector = detector;
			this->isInit = FALSE;
			this->isRetrying = FALSE;

			this->frmCnt = 0;	// Currently not in use
			this->distCoeff = distCOEFF;
			this->distThresh = distThresh;
			this->posGrowth = 0;
			
			this->notePath = notePath;
			this->notePath += LocalTimeCode();
			this->notePath += " Active Mass F";
			this->notePath += to_string(1 + this->myID);
			this->notePath += ".txt";
			
			this->noteFile.open(this->notePath);
		};

		~CMT()
		{
			this->noteFile.close();
		};


		//----------------------------------------------------------------------------
		VOID supplyKeyPoints(const vector<KeyPoint>& keypoints);
		VOID configDist(const BOOL& distCoeff, const BOOL& g_distThresh);

		VOID initialize(const Mat& imGray, const Rect& rect);
		BOOL processFrame(const Mat& imGray);
		
		BOOL determineFailure(Point2f& center);
		BOOL determineFailure(Point2f verticle[]);
		VOID recoverArchive(VOID);
		VOID distJournal(VOID);


		//----------------------------------------------------------------------------
		Matcher						matcher;
		Tracker						tracker;
		Fusion						fusion;		
		Consensus					consensus;
				
		Mat							imPrev;
		Point2f						center;
		Size2f						sizeInitial;
		vector<Point2f>				pointsActive;
		vector<INT>					classesActive;
		RotatedRect					bbRotated;


		//----------------------------------------------------------------------------
		UINT						myID;
		BOOL						isInit;
		BOOL						isRetrying;
		
		INT							xLimit, yLimit;
		BOOL						isVisible;
		Mat							imArchive;
		vector<Point2f>				pointsArchive;
		vector<INT>					classesArchive;

		UINT						frmCnt;
		ofstream					noteFile;
		string						notePath;


		//----------------------------------------------------------------------------
		DOUBLE						posChange;
		INT							posGrowth;

		Point2f						posCurrent, posPrev;
	
	
	private:

		Ptr<FeatureDetector>		detector;
		Ptr<DescriptorExtractor>	descriptor;
		vector<KeyPoint>			keypoints;

		DOUBLE						distCoeff, distThresh;

	};

} // namespace CMT
