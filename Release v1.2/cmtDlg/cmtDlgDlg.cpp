
// cmtDlgDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "cmtDlg.h"
#include "cmtDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcmtDlgDlg 대화 상자



CcmtDlgDlg::CcmtDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcmtDlgDlg::IDD, pParent)
	, m_console(_T(""))
	, m_consoleDist(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcmtDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_console);
	DDX_Control(pDX, ICO_0, mICO_0);
	DDX_Control(pDX, ICO_1, mICO_1);
	DDX_Control(pDX, ICO_2, mICO_2);
	DDX_Control(pDX, ICO_3, mICO_3);
	DDX_Control(pDX, ICO_4, mICO_4);
	DDX_Control(pDX, ICO_5, mICO_5);
	DDX_Control(pDX, ICO_6, mICO_6);
	DDX_Control(pDX, ICO_7, mICO_7);
	DDX_Control(pDX, ICO_LIST, mICO_LIST);
	DDX_Text(pDX, IDC_EDIT2, m_consoleDist);
	DDX_Control(pDX, IDC_SLIDER1, m_distCoeffCTRL);
	DDX_Control(pDX, IDC_SLIDER2, m_distThreshCTRL);
}

BEGIN_MESSAGE_MAP(CcmtDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDOK, &CcmtDlgDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CcmtDlgDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &CcmtDlgDlg::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK4, &CcmtDlgDlg::OnBnClickedOk4)
	ON_BN_CLICKED(IDOK5, &CcmtDlgDlg::OnBnClickedOk5)
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDOK6, &CcmtDlgDlg::OnBnClickedOk6)
END_MESSAGE_MAP()


//=======================================================================================
// CcmtDlgDlg 메시지 처리기
//=======================================================================================
void CcmtDlgDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_distCoeffCTRL)
	{
		INT pos = m_distCoeffCTRL.GetPos();
		m_distCoeffCTRL_posLegacy = pos;

		this->n_distCoeffCTRL = distCOEFF * (100 - pos);
		g_distCoeff = this->n_distCoeffCTRL;
	}

	else if (pScrollBar == (CScrollBar*)&m_distThreshCTRL)
	{
		INT pos = m_distThreshCTRL.GetPos();
		m_distThreshCTRL_posLegacy = pos;

		this->n_distThreshCTRL = distTHRESH * (100 - pos);
		g_distThresh = this->n_distThreshCTRL;
	}

	m_consoleDist.Format(
		_T(fillCTRL),
		n_distCoeffCTRL,
		n_distThreshCTRL
		);

	UpdateData(FALSE);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CcmtDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	//===================================================================================
	// Initializations for MFC GUI
	//===================================================================================
	m_distCoeffCTRL.SetRangeMin(0);
	m_distCoeffCTRL.SetRangeMax(99);
	m_distCoeffCTRL.SetPos(99);
	m_distCoeffCTRL_posLegacy = 99;
	
	m_distThreshCTRL.SetRangeMin(0);
	m_distThreshCTRL.SetRangeMax(99);
	m_distThreshCTRL.SetPos(99);
	m_distThreshCTRL_posLegacy = 99;


	this->n_distCoeffCTRL = distCOEFF * (100.0 - m_distCoeffCTRL.GetPos());
	g_distCoeff = this->n_distCoeffCTRL;

	this->n_distThreshCTRL = distTHRESH * (100.0 - m_distThreshCTRL.GetPos());
	g_distThresh = this->n_distThreshCTRL;

	m_consoleDist.Format(_T(fillCTRL),
		n_distCoeffCTRL, n_distThreshCTRL);
	
	CString notePATH;
	notePATH.Format(_T("C:\\Users\\%s\\Desktop\\cmtResult\\"), QueryWinUserName());
	::CreateDirectory(notePATH, 0);

	CT2CA ANSI_notePATH(notePATH);
	g_notePath = ANSI_notePATH.operator LPSTR();

	m_console.Format(_T("\r\n             Consensus Matching Tracking  \r\n"));
	m_console +=	 _T("             by Robinus Fuser.   2015/09/15  \r\n \r\n");

	UpdateData(FALSE);


	//-----------------------------------------------------------------------------------
	m_fontConsoleMain.CreatePointFont(140, L"Calibri");
	GetDlgItem(IDC_EDIT1)->SetFont(&m_fontConsoleMain);

	m_fontConsoleDist.CreatePointFont(152, L"Calibri");
	GetDlgItem(IDC_EDIT2)->SetFont(&m_fontConsoleDist);
	
	m_font.CreatePointFont(180, L"Berlin Sans FB");
	GetDlgItem(IDOK)->SetFont(&m_font);
	GetDlgItem(IDOK2)->SetFont(&m_font);
	GetDlgItem(IDOK3)->SetFont(&m_font);
	GetDlgItem(IDOK4)->SetFont(&m_font);
	GetDlgItem(IDOK5)->SetFont(&m_font);
	GetDlgItem(IDOK6)->SetFont(&m_font);

	
	//-----------------------------------------------------------------------------------
	GlobalInits();

	HINSTANCE handle = AfxGetInstanceHandle();

	this->icoBitmapList = ::LoadBitmap(handle, MAKEINTRESOURCE(rFn_LIST));
	mICO_LIST.SetBitmap(icoBitmapList);
	
	this->icoBitmapX = ::LoadBitmap(handle, MAKEINTRESOURCE(rFn_x));	
	
	this->icoBitmap[0][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF1_0));
	this->icoBitmap[0][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF1_1));
	this->icoBitmap[1][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF2_0));
	this->icoBitmap[1][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF2_1));
	this->icoBitmap[2][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF3_0));
	this->icoBitmap[2][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF3_1));
	this->icoBitmap[3][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF4_0));
	this->icoBitmap[3][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF4_1));
	this->icoBitmap[4][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF5_0));
	this->icoBitmap[4][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF5_1));
	this->icoBitmap[5][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF6_0));
	this->icoBitmap[5][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF6_1));
	this->icoBitmap[6][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF7_0));
	this->icoBitmap[6][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF7_1));
	this->icoBitmap[7][0] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF8_0));
	this->icoBitmap[7][1] = ::LoadBitmap(handle, MAKEINTRESOURCE(rF8_1));
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

