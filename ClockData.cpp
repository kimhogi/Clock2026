// ClockDataObject.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "ClockData.h"

// CClockDataObject
IMPLEMENT_DYNAMIC(CClockDataObject, CParentObject)
IMPLEMENT_DYNAMIC(CTimeString, CClockDataObject)
IMPLEMENT_DYNAMIC(CCharInfo, CClockDataObject)
IMPLEMENT_DYNAMIC(CCharInfoList, CClockDataObject)
IMPLEMENT_DYNAMIC(CNewsData, CClockDataObject)
IMPLEMENT_DYNAMIC(CNewsDataList, CTemplateList<CNewsData>)
IMPLEMENT_DYNAMIC(CWeatherData, CClockDataObject)
IMPLEMENT_DYNAMIC(CWeatherDataList, CTemplateList<CWeatherData>)
IMPLEMENT_DYNAMIC(CNoticeData, CClockDataObject)
IMPLEMENT_DYNAMIC(CNoticeDataList, CTemplateList<CNoticeData>)
IMPLEMENT_DYNAMIC(CClockConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CBigClockConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CLogoConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CLiveVideoConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CNTPConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CNewsConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CWeatherConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CNoticeConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CManualUpConfig, CClockDataObject)
IMPLEMENT_DYNAMIC(CUIFonts, CObject)
IMPLEMENT_DYNAMIC(CStatus, CClockDataObject)