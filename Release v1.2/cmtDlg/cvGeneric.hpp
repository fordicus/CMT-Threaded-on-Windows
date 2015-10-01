// Robinus Fuser. 2015-09-25 06:15
// Electronics Dept. Konkuk Univ. South Korea
// fordicus@naver.com

/*
	GlobalInits()
	SecureEndThread()
	GlobalExits()

	QueryActiveThread()
	QueryThreadToEnd()
	ReserveThreadStat()
	QueryThreadChange()

	QueryWinUserName()
	LocalTime()
	LocalTimeMS()

	class threadINFO
	class keyINFO
	class cvGENERIC
*/


#pragma once
#include <opencv2\opencv.hpp>
#include "cvColorDef.hpp"
#include "gcPreprocess.hpp"
#include "gcProfile.hpp"

using namespace cv;


//============================================================================
// Global Variables & Definitions
//----------------------------------------------------------------------------
// refer to "stdafx.cpp"
//============================================================================
extern VideoCapture				g_cap;
extern VideoWriter				g_writer;
extern BOOL						g_runningONE;
extern vector<KeyPoint>			g_keypoint;
extern BOOL						g_testDetector;
extern UINT						g_selDetector;
extern Ptr<FeatureDetector>		g_detector;
#define							detector_GFTT		0
#define							detector_FAST		1
//----------------------------------------------------------------------------
extern string					g_wnd;
extern Mat						g_frm, g_frmToShow, 
								g_frmGray, g_frmGrays[maxThreads];
extern vector<vector<KeyPoint>>	g_keypoints;
extern Rect2f					g_Rect;

extern BOOL						g_isScale;
extern BOOL						g_isRotation;
extern FLOAT					g_distCoeff, g_distThresh;
extern string					g_notePath;
extern INT 						g_captureStat;
#define							capture_IDLE		-1
#define							capture_RUNNING		1

extern vector<vector<Point2f>>	g_pointsActive;
extern Point2f					g_verticles[maxThreads][4];

extern UINT						g_IdxThread;
extern BOOL						g_isRedraw;

extern CWinThread*				pThreads[maxThreads];
extern CEvent					warpEvents[maxThreads];
extern CEvent					controlEvents[maxThreads];
extern CEvent					allowProcess[maxThreads];
extern CEvent					readyToDraw[maxThreads];
extern UINT						posChange[maxThreads];
extern UINT						retryCnt[maxThreads];
#define							maximum_RETRY		10

extern INT						threadStat[maxThreads];
extern INT						threadStatOld[maxThreads];
#define							thread_VOID			-1
#define							thread_IDLE			0
#define							thread_ACTIVE		1


//============================================================================
// Global Functions
//============================================================================
VOID inline GlobalInits(VOID)
{
	g_captureStat = capture_IDLE;
	g_testDetector = FALSE;

	vector<Point2f> vecPoint_VOID;
	Point2f aPoint2f;
	vecPoint_VOID.push_back(aPoint2f);

	vector<KeyPoint> vecKeyPoint_VOID;	
	KeyPoint aKeyPoint;
	vecKeyPoint_VOID.push_back(aKeyPoint);


	for (register UINT id = 0; id < maxThreads; ++id)
	{
		g_keypoints.push_back(vecKeyPoint_VOID);
		g_pointsActive.push_back(vecPoint_VOID);	// Access from threadCMT()
		pThreads[id] = NULL;
		warpEvents[id].ResetEvent();
		controlEvents[id].ResetEvent();
		allowProcess[id].ResetEvent();
		readyToDraw[id].ResetEvent();
		threadStat[id] = thread_VOID;
		retryCnt[id] = 0;
	}
}


vector<UINT> inline QueryThreadToEnd(INT threadStat[])
{
	vector<UINT> nonVOID;

	for (register UINT id = 0; id < maxThreads; ++id)
		if (threadStat[id] != thread_VOID) nonVOID.push_back(id);

	return nonVOID;
}



VOID inline SecureEndThread(INT threadStat[])
{
	vector<UINT> ThreadToEnd = QueryThreadToEnd(threadStat);

	while (ThreadToEnd.size() > 0)
	{
		for (register UINT id = 0; id < ThreadToEnd.size(); ++id)
			warpEvents[ThreadToEnd[id]].SetEvent();

		ThreadToEnd = QueryThreadToEnd(threadStat);
	}
}


