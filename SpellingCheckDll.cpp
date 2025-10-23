/////////////////////////////////////////////////////////////////////////
//			HwpSpeller Dll Call Class
//			(주)한글과컴퓨터 기술본부
/////////////////////////////////////////////////////////////////////////

//#include <stdafx.h>
#include "pch.h"
#include <io.h>
#include "SpellingCheckDll.h"
#include ".\spellingcheckdll.h"

// 맞춤법검사기 DLL Name Define
// ================================================================================
#define HWPSPELLER_DLLNAME "HwpSpeller.Dll"

// Dll Function Type Define
// ================================================================================
typedef SPLHND (fnHncSPLInitSpeller)(void) ;
typedef void (fnHncSPLCloseSpeller)(SPLHND) ;
typedef void (fnHncSPLRunOptionDlg)(HWND);
typedef BOOL (fnHncSPLGetOption)(int, LPINT);
typedef BOOL (fnHncSPLSetOption)(int, int);
typedef void (fnHncSPLDefaultSpellOption)();
typedef SPLHND (fnHncSPLCheckSpell)(SPLHND , LPCWSTR, UINT, LPINT , BOOL, BOOL) ;
typedef int (fnHncSPLSuggestion)(SPLHND , SPLERRHND , LPCWSTR, UINT, LPWSTR, int) ;
typedef void (fnHncSPLGetErrorPosLen)(SPLHND , SPLERRHND , LPINT , LPINT ) ;
typedef int (fnHncSPLGetErrorAttr)(SPLHND , SPLERRHND , int, LPWSTR) ;
typedef void (fnHncSPLLinkSuggestFunc)(SPLHND , LPVOID , int) ;
typedef void (fnHncSPLFreeErrorHandle)(SPLHND, SPLERRHND) ;
typedef int (fnHncSPLStartCheck)(SPLHND , LPCWSTR, UINT) ;
typedef void (fnHncSPLEndCheck)(SPLHND) ;
typedef SPLERRHND (fnHncSPLGetNextErrorHandle)(SPLHND) ;

typedef int (fnHncSPLGetAuxDicName)(SPLHND , LPWSTR, int) ;
typedef int (fnHncSPLGetAuxDicCount)(SPLHND ) ;
typedef BOOL (fnHncSPLIsAuxDicRunning)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLAuxDicReady)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLAuxDicClose)(SPLHND , LPCWSTR) ;
typedef int (fnHncSPLUserDicSearch)(SPLHND , LPCWSTR) ;
typedef int (fnHncSPLUserDicAdd)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLCanAddUserDic)(SPLHND , LPCWSTR) ;
typedef int (fnHncSPLUserDicItemCount)(SPLHND ) ;
typedef int (fnHncSPLGetUserDicItem)(SPLHND , int, LPWSTR, int) ;
typedef BOOL (fnHncSPLUserDicDelete)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLUserDicLoad)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLUserDicSave)(SPLHND , LPCWSTR) ;
typedef int (fnHncSPLIsOKDeleteAuxDic)(SPLHND , LPCWSTR) ;
typedef int (fnHncSPLDeleteAuxDic)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLAddSkipWord)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLGetSymbol)(SPLHND , UINT, LPWSTR) ;
typedef int (fnHncSPLGetSymbolCount)(SPLHND) ;
typedef int (fnHncSPLAddSymbol)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLDelSymbol)(SPLHND , UINT) ;
typedef int (fnHncSPLEditSymbol)(SPLHND , UINT , LPCWSTR ) ;
typedef int (fnHncSPLSearchSymbol)(SPLHND , LPCWSTR) ;
typedef int (fnHncSPLScanStartFromAuxDic)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLGetNextItemFromAuxDic)(SPLHND , LPWSTR , int ) ;
typedef BOOL (fnHncSPLAddAutoExecWord)(SPLHND , LPCWSTR, LPCWSTR, BOOL) ;
typedef BOOL (fnHncSPLIsAutoExecWord)(SPLHND) ;
typedef BOOL (fnHncSPLReadConfigAuxDic)(SPLHND , LPCWSTR , LPINT , LPVOID , LPINT ) ;
typedef BOOL (fnHncSPLWriteConfigAuxDic)(SPLHND , LPCWSTR , LPCWSTR , int , LPVOID , int ) ;
typedef int (fnHncSPLGetClassString)(SPLHND , int, LPWSTR) ;
typedef BOOL (fnHncSPLAuxDicDeliver)(SPLHND , LPCWSTR, LPWSTR) ;
typedef int (fnHncSPLGetErrorDepthType)(SPLHND , SPLERRHND , int , LPINT) ;
typedef int (fnHncSPLEnumDicView)(SPLHND , LPCWSTR , LPCWSTR *) ;
typedef void (fnHncSPLFreeDicView)(SPLHND);
typedef int (fnHncSPLGetWarnString)(SPLHND , int , LPWSTR , int ) ;
typedef BOOL (fnHncSPLGetWarnHelp)(SPLHND , LPCWSTR , LPWSTR , int ) ;
typedef BOOL (fnHncSPLMakeAuxDic)(SPLHND , LPVOID , LPCWSTR , int , LPVOID , int ) ;
typedef int (fnHncSPLSearchWarnString)(SPLHND , LPCWSTR , LPWSTR , int ) ;
typedef BOOL (fnHncSPLGetHelpInfo)(SPLHND , SPLERRHND , UINT , LPINT ) ;
typedef int (fnHncSPLGetFrequency)(SPLHND , LPCWSTR) ;
typedef BOOL (fnHncSPLGetUserHelpString)(int, BSTR FAR* );
typedef SPLERRHND (fnHncSPLExceptionCheck)(SPLHND, LPCWSTR, UINT, UINT) ;
typedef void (fnHncSPLSetSoundOn)(SPLHND, BOOL) ;