HBRUSH CcmtDlgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, register UINT nCtlColor)
{	// Dialog Background, Static Control

	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{ return (HBRUSH)GetStockObject(DKGRAY_BRUSH); }

	else if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
	{
		pDC->SetTextColor(winRGB_PaleGreen);
		pDC->SetBkColor(winRGB_DKGray);
		return (HBRUSH)GetStockObject(winRGB_DKGray);
	}

	else if (pWnd->GetDlgCtrlID() == IDC_EDIT2)
	{
		pDC->SetTextColor(winRGB_Light);
		pDC->SetBkColor(winRGB_DKGray);
		return (HBRUSH)GetStockObject(winRGB_DKGray);
	}


	return hbr;
}


void CcmtDlgDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{	// Button Control

	BOOL isTargetBtn = FALSE;
	if (nIDCtl == IDOK || nIDCtl == IDOK2 || nIDCtl == IDOK3 ||
		nIDCtl == IDOK4 || nIDCtl == IDOK5 || nIDCtl == IDOK6 )
	{
		isTargetBtn = TRUE;
	}

	if (isTargetBtn == TRUE)
	{
		CDC dc; RECT rect;
		dc.Attach(lpDrawItemStruct->hDC);
		rect = lpDrawItemStruct->rcItem;
		dc.Draw3dRect(&rect, RGB(0, 0, 0), RGB(0, 0, 0));
		dc.FillSolidRect(&rect, winRGB_DKGray);
	
		//-------------------------------------------------------------------------------
		//Show the Effect of Click Event
		//-------------------------------------------------------------------------------
		register UINT state = lpDrawItemStruct->itemState;
		if ((state & ODS_SELECTED)) dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		else dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);

		//-------------------------------------------------------------------------------
		//Draw Color Text
		//-------------------------------------------------------------------------------
		dc.SetBkMode(TRANSPARENT);
		if (nIDCtl == IDOK || nIDCtl == IDOK5){
			dc.SetTextColor(winRGB_Aquamarine);
		}
		else if (nIDCtl == IDOK6){
			dc.SetTextColor(winRGB_Orange);
		}
		else {
			dc.SetTextColor(winRGB_LightCoral);
		}

		TCHAR buffer[MAX_PATH];
		ZeroMemory(buffer, MAX_PATH);
		::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);

		dc.DrawText(buffer, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		dc.Detach();

	}

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
//=======================================================================================

