
// cmtDlgDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CcmtDlgDlg 대화 상자
class CcmtDlgDlg : public CDialogEx
{
// 생성입니다.
public:
	CcmtDlgDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CMTDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
		
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(register UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, register UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedOk();
	

	//-----------------------------------------------------------------------------------
	// Variables for MFC
	//-----------------------------------------------------------------------------------
	HICON m_hIcon;
	CFont m_font, m_fontConsoleMain, m_fontConsoleDist;
	
	CString m_console, m_consoleDist;
	
	INT m_distCoeffCTRL_posLegacy, m_distThreshCTRL_posLegacy;
	CSliderCtrl m_distCoeffCTRL, m_distThreshCTRL;
	DOUBLE n_distCoeffCTRL, n_distThreshCTRL;
	#define distCOEFF	0.26
	#define distTHRESH	0.001
	#define fillCTRL	"        Coefficient : %.2f             Threshold : %.3f"
	
	HBITMAP icoBitmapList;
	HBITMAP icoBitmapX;
	HBITMAP icoBitmap[maxThreads * 2][2];
	CStatic mICO_LIST, mICO_0, mICO_1, mICO_2, mICO_3, mICO_4, mICO_5, mICO_6, mICO_7;


	//-----------------------------------------------------------------------------------
	// Functions for MFC
	//-----------------------------------------------------------------------------------
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnBnClickedOk5();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk6();
};