VOID inline GlobalExits(VOID)
{
	g_writer.release();
	g_cap.release();
	g_captureStat = capture_IDLE;
	SecureEndThread(threadStat);
	cv::destroyAllWindows();
	g_runningONE = FALSE;
}


vector<UINT> inline QueryActiveThread(INT threadStat[])
{
	vector<UINT> ACTIVE;

	for (register UINT id = 0; id < maxThreads; ++id)
		if (threadStat[id] == thread_ACTIVE) ACTIVE.push_back(id);

	return ACTIVE;
}


VOID inline ReserveThreadStat(INT threadStat[], INT threadStatOld[])
{
	for (register UINT id = 0; id < maxThreads; ++id)
		threadStatOld[id] = threadStat[id];
}


BOOL inline QueryThreadChange(INT threadStat[], INT threadStatOld[])
{
	BOOL isChanged = FALSE;

	for (register UINT id = 0; id < maxThreads; ++id)
		if (threadStat[id] != threadStatOld[id]) isChanged = TRUE;

	return isChanged;
}


CString inline QueryWinUserName(VOID)
{
	DWORD dwCnt = 0;
	LPTSTR pStr = NULL;

	WTSQuerySessionInformation(
		WTS_CURRENT_SERVER_HANDLE,
		WTS_CURRENT_SESSION,
		WTSUserName,
		&pStr,
		&dwCnt);

	CString winUserName = pStr;
	WTSFreeMemory(pStr);
	
	return winUserName;
}


string inline LocalTime(BOOL bShowMS)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	stringstream ss;
	string time;

	ss << systime.wYear << "."
		<< systime.wMonth << "."
		<< systime.wDay;

#ifdef showDay
	string day;
	switch (systime.wDayOfWeek){
	case 0:	day = "Sun";	break;
	case 1:	day = "Mon";	break;
	case 2:	day = "Tue";	break;
	case 3:	day = "Wed";	break;
	case 4:	day = "Thur";	break;
	case 5:	day = "Fri";	break;
	case 6:	day = "Sat";	break;
	}
	ss << "." << day;
#endif

	ss << " " << systime.wHour << ":"
		<< systime.wMinute << ":"
		<< systime.wSecond;

	if (bShowMS) ss << "(" << systime.wMilliseconds << ")";

	time = ss.str();
	return time;
}


string inline LocalTimeMS(VOID)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	stringstream ss;
	string time;

	ss << systime.wYear
		<< systime.wMonth
		<< systime.wDay;

#ifdef showDay
	string day;
	switch (systime.wDayOfWeek){
	case 0:	day = "Sun";	break;
	case 1:	day = "Mon";	break;
	case 2:	day = "Tue";	break;
	case 3:	day = "Wed";	break;
	case 4:	day = "Thur";	break;
	case 5:	day = "Fri";	break;
	case 6:	day = "Sat";	break;
	}
	ss << "." << day;
#endif

	ss << systime.wHour
		<< systime.wMinute
		<< systime.wSecond
		<< systime.wMilliseconds;

	time = ss.str();
	return time;
}


//============================================================================
// [MFC] threadINFO
//============================================================================
class threadINFO
{
public :
	int job; // Raw(-1) : Create(0) : Destruct(1) : Control(2)
	int who;	
	string name;	

	#define tRAW		-1
	#define tCREATE		0
	#define tDESTRUCT	1
	#define tCONTROL	2

	threadINFO()
	{
		this->job = tRAW;
		this->who = tRAW;
		this->name = "";		
	}

	VOID Clear(VOID){
		this->job = tRAW;
		this->who = tRAW;
		this->name = "";		
	}

	template<typename _T>
	VOID Create(_T who, string& name){
		this->job = tCREATE;
		this->who = who;		
		this->name = name;		
	}
	
	template<typename _T>
	VOID Destruct(_T who, string& name){
		this->job = tDESTRUCT;
		this->who = who;
		this->name = name;		
	}

