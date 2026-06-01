// Clock2026.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Clock2026.h"
#include "Clock2026Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClock2026App

BEGIN_MESSAGE_MAP(CClock2026App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CClock2026App construction

CClock2026App::CClock2026App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CClock2026App object

CClock2026App theApp;

BOOL CALLBACK CClock2026App::searcher(HWND hWnd, LPARAM lParam)
{
	DWORD_PTR result;

	LRESULT ok = ::SendMessageTimeout(hWnd,
		WM_ARE_YOU_ME,
		0, 0,
		SMTO_BLOCK | SMTO_ABORTIFHUNG,
		200,
		&result);
	if (ok == 0)
		return TRUE; // ignore this and continue
	if (result == WM_ARE_YOU_ME)
	{ /* found it */
		HWND* target = (HWND*)lParam;
		*target = hWnd;
		return FALSE; // stop search
	} /* found it */
	return TRUE; // continue search
}

// CClock2026App initialization

BOOL CClock2026App::InitInstance()
{

	if (!AfxOleInit())
	{
		AfxMessageBox(_T("AfxOleInit failed"));
		return FALSE;
	}
	/// ////////////////////////////////////////////////////////////////
	//HRESULT	result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);   // ADDED
	//if (FAILED(result))
	//{
	//	::AfxMessageBox(_T("CoInitializeEx Fail !!!"));
	//}
	////////////////////////////////////////////////////////////////////

	BOOL bAlreadyRunning;

	HANDLE hMutexOneInstance = ::CreateMutex(nullptr, FALSE, createExclusionName(_T("{AD2C73BD-D211-406a-9822-B4117F148672}")));

	// what changes for the alternative solutions
	// is the UID in the above call
	// which will be replaced by a call on
	// createExclusionName

	bAlreadyRunning = (::GetLastError() == ERROR_ALREADY_EXISTS ||
		::GetLastError() == ERROR_ACCESS_DENIED);
	// The call fails with ERROR_ACCESS_DENIED if the Mutex was
	// created in a different users session because of passing
	// NULL for the SECURITY_ATTRIBUTES on Mutex creation);

	if (bAlreadyRunning)
	{ /* kill this */
		HWND hOther = nullptr;
		EnumWindows(searcher, (LPARAM)&hOther);

		if (hOther != nullptr)
		{ /* pop up */
			AfxMessageBox(_T("Mir관련 S/W가 실행 중이므로 MPT2025이 실행될 수 없습니다!"), MB_ICONSTOP);
			::SetForegroundWindow(hOther);
			::ShowWindow(hOther, SW_SHOW);

			if (IsIconic(hOther))
			{ /* restore */
				::ShowWindow(hOther, SW_RESTORE);
			} /* restore */
		} /* pop up */

		return FALSE; // terminates the creation
	} /* kill this */

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	if (!AfxInitRichEdit2())
	{
		return FALSE;
	}

	if (m_hInstAdvancedEdit == nullptr)
	{
		m_hInstAdvancedEdit = ::LoadLibrary(_T("RICHED20.DLL"));
	}

#ifdef _UHD_
	m_pHDdaVinci = new CHDdaVinci(3);  // UHD 해상도
#else


	CCGXFrameworkEdit::_useQuicktime = 0;
	m_pHDdaVinci = new CHDdaVinci;



	m_pStatus = new CStatus();

#endif


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager* pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("KBS MPT 2025"));
	
	CClock2026Dlg dlg;

	m_pMainWnd = &dlg;
	
	LoadSetting();
		
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CClock2026App::ExitInstance()
{
	SAFE_DELETE(m_pStatus);
	SAFE_DELETE(m_pHDdaVinci);

	CoUninitialize();     // ADDED

	return CWinApp::ExitInstance();
}

void CClock2026App::LoadSetting(void)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(m_pMainWnd);

	m_pStatus->m_strFileName = GetProfileString(_T("File"), _T("FileName"), m_pStatus->m_strFileName);
	m_pStatus->m_strFPRFileName = GetProfileString(_T("File"), _T("FPRFileName"), m_pStatus->m_strFPRFileName);
	m_pStatus->NewsConf()->SymbolFont(GetProfileString(_T("Symbol"), _T("Font"), m_pStatus->NewsConf()->SymbolFont()));
	m_pStatus->NewsConf()->SymbolSize(GetProfileString(_T("Symbol"), _T("Size"), m_pStatus->NewsConf()->SymbolSize()));
	m_pStatus->WeatherConf()->ExceptAir(GetProfileInt(_T("Weather"), _T("ExceptAir"), m_pStatus->WeatherConf()->ExceptAir()));


	CString strTemplate;

	strTemplate = GetProfileString(_T("File"), _T("LiveVideoTemplate"), m_pStatus->LiveVideoConf()->Template());

	m_pStatus->LiveVideoConf()->Template(strTemplate);

}

void CClock2026App::SaveSetting(void)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(m_pMainWnd);

	WriteProfileString(_T("File"), _T("FileName"), m_pStatus->m_strFileName);
	WriteProfileString(_T("File"), _T("FPRFileName"), m_pStatus->m_strFPRFileName);
	WriteProfileString(_T("File"), _T("LiveVideoTemplate"), m_pStatus->LiveVideoConf()->Template());
	WriteProfileString(_T("Symbol"), _T("Font"), m_pStatus->NewsConf()->SymbolFont());
	WriteProfileString(_T("Symbol"), _T("Size"), m_pStatus->NewsConf()->SymbolSize());
	WriteProfileInt(_T("Weather"), _T("ExceptAir"), m_pStatus->WeatherConf()->ExceptAir());
}

CString GetDataDir() { return CFileUtils::GetBaseDir() + TEXT("Data/"); }
CString GetLogsDir() { return CFileUtils::GetBaseDir() + TEXT("Logs/"); }
CString GetMirDir() { return CFileUtils::GetBaseDir() + TEXT("Mir/"); }
CString GetScrollDir() { return CFileUtils::GetBaseDir() + TEXT("Mir/Scroll/"); }
CString GetNoticeDir() { return CFileUtils::GetBaseDir() + TEXT("Mir/Notice/"); }
CString GetExportDir() { return CFileUtils::GetBaseDir() + TEXT("Mir/Notice/Export/"); };
CString GetManualUpDir() { return CFileUtils::GetBaseDir() + TEXT("Mir/ManualUp/"); }
CString GetLiveVideoDir() { return CFileUtils::GetBaseDir() + TEXT("Mir/LiveVideo/"); }
CString GetImagesDir() { return CFileUtils::GetBaseDir() + TEXT("Resource/Images/"); }
CString GetWeatherIconsDir() { return CFileUtils::GetBaseDir() + TEXT("Resource/Images/Weather_Icons/"); }