typedef int (fnHncSPLGetBaseString)(SPLHND , LPCWSTR , LPVOID ) ;
typedef int (fnHncSPLIsEnableChangeVerb)(SPLHND , LPCWSTR , LPCWSTR ) ;
typedef BOOL (fnHncSPLGetBaseVerbList)(SPLHND , LPCWSTR , LPVOID ) ;
typedef int (fnHncSPLGetChangedWord)(SPLHND , BOOL , LPCWSTR , LPCWSTR , LPCWSTR , LPWSTR , int ) ;

typedef BOOL (fnHncSPLGetBaseStringExt)(SPLHND , LPCWSTR , LPWSTR , UINT ) ;

typedef void (fnHncSPLLinkUserHanjaFunc)(SPLHND , LPVOID) ;

// Dll Import Function Global Definition
// ================================================================================
static fnHncSPLInitSpeller *pHncSPLInitSpeller = NULL ;
static fnHncSPLCloseSpeller *pHncSPLCloseSpeller = NULL ;
static fnHncSPLCheckSpell *pHncSPLCheckSpell = NULL ;
static fnHncSPLSuggestion *pHncSPLSuggestion = NULL ;
static fnHncSPLGetErrorPosLen *pHncSPLGetErrorPosLen = NULL ;
static fnHncSPLGetErrorAttr *pHncSPLGetErrorAttr = NULL ;
static fnHncSPLLinkSuggestFunc *pHncSPLLinkSuggestFunc = NULL ;
static fnHncSPLFreeErrorHandle *pHncSPLFreeErrorHandle = NULL ;
static fnHncSPLStartCheck *pHncSPLStartCheck = NULL ;
static fnHncSPLEndCheck *pHncSPLEndCheck = NULL ;
static fnHncSPLGetNextErrorHandle *pHncSPLGetNextErrorHandle = NULL ;

static fnHncSPLRunOptionDlg *pHncSPLRunOptionDlg = NULL;
static fnHncSPLGetOption *pHncSPLGetOption = NULL;
static fnHncSPLSetOption *pHncSPLSetOption = NULL;
static fnHncSPLDefaultSpellOption *pHncSPLDefaultSpellOption = NULL;
static fnHncSPLGetAuxDicName *pHncSPLGetAuxDicName = NULL ;
static fnHncSPLGetAuxDicCount *pHncSPLGetAuxDicCount = NULL ;
static fnHncSPLIsAuxDicRunning *pHncSPLIsAuxDicRunning = NULL ;
static fnHncSPLAuxDicReady *pHncSPLAuxDicReady = NULL ;
static fnHncSPLAuxDicClose *pHncSPLAuxDicClose = NULL ;
static fnHncSPLUserDicSearch *pHncSPLUserDicSearch = NULL ;
static fnHncSPLUserDicAdd *pHncSPLUserDicAdd = NULL ;
static fnHncSPLCanAddUserDic *pHncSPLCanAddUserDic = NULL ;
static fnHncSPLUserDicItemCount *pHncSPLUserDicItemCount = NULL ;
static fnHncSPLGetUserDicItem *pHncSPLGetUserDicItem = NULL ;
static fnHncSPLUserDicDelete *pHncSPLUserDicDelete = NULL ;
static fnHncSPLUserDicLoad *pHncSPLUserDicLoad = NULL ;
static fnHncSPLUserDicSave *pHncSPLUserDicSave = NULL ;
static fnHncSPLIsOKDeleteAuxDic *pHncSPLIsOKDeleteAuxDic = NULL ;
static fnHncSPLDeleteAuxDic *pHncSPLDeleteAuxDic = NULL ;
static fnHncSPLAddSkipWord *pHncSPLAddSkipWord = NULL ;
static fnHncSPLGetSymbol *pHncSPLGetSymbol = NULL ;
static fnHncSPLGetSymbolCount *pHncSPLGetSymbolCount = NULL ;
static fnHncSPLAddSymbol *pHncSPLAddSymbol = NULL ;
static fnHncSPLDelSymbol *pHncSPLDelSymbol = NULL ;
static fnHncSPLEditSymbol *pHncSPLEditSymbol = NULL ;
static fnHncSPLSearchSymbol *pHncSPLSearchSymbol = NULL ;
static fnHncSPLScanStartFromAuxDic *pHncSPLScanStartFromAuxDic = NULL ;
static fnHncSPLGetNextItemFromAuxDic *pHncSPLGetNextItemFromAuxDic = NULL ;
static fnHncSPLAddAutoExecWord *pHncSPLAddAutoExecWord = NULL ;
static fnHncSPLIsAutoExecWord *pHncSPLIsAutoExecWord = NULL ;
static fnHncSPLReadConfigAuxDic *pHncSPLReadConfigAuxDic = NULL ;
static fnHncSPLWriteConfigAuxDic *pHncSPLWriteConfigAuxDic = NULL ;
static fnHncSPLGetClassString *pHncSPLGetClassString = NULL ;
static fnHncSPLAuxDicDeliver *pHncSPLAuxDicDeliver = NULL ;
static fnHncSPLGetErrorDepthType *pHncSPLGetErrorDepthType = NULL ;
static fnHncSPLEnumDicView *pHncSPLEnumDicView = NULL ;
static fnHncSPLFreeDicView *pHncSPLFreeDicView = NULL ;
static fnHncSPLGetWarnString *pHncSPLGetWarnString = NULL ;
static fnHncSPLGetWarnHelp *pHncSPLGetWarnHelp = NULL ;
static fnHncSPLMakeAuxDic *pHncSPLMakeAuxDic = NULL ;
static fnHncSPLSearchWarnString *pHncSPLSearchWarnString = NULL ;
static fnHncSPLGetHelpInfo *pHncSPLGetHelpInfo = NULL ;