	template<typename _T>
	VOID Control(_T who, string& name){
		this->job = tCONTROL;
		this->who = who;		
		this->name = name;
	}
};


//============================================================================
// [MFC] keyINFO
//============================================================================
class keyINFO
{
public :
	string keyMsg;
	int keyClass;
};


//============================================================================
// [OpenCV - MFC] cvGENERIC
//============================================================================
class cvGENERIC
{	
public :		
	//========================================================================
	// Variables
	//========================================================================
	vector<string>		wnd;
	vector<Mat>			img;
	Size				vidSize;
	//------------------------------------------------------------------------
	string Vpath;		// [ D:\\root\\title.avi	]
	string Vroot;		// [ D:\\root\\				]
	string VsubRoot;	// [ D:\\root\\title\\		] Created Sub-root
	string Vtitle;		// [ title					] without extension
	//------------------------------------------------------------------------
	Mat					surTitleBox;
	BOOL				bCreateDir;
	//------------------------------------------------------------------------
	INT					status;
	#define eEXIT		0
	#define eRUN		1
	//------------------------------------------------------------------------
	#define	kNA			-1
	#define	kNUM		0
	#define	kFUNC		1
	//------------------------------------------------------------------------
private:
	INT					threadsNUM;
	string				threads[maxThreads];


	//========================================================================
	// Functions
	//========================================================================
public:
	cvGENERIC(); ~cvGENERIC();
	
	Ptr<FeatureDetector> Config_GFTT(VOID);
	Ptr<FeatureDetector> Config_FAST(VOID);

	static Scalar PickBrush(UINT& mThreadID);
	static VOID OnMouse(int event, int x, int y, int flags, VOID *param);
	Rect2f GetRect(string& winName, Mat& frm, UINT& threadID);

	string CreateWND(VOID);
	VOID CreateWND(UINT wnd_num);
	VOID PutSurTitle(Mat &panel, string surTitle);

	BOOL LoadImg(Mat &outImg);
	UINT LoadImgs(VOID);
	string LoadVid(BOOL CreatDir);

	BOOL SaveFrms(Mat& frm, UINT& frmCnt);
	BOOL SaveJPG(Mat& img);
	BOOL SavePNG(Mat& img);

	keyINFO ReadKey(UINT delay);

	threadINFO WarpThreadPool(string& threadName);
	threadINFO ControlThread(string& threadNum);
};


//============================================================================
// Constructor & Destructor
//============================================================================
inline cvGENERIC::cvGENERIC()
{
	this->status		= eRUN;
	this->threadsNUM	= 0;

	for (register UINT it = 0; it < maxThreads; ++it)
		this->threads[it] = "";
}

inline cvGENERIC::~cvGENERIC(){}


//============================================================================
// [OpenCV] Config_DETECTOR
//============================================================================
Ptr<FeatureDetector> inline cvGENERIC::Config_GFTT(VOID)
{
	UINT	maxCorners = 700;
	DOUBLE	qualityLevel = 0.001;
	DOUBLE	minDistance = 15.0;
	UINT	blockSize = 3;
	BOOL	useHarrisDetector = FALSE;
	DOUBLE	k = 0.04;

	return cv::GFTTDetector::create(
		maxCorners,				// int maxCorners
		qualityLevel,			// double qualityLevel
		minDistance,			// double minDistance
		blockSize,				// int blockSize
		useHarrisDetector,		// bool useHarrisDetector
		k						// double k
		);
}

Ptr<FeatureDetector> inline cvGENERIC::Config_FAST(VOID)
{
	UINT threshold = 20;
	BOOL nonmaxSuppression = TRUE;
	UINT type = FastFeatureDetector::TYPE_9_16;

	return FastFeatureDetector::create(
		threshold,						// int threshold
		nonmaxSuppression,				// bool nonmaxSuppression
		type							// int type
		);
}


Scalar inline cvGENERIC::PickBrush(UINT& mThreadID)
{
	Scalar BRUSH;

	if (mThreadID == 0) BRUSH = cvBGR_Tomato;
	else if (mThreadID == 1) BRUSH = cvBGR_PaleGreen;
	else if (mThreadID == 2) BRUSH = cvBGR_DeepSkyBlue;
	else if (mThreadID == 3) BRUSH = cvBGR_Violet;
	else if (mThreadID == 4) BRUSH = cvBGR_Orange;
	else if (mThreadID == 5) BRUSH = cvBGR_LemonChiffon;
	else if (mThreadID == 6) BRUSH = cvBGR_Cyan;
	else if (mThreadID == 7) BRUSH = cvBGR_Pink;

	return BRUSH;
}


