#include "stdafx.h"
#include "fusion.h"


namespace cmt {

	BOOL Fusion::preferFirst(
		const vector<Point2f>&	pointsFirst,
		const vector<int>&		classesFirst,
		const vector<Point2f>&	pointsSecond,
		const vector<int>&		classesSecond,
		vector<Point2f>&		pointsFused,
		vector<int>&			classesFused
		){

		if	(
				// (pointsFirst.size() == 0) ||
				(pointsSecond.size() == 0) ||
				(pointsSecond.size() != classesSecond.size()) ||
				(pointsFirst.size() != classesFirst.size())
			)
		{
			return FALSE;
		}

		pointsFused = pointsFirst;
		classesFused = classesFirst;
		
		for (register UINT id2nd = 0; id2nd < pointsSecond.size(); ++id2nd)
		{
			register int secondClass = classesSecond[id2nd];

			bool bOverlap = false;
			for (register UINT id1st = 0; id1st < pointsFirst.size(); id1st++)
			{
				register int firstClass = classesFirst[id1st];
				if (firstClass == secondClass) bOverlap = true;
			}

			if (!bOverlap)
			{
				pointsFused.push_back(pointsSecond[id2nd]);
				classesFused.push_back(secondClass);
			}
		}

		return TRUE;
	}

} // namespace cmt