static fnHncSPLGetUserHelpString *pHncSPLGetUserHelpString = NULL;
static fnHncSPLGetFrequency *pHncSPLGetFrequency = NULL ;
static fnHncSPLExceptionCheck *pHncSPLExceptionCheck = NULL ;
static fnHncSPLSetSoundOn *pHncSPLSetSoundOn = NULL ;

static fnHncSPLGetBaseString *pHncSPLGetBaseString = NULL ;
static fnHncSPLIsEnableChangeVerb *pHncSPLIsEnableChangeVerb = NULL ;
static fnHncSPLGetBaseVerbList *pHncSPLGetBaseVerbList = NULL ;
static fnHncSPLGetChangedWord *pHncSPLGetChangedWord = NULL ;

static fnHncSPLGetBaseStringExt *pHncSPLGetBaseStringExt = NULL ;

static fnHncSPLLinkUserHanjaFunc *pHncSPLLinkUserHanjaFunc = NULL ;

void CSpellingCheckDll::InitSpellerFunction(void)
{
	pHncSPLInitSpeller = NULL ;
	pHncSPLCloseSpeller = NULL ;
	pHncSPLCheckSpell = NULL ;
	pHncSPLSuggestion = NULL ;
	pHncSPLGetErrorPosLen = NULL ;
	pHncSPLGetErrorAttr = NULL ;
	pHncSPLLinkSuggestFunc = NULL ;
	pHncSPLFreeErrorHandle = NULL ;
	pHncSPLStartCheck = NULL ;
	pHncSPLEndCheck = NULL ;  
	pHncSPLGetNextErrorHandle = NULL ;

	pHncSPLGetOption = NULL;
	pHncSPLSetOption = NULL;
	pHncSPLGetAuxDicName = NULL ;
	pHncSPLGetAuxDicCount = NULL ;
	pHncSPLIsAuxDicRunning = NULL ;
	pHncSPLAuxDicReady = NULL;
	pHncSPLAuxDicClose = NULL ;
	pHncSPLUserDicSearch = NULL ;
	pHncSPLUserDicAdd = NULL ;
	pHncSPLCanAddUserDic = NULL ;
	pHncSPLUserDicItemCount = NULL ;
	pHncSPLGetUserDicItem = NULL ;
	pHncSPLUserDicDelete = NULL ;
	pHncSPLUserDicLoad = NULL ;
	pHncSPLUserDicSave = NULL ;
	pHncSPLIsOKDeleteAuxDic = NULL ;
	pHncSPLDeleteAuxDic = NULL ;
	pHncSPLAddSkipWord = NULL ;
	pHncSPLGetSymbol = NULL ;
	pHncSPLGetSymbolCount = NULL ;
	pHncSPLAddSymbol = NULL ;
	pHncSPLDelSymbol = NULL ;
	pHncSPLEditSymbol = NULL ;
	pHncSPLSearchSymbol = NULL ;
	pHncSPLScanStartFromAuxDic = NULL ;
	pHncSPLGetNextItemFromAuxDic = NULL ;
	pHncSPLAddAutoExecWord = NULL ;
	pHncSPLIsAutoExecWord = NULL ;
	pHncSPLReadConfigAuxDic = NULL ;
	pHncSPLWriteConfigAuxDic = NULL ;
	pHncSPLGetClassString = NULL ;
	pHncSPLAuxDicDeliver = NULL ;
	pHncSPLGetErrorDepthType = NULL ;
	pHncSPLEnumDicView = NULL ;
	pHncSPLFreeDicView = NULL ;
	pHncSPLGetWarnString = NULL ;
	pHncSPLGetWarnHelp = NULL ;
	pHncSPLMakeAuxDic = NULL ;
	pHncSPLSearchWarnString = NULL ;
	pHncSPLGetHelpInfo = NULL ;
	pHncSPLGetUserHelpString = NULL;
	pHncSPLGetFrequency = NULL ;
	pHncSPLExceptionCheck = NULL ;
	pHncSPLSetSoundOn = NULL ;
	
	pHncSPLGetBaseString = NULL ;
	pHncSPLIsEnableChangeVerb = NULL ;
	pHncSPLGetBaseVerbList = NULL ;
	pHncSPLGetChangedWord = NULL ;

	pHncSPLGetBaseStringExt = NULL ;

	pHncSPLLinkUserHanjaFunc = NULL ;
}