//----------------------------------------------------------------------------
// Variables		for cvGENERIC::OnMouse
//----------------------------------------------------------------------------
static string		frameToShow;
static Mat			frmPreserve, frmToShow;
static Point2f		mPoint;
static Rect2f		mRect;
static BOOL			mDrag;
static UINT			mThreadID;
static BOOL			bInitROI;


//============================================================================
// [OpenCV] onMouse
//----------------------------------------------------------------------------
// MouseCallback function
// Designed for getRect with "Mouse-dragging"
//============================================================================
VOID inline cvGENERIC::OnMouse(int event, int x, int y, int flags, VOID *param)
{
	Scalar BRUSH = PickBrush(mThreadID);
	
	if (event == CV_EVENT_LBUTTONDOWN && !mDrag)
	{
		mPoint = Point2f(x, y);
		mDrag = TRUE;
	}

	if (event == CV_EVENT_MOUSEMOVE && mDrag)
	{
		frmPreserve.copyTo(frmToShow);
		rectangle(frmToShow, mPoint, Point2f(x, y), BRUSH, 2);
		imshow(frameToShow, frmToShow);
	}

	if (event == CV_EVENT_LBUTTONUP && mDrag)
	{
		mRect = Rect2f(mPoint, Point2f(x, y));

		//----------------------------------------------------
		// Coordinate calibration on Inverse-oriented dragging
		//----------------------------------------------------
		if (mRect.width < 0){
			mRect.x += mRect.width;
			mRect.width = abs(mRect.width);
		}
		if (mRect.height < 0){
			mRect.y += mRect.height;
			mRect.height = abs(mRect.height);
		}

		mDrag = FALSE;
		bInitROI = TRUE;
	}
}


//============================================================================
// [onMouse] getRect
//============================================================================
Rect2f inline cvGENERIC::GetRect(string& winName, Mat& frm, UINT& threadID)
{
	mThreadID = threadID;

	bInitROI		= FALSE;
	mDrag			= FALSE;
	frameToShow		= winName;
	mRect			= Rect2f(-1, -1, -1, -1);

	frm.copyTo(frmPreserve);

	setMouseCallback(winName, OnMouse, NULL);
	while (!bInitROI) waitKey(keyDelay);
	setMouseCallback(winName, NULL, NULL);

	return mRect;
}


//========================================================================
// Multiple cv::NamedWindow Creator /w prefix "wnd_"
//========================================================================
string inline cvGENERIC::CreateWND()
{
	this->wnd.clear();
	this->wnd.push_back("Display"); namedWindow(wnd[0]);
	return this->wnd[0];
}

VOID inline cvGENERIC::CreateWND(UINT wnd_num)
{
	this->wnd.clear();

	for (UINT it = 0; it < wnd_num; ++it){
		stringstream ss;
		ss << "wnd_" << std::setfill('0') << std::setw(2) << it;
		this->wnd.push_back(ss.str()); namedWindow(wnd[it]);
	}
}


//========================================================================
// [OpenCV] Put SurTitle
//========================================================================
VOID inline cvGENERIC::PutSurTitle(Mat &panel, string surTitle)
{
	this->surTitleBox = panel(cv::Rect(0, 0, panel.cols, 30));
	this->surTitleBox.setTo(cvBGR_Black);

	putText(surTitleBox, surTitle,
		Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.6,
		cvBGR_Yellow
		);
}