void CcmtDlgDlg::OnSysCommand(register UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CcmtDlgDlg::OnPaint()
{
	{
		#define Fn_VOID					(icoBitmapX)
		#define F1_IDLE					(icoBitmap[0][0])
		#define F1_ACTIVE				(icoBitmap[0][1])
		#define F2_IDLE					(icoBitmap[1][0])
		#define F2_ACTIVE				(icoBitmap[1][1])
		#define F3_IDLE					(icoBitmap[2][0])
		#define F3_ACTIVE				(icoBitmap[2][1])
		#define F4_IDLE					(icoBitmap[3][0])
		#define F4_ACTIVE				(icoBitmap[3][1])
		#define F5_IDLE					(icoBitmap[4][0])
		#define F5_ACTIVE				(icoBitmap[4][1])
		#define F6_IDLE					(icoBitmap[5][0])
		#define F6_ACTIVE				(icoBitmap[5][1])
		#define F7_IDLE					(icoBitmap[6][0])
		#define F7_ACTIVE				(icoBitmap[6][1])
		#define F8_IDLE					(icoBitmap[7][0])
		#define F8_ACTIVE				(icoBitmap[7][1])

		if (threadStat[0] == thread_VOID) mICO_0.SetBitmap(Fn_VOID);
		else if (threadStat[0] == thread_IDLE) mICO_0.SetBitmap(F1_IDLE);
		else if (threadStat[0] == thread_ACTIVE) mICO_0.SetBitmap(F1_ACTIVE);

		if (threadStat[1] == thread_VOID) mICO_1.SetBitmap(Fn_VOID);
		else if (threadStat[1] == thread_IDLE) mICO_1.SetBitmap(F2_IDLE);
		else if (threadStat[1] == thread_ACTIVE) mICO_1.SetBitmap(F2_ACTIVE);

		if (threadStat[2] == thread_VOID) mICO_2.SetBitmap(Fn_VOID);
		else if (threadStat[2] == thread_IDLE) mICO_2.SetBitmap(F3_IDLE);
		else if (threadStat[2] == thread_ACTIVE) mICO_2.SetBitmap(F3_ACTIVE);
		
		if (threadStat[3] == thread_VOID) mICO_3.SetBitmap(Fn_VOID);
		else if (threadStat[3] == thread_IDLE) mICO_3.SetBitmap(F4_IDLE);
		else if (threadStat[3] == thread_ACTIVE) mICO_3.SetBitmap(F4_ACTIVE);

		if (threadStat[4] == thread_VOID) mICO_4.SetBitmap(Fn_VOID);
		else if (threadStat[4] == thread_IDLE) mICO_4.SetBitmap(F5_IDLE);
		else if (threadStat[4] == thread_ACTIVE) mICO_4.SetBitmap(F5_ACTIVE);

		if (threadStat[5] == thread_VOID) mICO_5.SetBitmap(Fn_VOID);
		else if (threadStat[5] == thread_IDLE) mICO_5.SetBitmap(F6_IDLE);
		else if (threadStat[5] == thread_ACTIVE) mICO_5.SetBitmap(F6_ACTIVE);

		if (threadStat[6] == thread_VOID) mICO_6.SetBitmap(Fn_VOID);
		else if (threadStat[6] == thread_IDLE) mICO_6.SetBitmap(F7_IDLE);
		else if (threadStat[6] == thread_ACTIVE) mICO_6.SetBitmap(F7_ACTIVE);

		if (threadStat[7] == thread_VOID) mICO_7.SetBitmap(Fn_VOID);
		else if (threadStat[7] == thread_IDLE) mICO_7.SetBitmap(F8_IDLE);
		else if (threadStat[7] == thread_ACTIVE) mICO_7.SetBitmap(F8_ACTIVE);
		
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CcmtDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//===============================================================================
// Button Controls
//===============================================================================
void CcmtDlgDlg::OnBnClickedOk2()
{	// toggle Scale Estimation

	g_isScale = !g_isScale;

	CString SCALE;

	if (g_isScale)			SCALE.Format(_T(" Scale        [o]"));
	else if (!g_isScale)	SCALE.Format(_T(" Scale        [x]"));
	
	GetDlgItem(IDOK2)->SetWindowText(SCALE);

}
void CcmtDlgDlg::OnBnClickedOk3()
{	// toggle Rotation Estimation
	
	g_isRotation = !g_isRotation;

	CString ROTATION;

	if (g_isRotation)			ROTATION.Format(_T(" Rotation   [o]"));
	else if (!g_isRotation)		ROTATION.Format(_T(" Rotation   [x]"));

	GetDlgItem(IDOK3)->SetWindowText(ROTATION);

}
void CcmtDlgDlg::OnBnClickedOk6()
{	// Detector selection

	CString BTN = _T("with ");

	if (g_selDetector == (maxDetectors - 1)) g_selDetector = 0;
	else g_selDetector = ++g_selDetector;

	if (g_selDetector == detector_GFTT) BTN += _T("GFTT");
	else if (g_selDetector == detector_FAST) BTN += _T("FAST");		

	GetDlgItem(IDOK6)->SetWindowText(BTN);

}
void CcmtDlgDlg::OnBnClickedOk4()
{	//	Quit

	GlobalExits();

	this->m_console.Format(_T(consoleSweep));
	this->UpdateData(FALSE);
	this->OnPaint();
	CcmtDlgDlg::OnOK();
}


//===============================================================================
// a CMT Thread
//===============================================================================
UINT static threadCMT(LPVOID pParam)
{
#ifdef PROFILE_PROGRAM
	string myNotePath = g_notePath;
	gcPERFORMANCE_REPORTER pREPORTER;
	gcPERFORMANCE_LOGGER pr_01("processFrame");
#endif

	UINT myID = g_IdxThread;
	threadStat[myID] = thread_IDLE;
	Ptr<FeatureDetector> myDetector = g_detector;

	CMT cmt(myID, myDetector, distCOEFF, distTHRESH, g_notePath);

	BOOL loop = TRUE;
	while (loop)
	{
		HANDLE eventLIST[3];
		eventLIST[0] = (HANDLE)warpEvents[myID];
		eventLIST[1] = (HANDLE)controlEvents[myID];
		eventLIST[2] = (HANDLE)allowProcess[myID];

		DWORD EVENT = ::WaitForMultipleObjects(3, eventLIST, FALSE, 2 * keyDelay);
		if (EVENT == WAIT_OBJECT_0)
		{	// Kill 

			cmt.noteFile.close();
			warpEvents[myID].ResetEvent();
			loop = FALSE;
			break;
		}
		else if (EVENT == WAIT_OBJECT_0 + 1)
		{	// Initialization

			threadStat[myID] = thread_ACTIVE;
			controlEvents[myID].ResetEvent();

			vector<KeyPoint> myKeypoint = g_keypoint;
			cmt.supplyKeyPoints(myKeypoint);

			Mat frmGray;
			g_frmGray.copyTo(frmGray);

			cmt.initialize(frmGray, g_Rect);
			cmt.openNoteFile();

#ifdef PROFILE_PROGRAM		
			myNotePath += to_string(RandNum());
#endif

		}
		else if (EVENT == WAIT_OBJECT_0 + 2)
		{	// Process a frame
			

#ifdef PROFILE_PROGRAM			
			pr_01.Start();
#endif
			allowProcess[myID].ResetEvent();

			cmt.consensus.configDeform(g_isScale, g_isRotation);
			cmt.supplyKeyPoints(g_keypoints[myID]);
			cmt.configDist(g_distCoeff, g_distThresh);

			Mat frmGray;
			g_frmGrays[myID].copyTo(frmGray);

			BOOL isVisible = cmt.processFrame(frmGray);
			if (!isVisible)
			{
				cmt.recoverArchive();
				continue;
			}

			g_pointsActive[myID].clear();
			g_pointsActive[myID].assign(cmt.pointsActive.begin(), cmt.pointsActive.end());
			cmt.bbRotated.points(g_verticles[myID]);
			// posChange[myID] = cmt.posChange;	// posChange at frame
			posChange[myID] = cmt.posGrowth;	// posgrowth by Frame 

			readyToDraw[myID].SetEvent();

#ifdef PROFILE_PROGRAM
			pr_01.Stop(TRUE);
#endif
		}		
	}
	
#ifdef PROFILE_PROGRAM
	pREPORTER.profiles.push_back(pr_01);
	pREPORTER.Report(myNotePath);
#endif
	
	threadStat[myID] = thread_VOID;
	pThreads[myID] = NULL;

	return 0;
}


//===================================================================================
// Cam Tracker
//===================================================================================
void CcmtDlgDlg::OnBnClickedOk()
{
	if (g_runningONE == TRUE)
	{
		AfxMessageBox(_T(notify_ONE));
		return;
	}


	//-------------------------------------------------------------------------------
	// Generic Initializations
	//-------------------------------------------------------------------------------
	g_runningONE = TRUE;

	string vidPath = g_notePath;

	stringstream ssCONSOLE;
	ssCONSOLE << "\r\n [path] " << vidPath;
	string STR_Results = ssCONSOLE.str();
	CA2CT LONG_vidPath(STR_Results.c_str());
	m_console = LONG_vidPath; m_console += _T("\r\n \r\n \r\n");
	UpdateData(FALSE);

	GlobalInits();
	this->OnPaint();

	cvGENERIC g;
	g_wnd = g.CreateWND();


	//-------------------------------------------------------------------------------
	// Main loop
	//-------------------------------------------------------------------------------
	g_cap.open(0);

	while (g.status == eRUN)
	{
		//---------------------------------------------------------------------------
		// Prevent "Traditional MFC Blinks on Redraw"
		// Redraw only on changes of threads
		//---------------------------------------------------------------------------
		g_isRedraw = QueryThreadChange(threadStat, threadStatOld);
		if (g_isRedraw) this->OnPaint();
		ReserveThreadStat(threadStat, threadStatOld);


		//---------------------------------------------------------------------------
		// Achieve the next Frame and Keypoints
		//---------------------------------------------------------------------------
		g_cap >> g_frm;
		if (g_frm.empty())
		{
			AfxMessageBox(_T(notify_END));
			break;
		}

		g_frm.copyTo(g_frmToShow);
		cvtColor(g_frm, g_frmGray, CV_BGR2GRAY);

		if (g_selDetector == detector_GFTT) g_detector = g.Config_GFTT();
		else if (g_selDetector == detector_FAST) g_detector = g.Config_FAST();
		g_detector->detect(g_frmGray, g_keypoint);


		//---------------------------------------------------------------------------
		// Determine Valid-active-threads which are ROI initialized
		// Reserve grayscale frames for those specific threads
		//---------------------------------------------------------------------------
		vector<UINT> VALID = QueryActiveThread(threadStat);

		for (register UINT id = 0; id < VALID.size(); ++id)
		{
			UINT VID = VALID[id];

			g_frmGray.copyTo(g_frmGrays[VID]);

			g_keypoints[VID].clear();
			g_keypoints[VID].assign(g_keypoint.begin(), g_keypoint.end());

			allowProcess[VID].SetEvent();
		}


		//---------------------------------------------------------------------------
		// Keyboard events handler
		//---------------------------------------------------------------------------
		keyINFO kINFO = g.ReadKey(keyDelay);

		if (kINFO.keyMsg == "[Q]")
		{	// Quit

			g.status = eEXIT;
			break;
		}

		else if (kINFO.keyMsg == "[SPACE]")
		{
			g_testDetector = !g_testDetector;
		}

		else if (kINFO.keyMsg == "[C]")
		{	// Capture

			if (g_captureStat == capture_RUNNING)
			{
				g_writer.release();
				vidPath = g_notePath;
				g_captureStat = capture_IDLE;
			}
			else if (g_captureStat == capture_IDLE)
			{
				//----------------------------------------------------------------------------
				// "C:\\Users\\VLSI-CUDA\\Desktop\\cmtResult\\LocalTimeMS().avi"
				//----------------------------------------------------------------------------
				vidPath += LocalTimeMS(); vidPath += ".avi";

				g_writer.open(vidPath, CV_FOURCC('M', 'P', 'E', 'G'), 30, Size(640, 480), TRUE);
				g_captureStat = capture_RUNNING;
			}
		}

		else if (kINFO.keyClass == kFUNC)
		{	// [F1] ~ [F8]

			threadINFO tINFO = g.WarpThreadPool(kINFO.keyMsg);

			if (tINFO.job == tCREATE)
			{
				g_IdxThread = tINFO.who;
				pThreads[tINFO.who] =
					AfxBeginThread(threadCMT, NULL, THREAD_PRIORITY_TIME_CRITICAL);
			}

			else if (tINFO.job == tDESTRUCT)
			{
				g_IdxThread = tINFO.who;
				warpEvents[g_IdxThread].SetEvent();
			}
		}

		else if (kINFO.keyClass == kNUM)
		{
			threadINFO tINFO = g.ControlThread(kINFO.keyMsg);

			if (tINFO.job == tCONTROL)
			{
				g_IdxThread = tINFO.who;
				g_Rect = g.GetRect(g_wnd, g_frm, g_IdxThread);
				controlEvents[g_IdxThread].SetEvent();
			}
		}


		//---------------------------------------------------------------------------
		// Drawings
		//---------------------------------------------------------------------------
		stringstream ssSURTITLE;

		if (g_captureStat == capture_RUNNING)	ssSURTITLE << "[o] rec  ";
		else if (g_captureStat == capture_IDLE)	ssSURTITLE << "[x] rec  ";

		if (VALID.size() == 0)
		{
			if (g_testDetector)
			{
				for (register UINT it = 0; it < g_keypoint.size(); ++it)
					circle(g_frmToShow, g_keypoint[it].pt, 1, cvBGR_Pink, 2);
			}

			g.PutSurTitle(g_frmToShow, ssSURTITLE.str());
			if (g_captureStat == capture_RUNNING) g_writer << g_frmToShow;

			imshow(g.wnd[0], g_frmToShow);
		}
		else
		{
			BOOL featuresDrawn = FALSE;

			for (register UINT id = 0; id < VALID.size(); ++id)
			{
				UINT VID = VALID[id];
				Scalar BRUSH = g.PickBrush(VID);

				DWORD sigDRAW = WaitForSingleObject((HANDLE)readyToDraw[VID], drawTimer);

				if (sigDRAW == WAIT_OBJECT_0)
				{
					readyToDraw[VID].ResetEvent();
					retryCnt[VID] = 0;

					ssSURTITLE << "[" << to_string(VID+1) << "] ";
					ssSURTITLE << to_string(posChange[VID]) << " ";
					featuresDrawn = TRUE;

					for (register UINT it = 0; it < g_pointsActive[VID].size(); ++it)
					{
						circle(g_frmToShow, g_pointsActive[VID][it], 1, BRUSH, 2);
					}

					for (register UINT it = 0; it < 4; ++it)
					{
						line(g_frmToShow, g_verticles[VID][it], g_verticles[VID][(it + 1) % 4], BRUSH, 2);
					}
				}

				else if (sigDRAW == WAIT_TIMEOUT && threadStat[VID] == thread_ACTIVE)
				{	// Failed thread handling : Kill and Regenerate

					if (retryCnt[VID] > maximum_RETRY)
					{
						retryCnt[VID] = 0;

						g_IdxThread = VID;
						warpEvents[g_IdxThread].SetEvent();

						while (TRUE)
						{
							if (threadStat[VID] == thread_VOID && pThreads[VID] == NULL)
							{
								break;
							}
						}

						pThreads[VID] =
							AfxBeginThread(threadCMT, NULL, THREAD_PRIORITY_TIME_CRITICAL);
					}
					else
					{
						++retryCnt[VID];
					}

					continue;
				}
			}

			if (featuresDrawn) g.PutSurTitle(g_frmToShow, ssSURTITLE.str());
			if (g_captureStat == capture_RUNNING) g_writer << g_frmToShow;

			imshow(g.wnd[0], g_frmToShow);
		}

	}	// Main loop

#ifndef DEBUG_PROGRAM
	{
		GlobalExits();

		this->m_console.Format(_T(consoleSweep));
		this->UpdateData(FALSE);
		this->OnPaint();
	}
#endif

#ifdef DEBUG_PROGRAM
	{
		stringstream ss;
		ss << "Threads' Status : ";
		for (register UINT it = 0; it < maxThreads; ++it)
		{
			ss << threadStat[it];

			if (it == 3) ss << ", ";
			else ss << " ";
		}
		ss << consoleSweep;

		string ANSI_threads = ss.str();
		CA2CT LONG_threads(ANSI_threads.c_str());
		m_console = LONG_threads;
		UpdateData(FALSE);
	}
#endif
}


//===================================================================================
// Vid Tracker
//===================================================================================
void CcmtDlgDlg::OnBnClickedOk5()
{
	if (g_runningONE == TRUE)
	{
		AfxMessageBox(_T(notify_ONE));
		return;
	}


	//-------------------------------------------------------------------------------
	// Generic Initializations
	//-------------------------------------------------------------------------------
	g_runningONE = TRUE;

	string vidPath = g_notePath;

	stringstream ssCONSOLE;
	ssCONSOLE << "\r\n [path] " << g_notePath;
	string STR_Results = ssCONSOLE.str();
	CA2CT LONG_vidPath(STR_Results.c_str());
	m_console = LONG_vidPath; m_console += _T("\r\n \r\n \r\n");
	UpdateData(FALSE);


	GlobalInits();
	this->OnPaint();

	cvGENERIC g;
	g_wnd = g.CreateWND();


	//-------------------------------------------------------------------------------
	// Main loop
	//-------------------------------------------------------------------------------
	g_cap.open(g.LoadVid(FALSE));

	while (g.status == eRUN)
	{
		//---------------------------------------------------------------------------
		// Prevent "Traditional MFC Blinks on Redraw"
		// Redraw only on changes of threads
		//---------------------------------------------------------------------------
		g_isRedraw = QueryThreadChange(threadStat, threadStatOld);
		if (g_isRedraw) this->OnPaint();
		ReserveThreadStat(threadStat, threadStatOld);


		//---------------------------------------------------------------------------
		// Achieve the next Frame
		//---------------------------------------------------------------------------
		g_cap >> g_frm;
		if (g_frm.empty())
		{
			AfxMessageBox(_T(notify_END));
			break;
		}

		g_frm.copyTo(g_frmToShow);
		cvtColor(g_frm, g_frmGray, CV_BGR2GRAY);

		if (g_selDetector == detector_GFTT) g_detector = g.Config_GFTT();
		else if (g_selDetector == detector_FAST) g_detector = g.Config_FAST();
		g_detector->detect(g_frmGray, g_keypoint);


		//---------------------------------------------------------------------------
		// Determine Valid-active-threads which are ROI initialized
		// Reserve grayscale frames for those specific threads
		//---------------------------------------------------------------------------
		vector<UINT> VALID = QueryActiveThread(threadStat);

		for (register UINT id = 0; id < VALID.size(); ++id)
		{
			UINT VID = VALID[id];

			g_frmGray.copyTo(g_frmGrays[VID]);

			g_keypoints[VID].clear();
			g_keypoints[VID].assign(g_keypoint.begin(), g_keypoint.end());

			allowProcess[VID].SetEvent();
		}


		//---------------------------------------------------------------------------
		// Keyboard events handler
		//---------------------------------------------------------------------------
		keyINFO kINFO = g.ReadKey(keyDelay);

		if (kINFO.keyMsg == "[Q]")
		{	// Quit

			g.status = eEXIT;
			break;
		}

		else if (kINFO.keyMsg == "[SPACE]")
		{
			g_testDetector = !g_testDetector;
		}

		else if (kINFO.keyMsg == "[C]")
		{	// Capture

			if (g_captureStat == capture_RUNNING)
			{
				g_writer.release();
				vidPath = g_notePath;
				g_captureStat = capture_IDLE;
			}
			else if (g_captureStat == capture_IDLE)
			{
				//----------------------------------------------------------------------------
				// "C:\\Users\\VLSI-CUDA\\Desktop\\cmtResult\\LocalTimeMS().avi"
				//----------------------------------------------------------------------------
				vidPath += LocalTimeMS(); vidPath += ".avi";

				g_writer.open(vidPath, CV_FOURCC('M', 'P', 'E', 'G'), 30, g.vidSize, TRUE);
				g_captureStat = capture_RUNNING;
			}
		}

		else if (kINFO.keyClass == kFUNC)
		{	// [F1] ~ [F8]

			threadINFO tINFO = g.WarpThreadPool(kINFO.keyMsg);

			if (tINFO.job == tCREATE)
			{
				g_IdxThread = tINFO.who;
				pThreads[tINFO.who] =
					AfxBeginThread(threadCMT, NULL, THREAD_PRIORITY_TIME_CRITICAL);
			}

			else if (tINFO.job == tDESTRUCT)
			{
				g_IdxThread = tINFO.who;
				warpEvents[g_IdxThread].SetEvent();
			}
		}

		else if (kINFO.keyClass == kNUM)
		{
			threadINFO tINFO = g.ControlThread(kINFO.keyMsg);

			if (tINFO.job == tCONTROL)
			{
				g_IdxThread = tINFO.who;
				g_Rect = g.GetRect(g_wnd, g_frm, g_IdxThread);
				controlEvents[g_IdxThread].SetEvent();
			}
		}


		//---------------------------------------------------------------------------
		// Drawings
		//---------------------------------------------------------------------------
		stringstream ssSURTITLE;

		if (g_captureStat == capture_RUNNING)	ssSURTITLE << "[o] rec  ";
		else if (g_captureStat == capture_IDLE)	ssSURTITLE << "[x] rec  ";

		if (VALID.size() == 0)
		{
			if (g_testDetector)
			{
				for (register UINT it = 0; it < g_keypoint.size(); ++it)
					circle(g_frmToShow, g_keypoint[it].pt, 1, cvBGR_Pink, 2);
			}

			g.PutSurTitle(g_frmToShow, ssSURTITLE.str());
			if (g_captureStat == capture_RUNNING) g_writer << g_frmToShow;

			imshow(g.wnd[0], g_frmToShow);
		}
		else
		{	
			BOOL featuresDrawn = FALSE;

			for (register UINT id = 0; id < VALID.size(); ++id)
			{
				UINT VID = VALID[id];
				Scalar BRUSH = g.PickBrush(VID);

				DWORD sigDRAW = WaitForSingleObject((HANDLE)readyToDraw[VID], drawTimer);

				if (sigDRAW == WAIT_OBJECT_0)
				{
					readyToDraw[VID].ResetEvent();
					retryCnt[VID] = 0;

					ssSURTITLE << "[" << to_string(VID + 1) << "] ";
					ssSURTITLE << to_string(posChange[VID]) << " ";
					featuresDrawn = TRUE;

					for (register UINT it = 0; it < g_pointsActive[VID].size(); ++it)
					{
						circle(g_frmToShow, g_pointsActive[VID][it], 1, BRUSH, 2);
					}

					for (register UINT it = 0; it < 4; ++it)
					{
						line(g_frmToShow, g_verticles[VID][it], g_verticles[VID][(it + 1) % 4], BRUSH, 2);
					}
				}

				else if (sigDRAW == WAIT_TIMEOUT && threadStat[VID] == thread_ACTIVE)
				{	// Failed thread handling : Kill and Regenerate

					if (retryCnt[VID] > maximum_RETRY)
					{
						retryCnt[VID] = 0;

						g_IdxThread = VID;
						warpEvents[g_IdxThread].SetEvent();

						while (TRUE)
						{
							if (threadStat[VID] == thread_VOID && pThreads[VID] == NULL)
							{
								break;
							}
						}

						pThreads[VID] =
							AfxBeginThread(threadCMT, NULL, THREAD_PRIORITY_TIME_CRITICAL);
					}
					else
					{
						++retryCnt[VID];
					}

					continue;
				}
			}
			if (featuresDrawn)	g.PutSurTitle(g_frmToShow, ssSURTITLE.str());
			if (g_captureStat == capture_RUNNING) g_writer << g_frmToShow;

			imshow(g.wnd[0], g_frmToShow);
		}

	}	// Main loop

#ifndef DEBUG_PROGRAM
	{
		GlobalExits();

		this->m_console.Format(_T(consoleSweep));
		this->UpdateData(FALSE);
		this->OnPaint();
	}
#endif

#ifdef DEBUG_PROGRAM
	{
		stringstream ss;
		ss << "Threads' Status : ";
		for (register UINT it = 0; it < maxThreads; ++it)
		{
			ss << threadStat[it];

			if (it == 3) ss << ", ";
			else ss << " ";
		}
		ss << consoleSweep;

		string ANSI_threads = ss.str();
		CA2CT LONG_threads(ANSI_threads.c_str());
		m_console = LONG_threads;
		UpdateData(FALSE);
	}
#endif
}