// Dll Global Function Link
// ================================================================================
BOOL CSpellingCheckDll::LinkSpellerFunction(void)
{
	pHncSPLInitSpeller = (fnHncSPLInitSpeller *)GetProcAddress(_hInstance, "HncSPLInitSpeller") ;
	pHncSPLCloseSpeller = (fnHncSPLCloseSpeller *)GetProcAddress(_hInstance, "HncSPLCloseSpeller") ;
	pHncSPLCheckSpell = (fnHncSPLCheckSpell *)GetProcAddress(_hInstance, "HncSPLCheckSpell") ;
	pHncSPLSuggestion = (fnHncSPLSuggestion *)GetProcAddress(_hInstance, "HncSPLSuggestion") ;
	pHncSPLGetErrorPosLen = (fnHncSPLGetErrorPosLen *)GetProcAddress(_hInstance, "HncSPLGetErrorPosLen") ;
	pHncSPLGetErrorAttr = (fnHncSPLGetErrorAttr *)GetProcAddress(_hInstance, "HncSPLGetErrorAttr") ;
	pHncSPLLinkSuggestFunc = (fnHncSPLLinkSuggestFunc *)GetProcAddress(_hInstance, "HncSPLLinkSuggestFunc") ;
	pHncSPLFreeErrorHandle = (fnHncSPLFreeErrorHandle *)GetProcAddress(_hInstance, "HncSPLFreeErrorHandle") ;
	pHncSPLStartCheck = (fnHncSPLStartCheck *)GetProcAddress(_hInstance, "HncSPLStartCheck") ;
	pHncSPLEndCheck = (fnHncSPLEndCheck *)GetProcAddress(_hInstance, "HncSPLEndCheck") ;
	pHncSPLGetNextErrorHandle = (fnHncSPLGetNextErrorHandle *)GetProcAddress(_hInstance, "HncSPLGetNextErrorHandle") ;

	pHncSPLRunOptionDlg = (fnHncSPLRunOptionDlg *)GetProcAddress(_hInstance, "HncSPLOptionDialog");
	pHncSPLGetOption = (fnHncSPLGetOption *)GetProcAddress(_hInstance, "HncSPLGetOption");
	pHncSPLSetOption = (fnHncSPLSetOption *)GetProcAddress(_hInstance, "HncSPLSetOption");
	pHncSPLDefaultSpellOption = (fnHncSPLDefaultSpellOption *)GetProcAddress(_hInstance, "HncSPLDefaultSpellOption");
	pHncSPLGetAuxDicName = (fnHncSPLGetAuxDicName *)GetProcAddress(_hInstance, "HncSPLGetAuxDicName") ;
	pHncSPLGetAuxDicCount = (fnHncSPLGetAuxDicCount *)GetProcAddress(_hInstance, "HncSPLGetAuxDicCount") ;
	pHncSPLIsAuxDicRunning = (fnHncSPLIsAuxDicRunning *)GetProcAddress(_hInstance, "HncSPLIsAuxDicRunning") ;
	pHncSPLAuxDicReady = (fnHncSPLAuxDicReady *)GetProcAddress(_hInstance, "HncSPLAuxDicReady") ;
	pHncSPLAuxDicClose = (fnHncSPLAuxDicClose *)GetProcAddress(_hInstance, "HncSPLAuxDicClose") ;
	pHncSPLUserDicSearch = (fnHncSPLUserDicSearch *)GetProcAddress(_hInstance, "HncSPLUserDicSearch") ;
	pHncSPLUserDicAdd = (fnHncSPLUserDicAdd *)GetProcAddress(_hInstance, "HncSPLUserDicAdd") ;
	pHncSPLCanAddUserDic = (fnHncSPLCanAddUserDic *)GetProcAddress(_hInstance, "HncSPLCanAddUserDic") ;
	pHncSPLUserDicItemCount = (fnHncSPLUserDicItemCount *)GetProcAddress(_hInstance, "HncSPLUserDicItemCount") ;
	pHncSPLGetUserDicItem = (fnHncSPLGetUserDicItem *)GetProcAddress(_hInstance, "HncSPLGetUserDicItem") ;
	pHncSPLUserDicDelete = (fnHncSPLUserDicDelete *)GetProcAddress(_hInstance, "HncSPLUserDicDelete") ;
	pHncSPLUserDicLoad = (fnHncSPLUserDicLoad *)GetProcAddress(_hInstance, "HncSPLUserDicLoad") ;
	pHncSPLUserDicSave = (fnHncSPLUserDicSave *)GetProcAddress(_hInstance, "HncSPLUserDicSave") ;
	pHncSPLIsOKDeleteAuxDic = (fnHncSPLIsOKDeleteAuxDic *)GetProcAddress(_hInstance, "HncSPLIsOKDeleteAuxDic") ;
	pHncSPLDeleteAuxDic = (fnHncSPLDeleteAuxDic *)GetProcAddress(_hInstance, "HncSPLDeleteAuxDic") ;
	pHncSPLAddSkipWord = (fnHncSPLAddSkipWord *)GetProcAddress(_hInstance, "HncSPLAddSkipWord") ;
	pHncSPLGetSymbol = (fnHncSPLGetSymbol *)GetProcAddress(_hInstance, "HncSPLGetSymbol") ;
	pHncSPLGetSymbolCount = (fnHncSPLGetSymbolCount *)GetProcAddress(_hInstance, "HncSPLGetSymbolCount") ;
	pHncSPLAddSymbol = (fnHncSPLAddSymbol *)GetProcAddress(_hInstance, "HncSPLAddSymbol") ;
	pHncSPLDelSymbol = (fnHncSPLDelSymbol *)GetProcAddress(_hInstance, "HncSPLDelSymbol") ;
	pHncSPLEditSymbol = (fnHncSPLEditSymbol *)GetProcAddress(_hInstance, "HncSPLEditSymbol") ;
	pHncSPLSearchSymbol = (fnHncSPLSearchSymbol *)GetProcAddress(_hInstance, "HncSPLSearchSymbol") ;
	pHncSPLScanStartFromAuxDic = (fnHncSPLScanStartFromAuxDic *)GetProcAddress(_hInstance, "HncSPLScanStartFromAuxDic") ;
	pHncSPLGetNextItemFromAuxDic = (fnHncSPLGetNextItemFromAuxDic *)GetProcAddress(_hInstance, "HncSPLGetNextItemFromAuxDic") ;
	pHncSPLAddAutoExecWord = (fnHncSPLAddAutoExecWord *)GetProcAddress(_hInstance, "HncSPLAddAutoExecWord") ;
	pHncSPLIsAutoExecWord = (fnHncSPLIsAutoExecWord *)GetProcAddress(_hInstance, "HncSPLIsAutoExecWord") ;
	pHncSPLReadConfigAuxDic = (fnHncSPLReadConfigAuxDic *)GetProcAddress(_hInstance, "HncSPLReadConfigAuxDic") ;
	pHncSPLWriteConfigAuxDic = (fnHncSPLWriteConfigAuxDic *)GetProcAddress(_hInstance, "HncSPLWriteConfigAuxDic") ;
	pHncSPLGetClassString = (fnHncSPLGetClassString *)GetProcAddress(_hInstance, "HncSPLGetClassString") ;
	pHncSPLAuxDicDeliver = (fnHncSPLAuxDicDeliver *)GetProcAddress(_hInstance, "HncSPLAuxDicDeliver") ;
	pHncSPLGetErrorDepthType = (fnHncSPLGetErrorDepthType *)GetProcAddress(_hInstance, "HncSPLGetErrorDepthType") ;
	pHncSPLEnumDicView = (fnHncSPLEnumDicView *)GetProcAddress(_hInstance, "HncSPLEnumDicView") ;
	pHncSPLFreeDicView = (fnHncSPLFreeDicView *)GetProcAddress(_hInstance, "HncSPLFreeDicView") ;
	pHncSPLGetWarnString = (fnHncSPLGetWarnString *)GetProcAddress(_hInstance, "HncSPLGetWarnString") ;
	pHncSPLGetWarnHelp = (fnHncSPLGetWarnHelp *)GetProcAddress(_hInstance, "HncSPLGetWarnHelp") ;
	pHncSPLMakeAuxDic = (fnHncSPLMakeAuxDic *)GetProcAddress(_hInstance, "HncSPLMakeAuxDic") ;
	pHncSPLSearchWarnString = (fnHncSPLSearchWarnString *)GetProcAddress(_hInstance, "HncSPLSearchWarnString") ;
	pHncSPLGetHelpInfo = (fnHncSPLGetHelpInfo *)GetProcAddress(_hInstance, "HncSPLGetHelpInfo") ;
	pHncSPLGetUserHelpString = (fnHncSPLGetUserHelpString *)GetProcAddress(_hInstance, "HncSPLGetUserHelpString");

	pHncSPLGetFrequency = (fnHncSPLGetFrequency *)GetProcAddress(_hInstance, "HncSPLGetFrequency") ;
	pHncSPLExceptionCheck = (fnHncSPLExceptionCheck *)GetProcAddress(_hInstance, "HncSPLExceptionCheck") ;
	pHncSPLSetSoundOn = (fnHncSPLSetSoundOn *)GetProcAddress(_hInstance, "HncSPLSetSoundOn") ;
	
	pHncSPLGetBaseString = (fnHncSPLGetBaseString *)GetProcAddress(_hInstance, "HncSPLGetBaseString") ;
	pHncSPLIsEnableChangeVerb = (fnHncSPLIsEnableChangeVerb *)GetProcAddress(_hInstance, "HncSPLIsEnableChangeVerb") ;
	pHncSPLGetBaseVerbList = (fnHncSPLGetBaseVerbList *)GetProcAddress(_hInstance, "HncSPLGetBaseVerbList") ;
	pHncSPLGetChangedWord = (fnHncSPLGetChangedWord *)GetProcAddress(_hInstance, "HncSPLGetChangedWord") ;

	pHncSPLGetBaseStringExt = (fnHncSPLGetBaseStringExt *)GetProcAddress(_hInstance, "HncSPLGetBaseStringExt") ;

	pHncSPLLinkUserHanjaFunc = (fnHncSPLLinkUserHanjaFunc *)GetProcAddress(_hInstance, "HncSPLLinkUserHanjaFunc") ;

	// main functions에 대한 safe 검사
	// 그외의 functions은 link가 되지 않아도 실행되도록 한다.
	if (!(pHncSPLInitSpeller && pHncSPLCloseSpeller && pHncSPLCheckSpell && pHncSPLSuggestion 
		&& pHncSPLGetErrorPosLen && pHncSPLGetErrorAttr && pHncSPLLinkSuggestFunc 
		&& pHncSPLStartCheck && pHncSPLEndCheck && pHncSPLGetNextErrorHandle
		&& pHncSPLFreeErrorHandle)) return FALSE ;

	return TRUE ;
}