//========================================================================
// [MFC] Single Image File Loader
//========================================================================
BOOL inline cvGENERIC::LoadImg(Mat &outImg)
{
	//-----------------------------------------------------------
	// [Set CFileDialog Options]
	//-----------------------------------------------------------
	static TCHAR BASED_CODE szFilter[] =
		_T("image | *.bmp; *.jpg; *.png; |")
		_T("jpg | *.jpg; |")
		_T("png | *.png; ||");

	CFileDialog FileDlg(TRUE, 0, 0,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter
		);

	if (FileDlg.DoModal() == IDOK)
	{
		CT2CA Apath(FileDlg.GetPathName());
		outImg = imread(Apath.operator LPSTR());

		CT2CA Aroot(FileDlg.GetFolderPath());
		this->Vroot = Aroot.operator LPSTR();

		stringstream ss;
		ss << Vroot << "\\";
		this->Vroot = ss.str();

		CT2CA Atitle(FileDlg.GetFileTitle());
		this->Vtitle = Atitle.operator LPSTR();
	}

	return !(outImg.empty());
}


//========================================================================
// [MFC] Multiple Image Files Loader
//========================================================================
UINT inline cvGENERIC::LoadImgs()
{
	vector<string> imgPath;
	this->img.clear();

	#define MAX_FILES 10000
	#define BUFFER_SIZE ((MAX_FILES * (MAX_PATH + 1)) + 1)

	//-----------------------------------------------------------
	// [Memory Allocation] wchar_t* = CString
	//-----------------------------------------------------------
	CString CSTRING;
	wchar_t* Linear = CSTRING.GetBuffer(BUFFER_SIZE);
	CSTRING.ReleaseBuffer();

	//-----------------------------------------------------------
	// [Set CFileDialog Options]
	//-----------------------------------------------------------
	static TCHAR BASED_CODE szFilter[] =
		_T("image | *.bmp; *.jpg; *.png; |")
		_T("jpg | *.jpg; |")
		_T("png | *.png; ||");

	CFileDialog FileDlg(TRUE, 0, 0,
		OFN_ALLOWMULTISELECT,
		szFilter
		);

	OPENFILENAME& OFN = FileDlg.GetOFN();
	OFN.nMaxFile = BUFFER_SIZE;
	OFN.lpstrFile = Linear;

	//-----------------------------------------------------------
	// [File Segmentation]
	//-----------------------------------------------------------
	if (FileDlg.DoModal() == IDOK)
	{
		CString ROOT; CString fileName;

		wchar_t* EOL = Linear + BUFFER_SIZE - 2;
		wchar_t* ENTRY = Linear;

		while ((Linear < EOL) && (*Linear))	Linear++;

		if (Linear > ENTRY)
		{	// [ROOT Stage]
			ROOT = ENTRY;
			Linear++;

			// Case of Loading a Single File ------------
			if (!(*Linear)){
				CT2CA CONV(ROOT);
				imgPath.push_back(CONV.operator LPSTR());
			}
			// ------------------------------------------

			while ((Linear < EOL) && (*Linear))
			{
				ENTRY = Linear;
				while ((Linear < EOL) && (*Linear)) Linear++;

				if (Linear > ENTRY)
				{ // [fileName Stage]
					fileName = ROOT; fileName += "\\"; fileName += ENTRY;
					CT2CA CONV(fileName);
					imgPath.push_back(CONV.operator LPSTR());
					Linear++;
				}
			}
		}
	#undef MAX_FILES
	#undef BUFFER_SIZE
	}	// From here use vector<string> imgPath

	for (UINT it = 0; it < imgPath.size(); ++it)
		this->img.push_back(imread(imgPath[it]));

	return this->img.size();
}


//========================================================================
// [MFC] Single Video File Loader
//========================================================================
string inline cvGENERIC::LoadVid(BOOL CreatDir)
{
	LPCTSTR szFilter = L"video | *.mp4; *.avi; ||";
	CFileDialog FileDlg(1, 0, 0, 0, szFilter);

	if (FileDlg.DoModal() == IDOK)
	{
		CT2CA Atitle(FileDlg.GetFileTitle());
		this->Vtitle = Atitle.operator LPSTR();

		CT2CA Apath(FileDlg.GetPathName());
		this->Vpath = Apath.operator LPSTR();

		CT2CA Aroot(FileDlg.GetFolderPath());
		this->Vroot = Aroot.operator LPSTR();

		stringstream ss;
		ss << Vroot << "\\";
		this->Vroot = ss.str();

		ss << Vtitle << "\\";
		this->VsubRoot = ss.str();

		if (CreatDir == TRUE){
			this->bCreateDir = TRUE;
			CA2CT Troot(VsubRoot.c_str());
			::CreateDirectory(Troot, 0);
		}
		else {
			this->bCreateDir = FALSE;
		}
	}

	VideoCapture vid(Vpath);
	if (vid.isOpened())
	{
		Mat frm; 
		vid >> frm;

		this->vidSize = Size(vid.get(3), vid.get(4));
		// 3 : CAP_PROP_FRAME_WIDTH
		// 4 : CAP_PROP_FRAME_HEIGHT
	}

	vid.release();

	return Vpath;
}


