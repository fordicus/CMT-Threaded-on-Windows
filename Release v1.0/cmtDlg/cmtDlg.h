
// cmtDlg.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CcmtDlgApp:
// �� Ŭ������ ������ ���ؼ��� cmtDlg.cpp�� �����Ͻʽÿ�.
//

class CcmtDlgApp : public CWinApp
{
public:
	CcmtDlgApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CcmtDlgApp theApp;