CSpellingCheckDll::CSpellingCheckDll()
{
	_hInstance = NULL ;
	_dllhandle = SPLHND_ERROR ;

	m_LPSuggestFunc = NULL ;
	m_iSuggestCount = 0 ;
	m_LPUserHanjaFunc = NULL ;

	InitSpellerFunction() ;
}

// _dllhandle을 Free한다.
CSpellingCheckDll::~CSpellingCheckDll()
{
	if (_dllhandle == SPLHND_ERROR) return ;

	if (pHncSPLCloseSpeller)
		(*pHncSPLCloseSpeller)(_dllhandle) ;

	InitSpellerFunction() ;
}

// Dll이 Load 되었는 지 살핀다.
// Load 되어 있지 않으면 Load하고 새로운 Dll Handle을 얻는다.
BOOL CSpellingCheckDll::IsLoadedDLL (void)
{
	if (_dllhandle != SPLHND_ERROR) 
		return TRUE ;
	
/*	
	TCHAR path_buffer[_MAX_PATH] = { 0, 0, };
	_tcscat( (path_buffer), HWPSPELLER_DLLNAME) ;


	char path[MAX_PATH];
	strcpy(path, HWPSPELLER_DLLNAME);
	
	int acs = _access((path), 0);
	*/

	DWORD err;
	CString path(_T(HWPSPELLER_DLLNAME));

	if (!_hInstance) {
		if ((_hInstance = LoadLibrary(path)) == NULL) {			
			err = GetLastError();
			return FALSE ;
		}

		if (LinkSpellerFunction() == FALSE) {
			FreeLibrary(_hInstance) ;			
			return FALSE ;
		}
	}

	if ((_dllhandle = (*pHncSPLInitSpeller)()) == SPLHND_ERROR) {		
		return FALSE ;
	}

	//SuggestFunc Link
	if (m_LPSuggestFunc && (m_iSuggestCount > 0) && pHncSPLLinkSuggestFunc)
		(*pHncSPLLinkSuggestFunc)(_dllhandle, m_LPSuggestFunc, m_iSuggestCount) ;

	//HanjaFunc Link...
	if (m_LPUserHanjaFunc && pHncSPLLinkUserHanjaFunc)
		(*pHncSPLLinkUserHanjaFunc)(_dllhandle, m_LPUserHanjaFunc) ;

	return TRUE ;
}

