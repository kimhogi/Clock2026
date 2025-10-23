/////////////////////////////////////////////////////////////////////////
//
//			HwpSpeller Dll Call Class
//			(주)한글과컴퓨터 기술연구소
/////////////////////////////////////////////////////////////////////////
#ifndef _SPELLINGCHECKDLL_H_
#define _SPELLINGCHECKDLL_H_

typedef unsigned int	SPLHND ;
typedef unsigned int	SPLERRHND ;
#define SPLHND_ERROR	0

// 맞춤법검사기 선택사항 define
enum HWPSPELLER_OPTION { //17개
	HWPSPLOPT_COMBINOUN=0,	HWPSPLOPT_AUXATTACH,	HWPSPLOPT_SPACECHAIN,	HWPSPLOPT_IHEECHECK,
    HWPSPLOPT_REDUNDANT,	HWPSPLOPT_ENGJONGCHK,	HWPSPLOPT_WARNBELL,		HWPSPLOPT_WARNCHK,
	HWPSPLOPT_HGCHECK,		HWPSPLOPT_HJCHECK,		HWPSPLOPT_ENGCHECK,		HWPSPLOPT_ENGABBRCHECK, 
	HWPSPLOPT_STYLECHECK,	HWPSPLOPT_STCHKLEVEL,	HWPSPLOPT_BUHOCHECK,	HWPSPLOPT_SUSACHECK, 
	HWPSPLOPT_JONBICHECK
};

class CSpellingCheckDll {
private :
	HMODULE _hInstance ;
	SPLHND _dllhandle ;

	void InitSpellerFunction() ;
	BOOL LinkSpellerFunction() ;
	
public :
	// construction
	CSpellingCheckDll() ;
	~CSpellingCheckDll() ;

	// run HwpSpeller.dll member function
	// Hwpspeller.Dll이 Load되어 있는지 Load할 수 있는지 검사.
	// Dll의 존재 유무에 따른 Message를 출력하기 위해서 public으로..
	BOOL IsLoadedDLL (void) ;
	// 맞춤법검사기 선택사항을 get/set
	BOOL GetSPLOption(int sploption, int *flag);
	BOOL SetSPLOption(int sploption, int flag);
	BOOL RunOptionDlg(HWND hWnd);
	void SetDefaultOption();

	SPLERRHND CheckSpell (LPCWSTR inputstr, UINT inputlen, LPINT chkpos, BOOL bNewPara, BOOL bChangedWord) ;
	int  Suggestion (SPLERRHND _errorhandle, LPCWSTR inputstr, UINT inputlen, LPWSTR repword, int repwordlen) ;

	void FreeErrorHandle (SPLERRHND _errorhandle) ;
	int  GetErrorDepthType(SPLERRHND _errorhandle, int idx, LPINT) ;
	void GetErrorPosLen (SPLERRHND _errorhandle, LPINT pos, LPINT len) ;
	int  GetErrorAttr (SPLERRHND _errorhandle, int idx, LPWSTR attrstr) ;
	BOOL GetHelpInfo(SPLERRHND errorhandle, UINT index, LPINT helpnum) ;

	
	int  GetAuxDicName (LPWSTR Auxdicnamebuf, int bufsize) ;
	int  GetAuxDicCount (void) ;
	BOOL IsAuxDicRunning (LPCWSTR auxtitle) ;
	BOOL AuxDicReady (LPCWSTR auxtitle) ;
	BOOL AuxDicClose (LPCWSTR auxtitle) ;

	// 사용자 사전 관련 함수
	int  UserdicSearch (LPCWSTR userword) ;
	int  UserdicAdd (LPCWSTR userword) ;
	BOOL UserdicAddable (LPCWSTR userword) ;
	int  UserdicItemCount (void) ;
	int  UserdicItem (int idx, LPWSTR userword, int bufferlen) ;
	BOOL UserdicDelete (LPCWSTR userword) ;
	BOOL UserdicLoad (LPCWSTR usrdicpath) ;
	BOOL UserdicSave (LPCWSTR usrdicpath) ;


	BOOL AddSkipWord(LPCWSTR skipword) ;
	void SetSoundOn(BOOL TurnOn) ;

	BOOL MakeAuxDic (LPVOID getfunc, LPCWSTR TitleName, int classtype, LPVOID config, int configsize) ;
	int  AuxdicDelete(LPCWSTR titlename) ;
	int  IsOKDeleteAuxdic(LPCWSTR titlename) ;

	int	 ScanStartFromAuxDic(LPCWSTR titlename) ;
	BOOL GetNextItemFromAuxDic(LPWSTR itemstr, int maxlen) ;

	BOOL ReadConfigAuxDic (LPCWSTR titlename, LPINT classtype, LPVOID pool, LPINT maxsize) ;
	BOOL WriteConfigAuxDic (LPCWSTR titlename, LPCWSTR newtitle, int classtype, LPVOID pool, int size) ;

	BOOL DeliverAuxDic(LPCWSTR filename, LPWSTR titlename) ;

	int  GetClassString(int index, LPWSTR getstr) ;
	
