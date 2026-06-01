
// Clock2026.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "ClockData.h"


// CClock2026App:
// See Clock2026.cpp for the implementation of this class
//

class CClock2026App : public CWinApp
{
public:
	CClock2026App();

private:
	HINSTANCE m_hInstAdvancedEdit;
	CHDdaVinci* m_pHDdaVinci;
	CStatus* m_pStatus;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	CHDdaVinci* GetHDdaVinci(void) { return m_pHDdaVinci; }
	CStatus* GetStatus(void) { return m_pStatus; }
	void LoadSetting(void);
	void SaveSetting(void);

	static BOOL CALLBACK searcher(HWND hWnd, LPARAM lParam);

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CClock2026App theApp;


extern CString GetDataDir(); 
extern CString GetLogsDir();
extern CString GetMirDir();
extern CString GetScrollDir();
extern CString GetNoticeDir();
extern CString GetExportDir();
extern CString GetManualUpDir();
extern CString GetLiveVideoDir();
extern CString GetImagesDir();
extern CString GetWeatherIconsDir();