BOOL CSpellingCheckDll::RunOptionDlg(HWND hWnd)
{
	if(IsLoadedDLL() == FALSE || pHncSPLRunOptionDlg == NULL) {
		return FALSE;
	}
	
	(*pHncSPLRunOptionDlg)(hWnd);
	return TRUE;
}

BOOL CSpellingCheckDll::GetSPLOption(int sploption, int *flag)
{
	if(IsLoadedDLL() == FALSE || pHncSPLGetOption == NULL) {
		return FALSE;
	}

	return (*pHncSPLGetOption)(sploption, flag);
}

BOOL CSpellingCheckDll::SetSPLOption(int sploption, int flag)
{
	if(IsLoadedDLL() == FALSE || pHncSPLSetOption == NULL) {
		return FALSE;
	}

	return (*pHncSPLSetOption)(sploption, flag);
}

void CSpellingCheckDll::SetDefaultOption()
{
	if(IsLoadedDLL() == FALSE || pHncSPLDefaultSpellOption == NULL) {
		return ;
	}

	(*pHncSPLDefaultSpellOption)();
}

SPLERRHND CSpellingCheckDll::CheckSpell (LPCWSTR inputstr, UINT inputlen, LPINT chkpos, BOOL bNewPara, BOOL bChangedWord)
{
	if (IsLoadedDLL() == FALSE) return SPLHND_ERROR ;

	return (*pHncSPLCheckSpell)(_dllhandle, inputstr, inputlen, chkpos, bNewPara, bChangedWord) ;
}

int CSpellingCheckDll::Suggestion (SPLERRHND _errhandle, LPCWSTR inputstr, UINT inputlen, LPWSTR repword, int repwordlen)
{
	if (IsLoadedDLL() == FALSE || _errhandle == SPLHND_ERROR) {
		return 0 ;
	}
	
	return (*pHncSPLSuggestion)(_dllhandle, _errhandle, inputstr, inputlen, repword, repwordlen) ;
}

void CSpellingCheckDll::GetErrorPosLen (SPLERRHND _errhandle, LPINT pos, LPINT len)
{
	if (IsLoadedDLL() == FALSE || _errhandle == SPLHND_ERROR) {
		return ;
	}

	(*pHncSPLGetErrorPosLen)(_dllhandle, _errhandle, pos, len) ;
}

int CSpellingCheckDll::GetErrorAttr (SPLERRHND _errhandle, int idx, LPWSTR attrstr)
{
	if (IsLoadedDLL() == FALSE || _errhandle == SPLHND_ERROR) {
		return FALSE ;
	}

	return (*pHncSPLGetErrorAttr)(_dllhandle, _errhandle, idx, attrstr) ;
}

int CSpellingCheckDll::GetAuxDicName (LPWSTR Auxdicnamebuf, int bufsize)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetAuxDicName)
		return (*pHncSPLGetAuxDicName)(_dllhandle, Auxdicnamebuf, bufsize) ;

	return FALSE ;
}

int CSpellingCheckDll::GetAuxDicCount (void)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetAuxDicCount)
		return (*pHncSPLGetAuxDicCount)(_dllhandle) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::IsAuxDicRunning(LPCWSTR auxtitle)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLIsAuxDicRunning)
		return (*pHncSPLIsAuxDicRunning)(_dllhandle, auxtitle) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::AuxDicReady(LPCWSTR auxtitle)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLAuxDicReady)
		return (*pHncSPLAuxDicReady)(_dllhandle, auxtitle) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::AuxDicClose(LPCWSTR auxtitle)
{
	if (IsLoadedDLL() == FALSE)	return FALSE ;

	if (pHncSPLAuxDicClose)
		return (*pHncSPLAuxDicClose)(_dllhandle, auxtitle) ;

	return FALSE ;
}

int CSpellingCheckDll::UserdicSearch(LPCWSTR userword)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLUserDicSearch)
		return (*pHncSPLUserDicSearch)(_dllhandle, userword) ;

	return FALSE ;
}

/*	
	개인 사전의 index (zero-base)
	-1 : Duplication Error
	-2 : Cannot insert Error
*/
int CSpellingCheckDll::UserdicAdd(LPCWSTR userword)
{
	if (IsLoadedDLL() == FALSE) return -1 ;

	if (pHncSPLUserDicAdd)
		return (*pHncSPLUserDicAdd)(_dllhandle, userword) ;

	return -1 ;
}

BOOL CSpellingCheckDll::UserdicAddable(LPCWSTR userword)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLCanAddUserDic)
		return (*pHncSPLCanAddUserDic)(_dllhandle, userword) ;

	return FALSE ;
}