	// 맞춤법 도우미 관련 Function Call
	int	 StartCheck(LPCWSTR inputstr, UINT inputlen) ;
	void EndCheck(void) ;
	SPLERRHND GetNextErrorHandle(void) ;
	SPLERRHND ExceptionCheck (LPCWSTR inputstr, UINT inputlen, UINT checkpos) ;

	// 등록 기호 관련 Function Call
	BOOL GetSymbol(UINT index, LPWSTR symbol);
	int  GetSymbolCount(void);
	int  AddSymbol(LPCWSTR symbol);
	BOOL DelSymbol(UINT index);
	int  EditSymbol(UINT index, LPCWSTR symbol);
	int  SearchSymbol(LPCWSTR symbol);

	// 자동 수행 관련 Function Call
	BOOL AddAutoExecWord(LPCWSTR errword, LPCWSTR replaceword, BOOL IsAutoExec) ;
	BOOL IsAutoExecWord (void) ;

	int  EnumDicView(LPCWSTR keyword, LPCWSTR *DicViewBuffer) ;
	void FreeDicView(void) ;

	// Base String을 가져오는 함수
	int  GetBaseString(LPCWSTR keyword, LPVOID getfunc) ;
	BOOL GetBaseStringExt(LPCWSTR keyword, LPWSTR BaseString, UINT max_len) ;

	// 혼동되는 말 관련
	int  SearchWarnString(LPCWSTR searchword, LPWSTR stringbuf, int maxlen) ;
	int	 GetWarnString(int index, LPWSTR stringbuf, int maxlen) ;
	BOOL GetWarnHelp(LPCWSTR searchword, LPWSTR helpbuf, int maxlen) ;

	// 고빈도 사전 관련
	int  GetFrequency (LPCWSTR findstr) ;
	
	// 대치어에 대한 맞춤법 도움말 (사용자에 의해 커스터마이징된 도움말)
	// 리턴된 메모리는 함수를 호출한 호출가가 delete해야 한다.
	// index는 현재 대치되려는 단어의 인덱스
	BOOL GetUserHelpString(int index, BSTR FAR* pData);

	// 찾아바꾸기 관련 함수

	/* *********************************************************************
	용언 찾아바꾸기를 할 수 있는지를 검사한다.
	입력받은 스트링이 정확하게 기본꼴인지를 검사하고
	이 검사에서 TRUE일 때에만 GetBaseVerbList를 호출해야 한다.
	또한 이 검사에서 TRUE일 때만 GetChangedWord에서 IsChangeVerb의 값이 TRUE가 될수 있다.

	리턴값이 FALSE이면 입력받은 스트링이 용언이 아니라는 의미이다.
	이때에는 조사자동바꾸기가 있는 경우는 IsChangeVerb가 FALSE가 되어 조사 자동 바꾸기를 수행할 수 있다.

	리턴값이 -1이면 용언으로 분석되었지만 입력받은 스트링이 기본꼴이 아닌 경우,
	어떤 오류가 발생하였거나 한글이 아닌 경우등이다.
	이때에는 보통의 찾아바꾸기를 수행해야 한다.

	replacebaseword는 NULL이 될 수 있다. 그러면 findbaseword만 검사한다.
	==> 찾기에서 이용될 수 있다.
	********************************************************************* */
	int	IsEnableChangeVerb (LPCWSTR findbaseword, LPCWSTR replacebaseword) ;

	/* *********************************************************************
	입력받은 용언의 활용되는 어간List을 얻는다.
	IsEnableChangeVerb에서 TRUE일 때에만 호출되어야 한다.
	getfunc은 함수 포인터이다. 
	void getfunc(LPCWSTR instr) ;
	********************************************************************* */
	BOOL GetBaseVerbList (LPCWSTR findbaseword, LPVOID getfunc) ;

	/* *********************************************************************
	입력받은 말에 대해 조사나 용언 활용꼴을 얻는다.
	IsEnableChangeVerb에서 TRUE일 때만 IsChangeVerb의 값이 TRUE가 될 수 있다.

	조사 자동 교정 :
		IsChangeVerb가 FALSE
		return FALSE이면 조사가 바뀔 필요가 없다는 의미, 따라서 찾아진 말만 바꾸면 된다.
		return TRUE이면 findword을 replaceword로 바꾸어야 한다.
	용언 자동 교정 :
		IsChangeVerb가 TRUE
		return FALSE이면 findword가 용언이 아니거나 올바른 용언의 활용꼴이 아닌 경우, 
		그리고 용언의 활용꼴을 얻는데 실패한 경우
			따라서 이 경우에는 찾아진 말로 바뀌어서는 안된다.
		return TRUE이면 findword을 replaceword로 바꾸어야 한다.
	********************************************************************* */
	BOOL GetChangedWord (BOOL IsChangeVerb, LPCWSTR findbaseword, LPCWSTR findword, 
		LPCWSTR repbaseword, LPWSTR replaceword, int maxsize) ;
	
	/* *********************************************************************
	맞춤법 결과를 얻을 함수를 링크한다.
	********************************************************************* */
	LPVOID m_LPSuggestFunc ;
	int m_iSuggestCount ;

	/* *********************************************************************
	사용자 한자 사전 검색 루틴을 링크한다.
	********************************************************************* */
	LPVOID m_LPUserHanjaFunc ;
} ;

#endif