//========================================================================
// [OpenCV] Save a Video frame with [ frmCnt ] as postfix
//------------------------------------------------------------------------
// Save into the Video's subFolder >> D:\\root\\title\\title_000000.jpg
// If the subFolder was not created, returns FALSE. Else returns imwrite()
//========================================================================
BOOL inline cvGENERIC::SaveFrms(Mat& frm, UINT& frmCnt)
{
	if (!this->bCreateDir) return FALSE;

	stringstream ss;
	ss << this->VsubRoot << this->Vtitle << "_" << setfill('0') << setw(6)
		<< frmCnt << ".jpg";

	return imwrite(ss.str(), frm);
}


//========================================================================
// [OpenCV] Write a given Image
//------------------------------------------------------------------------
// Write into the same root located by LoadImg() or by LoadVid()
//========================================================================
BOOL inline cvGENERIC::SaveJPG(Mat& img)
{
	if (this->Vroot.empty() || this->Vtitle.empty()) return false;
	
	stringstream ss;
	ss << Vroot << Vtitle << "_new.jpg";

	return imwrite(ss.str(), img);
}

BOOL inline cvGENERIC::SavePNG(Mat& img)
{
	if (this->Vroot.empty() || this->Vtitle.empty()) return false;

	vector<int> pngPOLICY;
	pngPOLICY.push_back(CV_IMWRITE_PNG_COMPRESSION);
	pngPOLICY.push_back(0);

	stringstream ss;
	ss << Vroot << Vtitle << "_new.png";

	return imwrite(ss.str(), img, pngPOLICY);
}


