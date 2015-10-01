// Robinus Fuser. 2015-09-25 06:15
// Electronics Dept. Konkuk Univ. South Korea
// fordicus@naver.com


#pragma once

#include <WtsApi32.h>					// for QueryWinUserName()
#pragma comment(lib, "Wtsapi32.lib")	// for QueryWinUserName()

#include <vector>
#include <atlstr.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <Windows.h>

using namespace std;


//----------------------------------------------------------------------------
// #define DEBUG_PROGRAM
#define PROFILE_PROGRAM

#define keyDelay			1
#define drawTimer			100
#define maxThreads			8
#define maxDetectors		2

#define consoleSweep		"\r\n \r\n \r\n \r\n "
#define notify_ONE			"Allow single Process."
#define notify_END			"the End."

//----------------------------------------------------------------------------
#ifndef TRUE
#define TRUE		1
#define FALSE		0
#endif

#ifndef VOID
#define VOID		void
#endif

typedef unsigned long long	ULONGLONG;
typedef int					INT;
typedef unsigned int		UINT;
typedef unsigned char		UCHAR;
//typedef bool				BOOL;