// Robinus Fuser. 2015-09-25 06:15
// Electronics Dept. Konkuk Univ. South Korea
// fordicus@naver.com

/*
	RandNum()
	gcTimeScale()

	class gcPERFORMANCE_LOGGER
	class gcPERFORMANCE_REPORTER
*/


#pragma once
#include "gcPreprocess.hpp"


//=================================================================================
// Random Number Generator
//=================================================================================
UINT inline RandNum(VOID)
{
	LARGE_INTEGER SEED;
	::QueryPerformanceCounter(&SEED);
	srand(static_cast<UINT>(SEED.QuadPart));

	return rand();
}


//=================================================================================
// gcTimeScale : [min], [s], [ms], [us]
//=================================================================================
string inline gcTimeScale(const DOUBLE& elapse)
{
	DOUBLE scaled;
	CString avgT;

	if (elapse > 1000000.0)
	{
		scaled = (elapse / 1000000.0);

		if (scaled > 60)
		{
			scaled /= 60.0;
			avgT.Format(_T("%.3f[min]"), scaled);
		}
		else
		{
			avgT.Format(_T("%.3f[s]"), scaled);
		}

	}
	else if (elapse > 1000.0)
	{
		scaled = (elapse / 1000.0);
		avgT.Format(_T("%.3f[ms]"), scaled);
	}
	else
	{
		scaled = elapse;
		avgT.Format(_T("%.3f[us]"), scaled);
	}

	CT2CA ANSI_avgT(avgT);
	return ANSI_avgT.operator LPSTR();
}


//=================================================================================
// gcPERFORMANCE_LOGGER		: Measure, and Accumulate
//=================================================================================
class gcPERFORMANCE_LOGGER
{
private:
	LARGE_INTEGER lnFreqStart, lnFreqStop;
	LARGE_INTEGER lnStart, lnStop;
	UINT itrCnt, itrLimit;
	string blockName;

public:
	vector<DOUBLE> elapse;

#define itrDefault 10000
	gcPERFORMANCE_LOGGER();
	gcPERFORMANCE_LOGGER(const string& blockName);
	gcPERFORMANCE_LOGGER(const UINT& itrLimit);
	gcPERFORMANCE_LOGGER(const string& blockName, const UINT& itrLimit);
	string getName(VOID);
	UINT getItrCnt(VOID);

	VOID Start(VOID);
	DOUBLE Stop(const BOOL& Accumulate);
};


inline gcPERFORMANCE_LOGGER::gcPERFORMANCE_LOGGER()
{
	this->itrCnt = 0;
	this->itrLimit = itrDefault;
	this->blockName = "";
}
inline gcPERFORMANCE_LOGGER::gcPERFORMANCE_LOGGER(const string& blockName)
{
	this->itrCnt = 0;
	this->itrLimit = itrDefault;
	this->blockName = blockName;
}
inline gcPERFORMANCE_LOGGER::gcPERFORMANCE_LOGGER(const UINT& itrLimit)
{
	this->itrCnt = 0;
	this->itrLimit = itrLimit;
	this->blockName = "";
}
inline gcPERFORMANCE_LOGGER::gcPERFORMANCE_LOGGER(const string& blockName, const UINT& itrLimit)
{
	this->itrCnt = 0;
	this->itrLimit = itrLimit;
	this->blockName = blockName;
}


string inline gcPERFORMANCE_LOGGER::getName(VOID)
{
	return this->blockName;
}


UINT inline gcPERFORMANCE_LOGGER::getItrCnt(VOID)
{
	return this->itrCnt;
}


VOID inline gcPERFORMANCE_LOGGER::Start(VOID)
{
	if (this->itrCnt <= this->itrLimit)
	{
		::QueryPerformanceFrequency(&this->lnFreqStart);
		::QueryPerformanceCounter(&this->lnStart);
	}
}


DOUBLE inline gcPERFORMANCE_LOGGER::Stop(const BOOL& Accumulate)
{
	DOUBLE time = 0.0;

	++this->itrCnt;
	if (this->itrCnt <= this->itrLimit)
	{
		::QueryPerformanceCounter(&this->lnStop);
		::QueryPerformanceFrequency(&this->lnFreqStop);

		time = // static_cast<DOUBLE>
			(
			((this->lnStop.QuadPart - this->lnStart.QuadPart) * 1000000.0) /
			((this->lnFreqStart.QuadPart + this->lnFreqStop.QuadPart) / 2.0)
			);

		if (Accumulate) this->elapse.push_back(time);
	}

	return time;
}


//=================================================================================
// gcPERFORMANCE_REPORTER	: Find average, and Report into File
//=================================================================================
class gcPERFORMANCE_REPORTER
{
public:
	vector<gcPERFORMANCE_LOGGER> profiles;
	VOID Report(const string& notePath);
};


VOID inline gcPERFORMANCE_REPORTER::Report(const string& notePath)
{	
	register UINT numProfies = this->profiles.size();

	BOOL isEmpty = FALSE;
	for (register UINT pr = 0; pr < numProfies; ++pr)
	{
		if (this->profiles[pr].elapse.size() == 0) isEmpty = TRUE;
	}

	if (isEmpty) return;

	for (register UINT pr = 0; pr < numProfies; ++pr)
		sort(this->profiles[pr].elapse.begin(), this->profiles[pr].elapse.end());


	ofstream noteFile;
	string PATH = notePath;
	PATH += "_prReport.txt";
	noteFile.open(PATH);
	noteFile << "=====================================\n";

	for (register UINT pr = 0; pr < numProfies; ++pr)
	{
		register UINT itrCNT = this->profiles[pr].elapse.size();
		if (itrCNT == 0) continue;

		UINT pick = itrCNT / 2;
		DOUBLE mid = this->profiles[pr].elapse[pick];
		UINT fps = static_cast<UINT>(1000000.0 * (1.0 / mid));

		// MPT = Median Processing Time
		noteFile << this->profiles[pr].getName() << " called " << to_string(itrCNT)
			<< "[times]\n" << "MPT " << gcTimeScale(mid) << " ¢¡ "
			<< fps << "[FPS]\n" << "Elapse Range : "
			<< gcTimeScale(this->profiles[pr].elapse[0])
			<< " ~ " << gcTimeScale(this->profiles[pr].elapse[itrCNT - 1]) << "\n";

		if (numProfies != 1)
		{
			noteFile << "-----------------------------------";
			noteFile << "-----------------------------------\n";
		}
	}

	noteFile << "\n\n=====================================\n";
	noteFile << "Verbose in [us], with increasing Order.\n";

	for (register UINT pr = 0; pr < numProfies; ++pr)
	{
		noteFile << "-----------------------------------";
		noteFile << "-----------------------------------\n";
		noteFile << this->profiles[pr].getName() << " elapses, \n";

		for (register UINT tick = 0; tick < profiles[pr].elapse.size(); ++tick)
			noteFile << to_string(profiles[pr].elapse[tick]) << "\n";

	}

	noteFile.close();
}