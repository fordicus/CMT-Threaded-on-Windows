
// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// cmtDlg.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"


//-----------------------------------------------------------------------------------
VideoCapture				g_cap;
VideoWriter					g_writer;
BOOL						g_runningONE;
vector<KeyPoint>			g_keypoint;
BOOL						g_testDetector = FALSE;
UINT						g_selDetector;
Ptr<FeatureDetector>		g_detector;
//-----------------------------------------------------------------------------------
string						g_wnd;
Mat							g_frm, g_frmToShow,
							g_frmGray, g_frmGrays[maxThreads];
vector<vector<KeyPoint>>	g_keypoints;
Rect2f						g_Rect;

BOOL						g_isScale = FALSE;
BOOL						g_isRotation = TRUE;
FLOAT						g_distCoeff, g_distThresh;
string						g_notePath;
INT 						g_captureStat;

vector<vector<Point2f>>		g_pointsActive;
Point2f						g_verticles[maxThreads][4];

UINT						g_IdxThread;
BOOL						g_isRedraw;
//-----------------------------------------------------------------------------------
CWinThread*					pThreads[maxThreads];
CEvent						warpEvents[maxThreads];
CEvent						controlEvents[maxThreads];
CEvent						allowProcess[maxThreads];
CEvent						readyToDraw[maxThreads];
UINT						posChange[maxThreads];
UINT						retryCnt[maxThreads];
INT							threadStat[maxThreads];
INT							threadStatOld[maxThreads];
//-----------------------------------------------------------------------------------
