#include "stdafx.h"
#include "tracker.h"


namespace cmt {

	BOOL Tracker::track(
		const Mat&				imPrev,
		const Mat&				imGray,
		const vector<Point2f>&	pointsPrev,
		vector<Point2f>&		pointsTracked,
		vector<UCHAR>&			status
		){

		if (pointsPrev.size() > 0)
		{
			vector<float> err;

			//------------------------------------------------------------------------------------------
			// Calculate [Forward Optical Flow]
			//------------------------------------------------------------------------------------------
			calcOpticalFlowPyrLK(imPrev, imGray, pointsPrev, pointsTracked, status, err);

			//------------------------------------------------------------------------------------------
			// Calculate [Backward Optical Flow]
			//------------------------------------------------------------------------------------------
			vector<Point2f>	pointsBack;
			vector<UCHAR> statusBack;
			vector<float> errBack;

			calcOpticalFlowPyrLK(imGray, imPrev, pointsTracked, pointsBack, statusBack, errBack);


			//------------------------------------------------------------------------------------------
			// Remove [Mal-tracked points]
			//------------------------------------------------------------------------------------------
			for (register UINT it = pointsPrev.size() - 1; it > 0; --it)
			{
				float l2norm = norm(pointsBack[it] - pointsPrev[it]);
				bool bPointsFly = l2norm > thrPointsFly;

				if (bPointsFly || !status[it] || !statusBack[it])
				{
					pointsTracked.erase(pointsTracked.begin() + it);
					status[it] = 0;
				}
			}

			return TRUE;
		}

		else
		{
			return FALSE;
		}
	}

} // namespace cmt
