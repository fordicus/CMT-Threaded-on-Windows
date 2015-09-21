#include "stdafx.h"
#include "consensus.h"
#include "fastcluster.h"


namespace cmt {

	VOID Consensus::configDeform(const BOOL& isScale, const BOOL& isRotation)
	{
		this->isScale = isScale;
		this->isRotation = isRotation;
	}


	//---------------------------------------------------------------------------------------------------
	// Calculate correspondences from foreground Constellation
	// Correlation of 2 vectors : normsPairwise, anglesPairwise
	//---------------------------------------------------------------------------------------------------
	void Consensus::initialize(const vector<Point2f>& vectorsFG)
	{
		this->vectorsFG = vectorsFG;
		UINT numPointsFG = vectorsFG.size();

		this->normsPairwise = MatND(Size(numPointsFG, numPointsFG), CV_32FC1);
		this->anglesPairwise = MatND(Size(numPointsFG, numPointsFG), CV_32FC1);

		for (register UINT i = 0; i < numPointsFG; ++i)
		{
			for (register UINT j = 0; j < numPointsFG; ++j)
			{
				Point2f v = vectorsFG[i] - vectorsFG[j];

				float dist = norm(v);
				float angleCurrent = atan2(v.y, v.x);

				this->normsPairwise.at<float>(i, j) = dist;
				this->anglesPairwise.at<float>(i, j) = angleCurrent;
			}
		}
	}


	//---------------------------------------------------------------------------------------------------
	// Estimate the changes in Scale and Rotation of pointsFused from the initial Constellation
	//---------------------------------------------------------------------------------------------------
	BOOL Consensus::estimateScaleRotation(
		const vector<Point2f>&	points,
		const vector<int>&		classes,
		float&					scale,
		float&					rotation
		){

		vector<float> changesScale, changesAngle;

		UINT sizePoints = points.size();
		if (sizePoints == 0)
		{
			return FALSE;
		}

		for (register UINT i = 0; i < sizePoints; ++i)
		{
			for (register UINT j = 0; j < sizePoints; j++)
			{
				if (classes[i] != classes[j])
				{
					Point2f v = points[i] - points[j];

					if (isScale)
					{
						float distCurrent = norm(v);
						float distInitial = this->normsPairwise.at<float>(classes[i], classes[j]);
						float changeScale = distCurrent / distInitial;
						changesScale.push_back(changeScale);
					}

					if (isRotation)
					{
						float angleCurrent = atan2(v.y, v.x);
						float angleInitial = this->anglesPairwise.at<float>(classes[i], classes[j]);
						float changeAngle = angleCurrent - angleInitial;

						if (fabs(changeAngle) > CV_PI) // Fix long-way anglesPairwise
							changeAngle = sgn(changeAngle) * 2 * CV_PI + changeAngle;

						changesAngle.push_back(changeAngle);
					}
				}
			}
		}

		if (changesScale.size() < 2) scale = 1;
		else scale = median(changesScale);

		if (changesAngle.size() < 2) rotation = 0;
		else rotation = median(changesAngle);

		return TRUE;
	}


	BOOL Consensus::findConsensus(
		const vector<Point2f>&	points,
		const vector<int>&		classes,
		const float&			scale,
		const float&			rotation,
		Point2f&				center,
		vector<Point2f>&		pointsInlier,
		vector<int>&			classesInlier
		){

		UINT sizePoints = points.size();

		//---------------------------------------------------------------------------------------------------
		// If no points are available, reteurn nan
		//---------------------------------------------------------------------------------------------------
		if (sizePoints == 0)
		{
			center.x = numeric_limits<float>::quiet_NaN();
			center.y = numeric_limits<float>::quiet_NaN();
			
			return FALSE;
		}


		//---------------------------------------------------------------------------------------------------
		// Compute votes
		//---------------------------------------------------------------------------------------------------
		vector<Point2f> votes(points.size());

		for (register UINT it = 0; it < sizePoints; ++it)
		{	// votes : pointsFused - DeformsFG
			votes[it] = points[it] - scale * rotate(vectorsFG[classes[it]], rotation);
		}

		t_index N = sizePoints;

		//This is a lot of memory, so we put it on the heap
		float* D = new float[N*(N - 1) / 2];

		cluster_result Z(N - 1);

		//---------------------------------------------------------------------------------------------------
		// Compute pairwise distances between votes
		//---------------------------------------------------------------------------------------------------
		register int index = 0;
		for (register UINT it = 0; it < sizePoints; ++it)
		{
			for (register UINT j = it + 1; j < sizePoints; j++)
			{
				//TODO: This index calculation is correct, but is it a good thing?
				//int index = it * (points.size() - 1) - (it*it + it) / 2 + j - 1;
				D[index] = norm(votes[it] - votes[j]);
				index++;
			}
		}
		MST_linkage_core(N, D, Z);

		union_find nodes(N);

		//---------------------------------------------------------------------------------------------------
		// Sort linkage by distance ascending
		//---------------------------------------------------------------------------------------------------
		std::stable_sort(Z[0], Z[N - 1]);

		//S are cluster sizes
		int* S = new int[2 * N - 1];

		// TODO: Why does this loop go to 2*N-1?
		// Shouldn't it be simply N? Everything > N gets overwritten later
		for (register int it = 0; it < (2*N - 1); ++it) S[it] = 1;

		// After the loop ends, parent contains the index of the last cluster
		t_index parent = 0;
		for (node const * NN = Z[0]; NN != Z[N - 1]; ++NN)
		{
			// Get two data points whose clusters are merged in step it.
			// Find the cluster identifiers for these points.
			t_index node1 = nodes.Find(NN->node1);
			t_index node2 = nodes.Find(NN->node2);

			// Merge the nodes in the union-find data structure by making them
			// children of a new node
			// if the distance is appropriate
			if (NN->dist < thrCutoff)
			{
				parent = nodes.Union(node1, node2);
				S[parent] = S[node1] + S[node2];
			}
		}

		//---------------------------------------------------------------------------------------------------
		// Get cluster labels
		//---------------------------------------------------------------------------------------------------
		int* T = new int[N];
		for (t_index it = 0; it < N; ++it)
			T[it] = nodes.Find(it);

		// Find largest cluster
		int S_max = distance(S, max_element(S, S + 2 * N - 1));

		// Find inliers, compute center of votes
		pointsInlier.reserve(S[S_max]);
		classesInlier.reserve(S[S_max]);
		center.x = center.y = 0;

		for (register UINT it = 0; it < sizePoints; ++it)
		{
			// If point is in consensus cluster
			if (T[it] == S_max)
			{
				pointsInlier.push_back(points[it]);
				classesInlier.push_back(classes[it]);
				center.x += votes[it].x;
				center.y += votes[it].y;
			}
		}

		center.x /= pointsInlier.size();
		center.y /= pointsInlier.size();

		delete[] D;
		delete[] S;
		delete[] T;

		return TRUE;
	}

} // namespace cmt
