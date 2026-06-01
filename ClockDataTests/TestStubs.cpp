// TestStubs.cpp: stubs for main project symbols referenced by tests
#include "pch.h"
#include "ClockData.h"

DWORD g_dwVersion = 0;

// GetDataDir stub
CString GetDataDir()
{
    return CString(_T(""));
}

IMPLEMENT_DYNAMIC(CParentObject,      CObject)
IMPLEMENT_DYNAMIC(CClockDataObject,   CParentObject)
IMPLEMENT_DYNAMIC(CTimeString,        CClockDataObject)
IMPLEMENT_DYNAMIC(CCharInfo,          CClockDataObject)
IMPLEMENT_DYNAMIC(CCharInfoList,      CClockDataObject)
IMPLEMENT_DYNAMIC(CNewsData,          CClockDataObject)
IMPLEMENT_DYNAMIC(CNewsDataList,      CParentObject)
IMPLEMENT_DYNAMIC(CWeatherData,       CClockDataObject)
IMPLEMENT_DYNAMIC(CWeatherDataList,   CParentObject)
IMPLEMENT_DYNAMIC(CNoticeData,        CClockDataObject)
IMPLEMENT_DYNAMIC(CNoticeDataList,    CParentObject)
IMPLEMENT_DYNAMIC(CClockConfig,       CClockDataObject)
IMPLEMENT_DYNAMIC(CBigClockConfig,    CClockDataObject)
IMPLEMENT_DYNAMIC(CLogoConfig,        CClockDataObject)
IMPLEMENT_DYNAMIC(CNTPConfig,         CClockDataObject)
IMPLEMENT_DYNAMIC(CNewsConfig,        CClockDataObject)
IMPLEMENT_DYNAMIC(CWeatherConfig,     CClockDataObject)
IMPLEMENT_DYNAMIC(CNoticeConfig,      CClockDataObject)
IMPLEMENT_DYNAMIC(CManualUpConfig,    CClockDataObject)
IMPLEMENT_DYNAMIC(CLiveVideoConfig,   CClockDataObject)
IMPLEMENT_DYNAMIC(CUIFonts,           CObject)
IMPLEMENT_DYNAMIC(CStatus,            CClockDataObject)