int CSpellingCheckDll::UserdicItemCount(void)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLUserDicItemCount)
		return (*pHncSPLUserDicItemCount)(_dllhandle) ;

	return FALSE ;
}

int CSpellingCheckDll::UserdicItem(int idx, LPWSTR userword, int bufferlen)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetUserDicItem)
		return (*pHncSPLGetUserDicItem)(_dllhandle, idx, userword, bufferlen) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::UserdicDelete(LPCWSTR userword)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLUserDicDelete)
		return (*pHncSPLUserDicDelete)(_dllhandle, userword) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::UserdicLoad(LPCWSTR usrdicpath)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLUserDicLoad)
		return (*pHncSPLUserDicLoad)(_dllhandle, usrdicpath) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::UserdicSave(LPCWSTR usrdicpath)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLUserDicSave)
		return (*pHncSPLUserDicSave)(_dllhandle, usrdicpath) ;

	return FALSE ;
}

int CSpellingCheckDll::IsOKDeleteAuxdic(LPCWSTR titlename)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLIsOKDeleteAuxDic)
		return (*pHncSPLIsOKDeleteAuxDic)(_dllhandle, titlename) ;

	return FALSE ;
}

int CSpellingCheckDll::AuxdicDelete(LPCWSTR titlename)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLDeleteAuxDic)
		return (*pHncSPLDeleteAuxDic)(_dllhandle, titlename) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::AddSkipWord(LPCWSTR skipword)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLAddSkipWord)
		return (*pHncSPLAddSkipWord)(_dllhandle, skipword) ;

	return FALSE ;
}


//==============================================================================
// 맞춤법 도우미 관련 Functions
//==============================================================================

int CSpellingCheckDll::StartCheck(LPCWSTR inputstr, UINT inputlen)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;
	
	if ( pHncSPLStartCheck != NULL ) 
		return (*pHncSPLStartCheck)(_dllhandle, inputstr, inputlen) ;
	else 
	{		
		TRACE( _T("CSpellingCheckDll::StartCheck  -  error \n") ) ; 
		return FALSE ; 
	}
}

void CSpellingCheckDll::EndCheck(void)
{
	if (IsLoadedDLL() == FALSE) return ;

	(*pHncSPLEndCheck)(_dllhandle) ;
}

SPLERRHND CSpellingCheckDll::GetNextErrorHandle(void)
{
	if (IsLoadedDLL() == FALSE) return SPLHND_ERROR ;

	return (*pHncSPLGetNextErrorHandle)(_dllhandle) ;
}


//==============================================================================
// 등록 기호 관련 Member Functions
//==============================================================================

BOOL CSpellingCheckDll::GetSymbol(UINT index, LPWSTR symbol)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetSymbol)
		return (*pHncSPLGetSymbol)(_dllhandle, index, symbol) ;

	return FALSE ;
}

int CSpellingCheckDll::GetSymbolCount(void)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetSymbolCount)
		return (*pHncSPLGetSymbolCount)(_dllhandle) ;

	return FALSE ;
}

int CSpellingCheckDll::AddSymbol(LPCWSTR symbol)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;
	
	if (pHncSPLAddSymbol)
		return (*pHncSPLAddSymbol)(_dllhandle, symbol) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::DelSymbol(UINT index)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLDelSymbol)
		return (*pHncSPLDelSymbol)(_dllhandle, index) ;

	return FALSE ;
}

int CSpellingCheckDll::EditSymbol(UINT index, LPCWSTR symbol)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLEditSymbol)
		return (*pHncSPLEditSymbol)(_dllhandle, index, symbol) ;

	return FALSE ;
}

int CSpellingCheckDll::SearchSymbol(LPCWSTR symbol)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLSearchSymbol)
		return (*pHncSPLSearchSymbol)(_dllhandle, symbol) ;

	return FALSE ;
}

int CSpellingCheckDll::ScanStartFromAuxDic(LPCWSTR titlename)
{
	if (IsLoadedDLL() == FALSE) return -1 ;

	if (pHncSPLScanStartFromAuxDic)
		return (*pHncSPLScanStartFromAuxDic)(_dllhandle, titlename) ;

	return -1 ;
}

BOOL CSpellingCheckDll::GetNextItemFromAuxDic(LPWSTR itemstr, int maxlen)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetNextItemFromAuxDic)
		return (*pHncSPLGetNextItemFromAuxDic)(_dllhandle, itemstr, maxlen) ;

	return FALSE ;
}

void CSpellingCheckDll::FreeErrorHandle (SPLERRHND _errorhandle)
{
	if (IsLoadedDLL() == FALSE || _errorhandle == SPLHND_ERROR) {
		return ;
	}

	(*pHncSPLFreeErrorHandle)(_dllhandle, _errorhandle) ;
}

BOOL CSpellingCheckDll::AddAutoExecWord(LPCWSTR errword, LPCWSTR replaceword, BOOL IsAutoExec)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLAddAutoExecWord)
		return (*pHncSPLAddAutoExecWord)(_dllhandle, errword, replaceword, IsAutoExec) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::IsAutoExecWord(void)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLIsAutoExecWord)
		return (*pHncSPLIsAutoExecWord)(_dllhandle) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::ReadConfigAuxDic (LPCWSTR titlename, LPINT classtype, LPVOID pool, LPINT maxsize)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLReadConfigAuxDic)
		return (*pHncSPLReadConfigAuxDic)(_dllhandle, titlename, classtype, pool, maxsize) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::WriteConfigAuxDic (LPCWSTR titlename, LPCWSTR newtitle, int classtype, LPVOID pool, int size)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLWriteConfigAuxDic)
		return (*pHncSPLWriteConfigAuxDic)(_dllhandle, titlename, newtitle, classtype, pool, size) ;

	return FALSE ;
}