//========================================================================
// [OpenCV] Keyboard Message Handler
//========================================================================
keyINFO inline cvGENERIC::ReadKey(UINT delay)
{
	UINT key = waitKey(delay);
	keyINFO kINFO;

	if (key == 81 || key == 113) { kINFO.keyMsg = "[Q]"; kINFO.keyClass = kNA; }
	else if (key == 82 || key == 114) { kINFO.keyMsg = "[R]"; kINFO.keyClass = kNA; }
	else if (key == 67 || key == 99) { kINFO.keyMsg = "[C]"; kINFO.keyClass = kNA; }
	else if (key == 13) { kINFO.keyMsg = "[ENTER]"; kINFO.keyClass = kNA; }
	else if (key == 32) { kINFO.keyMsg = "[SPACE]"; kINFO.keyClass = kNA; }
	else if (key == 48) { kINFO.keyMsg = "[0]"; kINFO.keyClass = kNA; }
	else if (key == 49) { kINFO.keyMsg = "[1]"; kINFO.keyClass = kNUM; }
	else if (key == 50) { kINFO.keyMsg = "[2]"; kINFO.keyClass = kNUM; }
	else if (key == 51) { kINFO.keyMsg = "[3]"; kINFO.keyClass = kNUM; }
	else if (key == 52) { kINFO.keyMsg = "[4]"; kINFO.keyClass = kNUM; }
	else if (key == 53) { kINFO.keyMsg = "[5]"; kINFO.keyClass = kNUM; }
	else if (key == 54) { kINFO.keyMsg = "[6]"; kINFO.keyClass = kNUM; }
	else if (key == 55) { kINFO.keyMsg = "[7]"; kINFO.keyClass = kNUM; }
	else if (key == 56) { kINFO.keyMsg = "[8]"; kINFO.keyClass = kNUM; }
	else if (key == 57) { kINFO.keyMsg = "[9]"; kINFO.keyClass = kNA; }
	else if (key >= 0 && key < 128) { kINFO.keyMsg = (char)key; kINFO.keyClass = kNA; }
	else if (key == 7340032) { kINFO.keyMsg = "[F1]"; kINFO.keyClass = kFUNC; }
	else if (key == 7405568) { kINFO.keyMsg = "[F2]"; kINFO.keyClass = kFUNC; }
	else if (key == 7471104) { kINFO.keyMsg = "[F3]"; kINFO.keyClass = kFUNC; }
	else if (key == 7536640) { kINFO.keyMsg = "[F4]"; kINFO.keyClass = kFUNC; }
	else if (key == 7602176) { kINFO.keyMsg = "[F5]"; kINFO.keyClass = kFUNC; }
	else if (key == 7667712) { kINFO.keyMsg = "[F6]"; kINFO.keyClass = kFUNC; }
	else if (key == 7733248) { kINFO.keyMsg = "[F7]"; kINFO.keyClass = kFUNC; }
	else if (key == 7798784) { kINFO.keyMsg = "[F8]"; kINFO.keyClass = kFUNC; }
	else if (key == 7864320) { kINFO.keyMsg = "[F9]"; kINFO.keyClass = kNA; }
	else if (key == 7929856) { kINFO.keyMsg = "[F10]"; kINFO.keyClass = kNA; }
	else if (key == 7995392) { kINFO.keyMsg = "[F11]"; kINFO.keyClass = kNA; }
	else if (key == 8060928) { kINFO.keyMsg = "[F12]"; kINFO.keyClass = kNA; }
	else if (key == 2424832) { kINFO.keyMsg = "[<]"; kINFO.keyClass = kNA; }
	else if (key == 2555904) { kINFO.keyMsg = "[>]"; kINFO.keyClass = kNA; }
	else if (key == 2490368) { kINFO.keyMsg = "[^]"; kINFO.keyClass = kNA; }
	else if (key == 2621440) { kINFO.keyMsg = "[v]"; kINFO.keyClass = kNA; }
	else { kINFO.keyMsg = ""; kINFO.keyClass = kNA; }
	
	return kINFO;
}


//========================================================================
// Parse { construction / destruct } of threads
//========================================================================
threadINFO inline cvGENERIC::WarpThreadPool(string& threadName)
{
	//---------------------------
	// Temporal Method
	// [Fn] -> [n] -> n
	//---------------------------
	threadName.erase(1, 1);
	char numPart = threadName[1];
	UINT num = atoi(&numPart);
	UINT nth = num - 1;

	threadINFO tINFO;

	//----------------------------------------
	// threads isn't empty
	//----------------------------------------
	if (this->threadsNUM > 0)
	{		
		if (this->threads[nth] != "")
		{	// delete the existing

			this->threads[nth] = "";
			this->threadsNUM--;
			tINFO.Destruct(nth, threadName);
		}
		else
		{	// insert the new

			this->threads[nth] = threadName;
			this->threadsNUM++;
			tINFO.Create(nth, threadName);
		}

	}

	//----------------------------------------
	// threads is empty
	//----------------------------------------
	else
	{	// insert the new

		this->threads[nth] = threadName;
		this->threadsNUM++;
		tINFO.Create(nth, threadName);
	}

	return tINFO;
}


//========================================================================
// Parse { control } of threads
//========================================================================
threadINFO inline cvGENERIC::ControlThread(string& threadNum)
{
	//-------------------------
	// Temporal Method
	// [Fn] -> [n] -> n
	//-------------------------
	char numPart = threadNum[1];
	UINT num = atoi(&numPart);
	UINT nth = num - 1;

	threadINFO tINFO;

	if (nth < 0)
	{	// pressed [0]

		tINFO.Clear();
		return tINFO;
	}

	//-------------------------------------------------------
	// threads isn't empty
	//-------------------------------------------------------
	if (this->threadsNUM > 0)
	{
		if(this->threads[nth] != "")
		{	// control the match

			tINFO.Control(nth, threadNum);
		}

		else
		{	// nothing matched

			tINFO.Clear();
		}
	}

	//-------------------------------------------------------
	// threads is empty
	//-------------------------------------------------------
	else
	{
		tINFO.Clear();
	}

	return tINFO;
}