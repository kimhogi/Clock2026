#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#include <afxmt.h>

#define __KBS_SPORTS__
#define __IMAGE_PROCESS__
#define __USE_IMAGE_DLG__
#define __ETCCTRL__
//#define __SOCKET__
//#define __SERIAL__
//#define __HDDAVINCI_VS_2013__
#define __HDDAVINCI_VS_2017__
//#define __UTB__
//#define __UTG__
#define __SPREAD__
#define __LIBXL__
//#define __X_DATA__
//#define __J_DATA__
#define __S_DATA__ 
#define __JSON__
//#define __XTREAM_TOOLKIT__
#define __CURL__
#define __LIVE_VIDEO__

//////////////////////////// 파일 관리를 위한 버전정보 ///////////////////////////////
static DWORD g_dwVersion = 1;      // 초기버전

#include "SportsInclude.h"
#include "SportsDef.h"