int CSpellingCheckDll::GetClassString(int index, LPWSTR getstr)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetClassString)
		return (*pHncSPLGetClassString)(_dllhandle, index, getstr) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::DeliverAuxDic(LPCWSTR filename, LPWSTR titlename)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLAuxDicDeliver)
		return (*pHncSPLAuxDicDeliver)(_dllhandle, filename, titlename) ;

	return FALSE ;
}

int CSpellingCheckDll::GetErrorDepthType(SPLERRHND _errorhandle, int idx, LPINT errortype)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetErrorDepthType)
		return (*pHncSPLGetErrorDepthType)(_dllhandle, _errorhandle, idx, errortype) ;

	return FALSE ;
}

int CSpellingCheckDll::EnumDicView(LPCWSTR keyword, LPCWSTR *DicViewBuffer)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLEnumDicView)
		return (*pHncSPLEnumDicView)(_dllhandle, keyword, DicViewBuffer) ;

	return FALSE ;
}

void CSpellingCheckDll::FreeDicView(void)
{
	if (IsLoadedDLL() == FALSE) return ;

	if (pHncSPLFreeDicView)
		(*pHncSPLFreeDicView)(_dllhandle) ;
}

int CSpellingCheckDll::GetBaseString(LPCWSTR keyword, LPVOID getfunc)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetBaseString)
		return (*pHncSPLGetBaseString)(_dllhandle, keyword, getfunc) ;

	return FALSE ;
}

int CSpellingCheckDll::IsEnableChangeVerb (LPCWSTR findbaseword, LPCWSTR replacebaseword)
{
	if (IsLoadedDLL() == FALSE) return -1 ;

	if (pHncSPLIsEnableChangeVerb)
		return (*pHncSPLIsEnableChangeVerb)(_dllhandle, findbaseword, replacebaseword) ;

	return -1 ;
}

BOOL CSpellingCheckDll::GetBaseVerbList (LPCWSTR findbaseword, LPVOID getfunc)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetBaseVerbList)
		return (*pHncSPLGetBaseVerbList)(_dllhandle, findbaseword, getfunc) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::GetChangedWord (BOOL IsChangeVerb, LPCWSTR findbaseword, LPCWSTR findword,
									   LPCWSTR repbaseword, LPWSTR replaceword, int maxsize)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetChangedWord)
		return (*pHncSPLGetChangedWord)(_dllhandle, IsChangeVerb, findbaseword, findword, repbaseword, replaceword, maxsize) ;
	
	return FALSE ;
}

int CSpellingCheckDll::GetWarnString(int index, LPWSTR stringbuf, int maxlen)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetWarnString)
		return (*pHncSPLGetWarnString)(_dllhandle, index, stringbuf, maxlen) ;
	
	return FALSE ;
}

BOOL CSpellingCheckDll::GetWarnHelp(LPCWSTR searchword, LPWSTR helpbuf, int maxlen)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetWarnHelp)
		return (*pHncSPLGetWarnHelp)(_dllhandle, searchword, helpbuf, maxlen) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::MakeAuxDic(LPVOID getfunc, LPCWSTR TitleName, int classtype, LPVOID config, int configsize)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLMakeAuxDic)
		return (*pHncSPLMakeAuxDic)(_dllhandle, getfunc, TitleName, classtype, config, configsize) ;

	return FALSE ;
}

int CSpellingCheckDll::SearchWarnString(LPCWSTR searchword, LPWSTR stringbuf, int maxlen)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLSearchWarnString)
		return (*pHncSPLSearchWarnString)(_dllhandle, searchword, stringbuf, maxlen) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::GetHelpInfo(SPLERRHND errorhandle, UINT index, LPINT helpnum)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetHelpInfo)
		return (*pHncSPLGetHelpInfo)(_dllhandle, errorhandle, index, helpnum) ;

	return FALSE ;
}

BOOL CSpellingCheckDll::GetUserHelpString(int index, BSTR FAR* pData)
{
	if (IsLoadedDLL() == FALSE) return FALSE;

	if (pHncSPLGetUserHelpString)
		return (*pHncSPLGetUserHelpString)(index, pData) ;

	return FALSE;
}

int CSpellingCheckDll::GetFrequency(LPCWSTR keyword)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetFrequency)
		return (*pHncSPLGetFrequency)(_dllhandle, keyword) ;

	return FALSE ;
}

SPLERRHND CSpellingCheckDll::ExceptionCheck (LPCWSTR inputstr, UINT inputlen, UINT checkpos)
{
	SPLERRHND errorID ;

	if (IsLoadedDLL() == FALSE) return SPLHND_ERROR ;

	SetSoundOn(FALSE) ;
	if (pHncSPLExceptionCheck)
		errorID = (*pHncSPLExceptionCheck)(_dllhandle, inputstr, inputlen, checkpos) ;
	else errorID = SPLHND_ERROR ;

	SetSoundOn(TRUE) ;
	return errorID ;
}

void CSpellingCheckDll::SetSoundOn(BOOL TurnOn)
{
	if (IsLoadedDLL() == FALSE) return ;

	if (pHncSPLSetSoundOn)
		(*pHncSPLSetSoundOn)(_dllhandle, TurnOn) ;
}

BOOL CSpellingCheckDll::GetBaseStringExt(LPCWSTR keyword, LPWSTR BaseString, UINT max_len)
{
	if (IsLoadedDLL() == FALSE) return FALSE ;

	if (pHncSPLGetBaseStringExt)
		return (*pHncSPLGetBaseStringExt)(_dllhandle, keyword, BaseString, max_len) ;

	return FALSE ;
}
