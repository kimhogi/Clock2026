// SpellCheck.cpp : implementation file
//

//#include <stdafx.h>
#include "pch.h"
//#include "Clock.h"
#include "SpellCheck.h"
#include "process.h"
#include <atlbase.h>
#include ".\spellcheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*  맞춤법 관련 부분 */
#define REPLACEWORD_MAXCOUNT 11
#define SPELL_REPLACE_MENU_BASE 0xC700

typedef struct {
	WCHAR suggestword[30] ;
	int   frequency ;
} AutoSuggestBufFormat ;


static	AutoSuggestBufFormat _repword_buffer[REPLACEWORD_MAXCOUNT] ;
static  UINT _current_repwordcount ;


static void linkfunc_suggestion(LPCWSTR repword)
{
	if (_current_repwordcount >= REPLACEWORD_MAXCOUNT) return ;
	if (wcslen(repword) >= 30) return ;

	wcscpy(_repword_buffer[_current_repwordcount].suggestword, repword) ;
	_repword_buffer[_current_repwordcount++].frequency = 0 ;
}



/////////////////////////////////////////////////////////////////////////////
// CSpellCheck dialog


CSpellCheck::CSpellCheck(CWnd* pParent /*=NULL*/)
	: CDialog(CSpellCheck::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpellCheck)
	//}}AFX_DATA_INIT
	
	m_nCount = 0;
	m_nFirst = 0;
	
	m_hThread  = 0 ;

}

void CSpellCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpellCheck)
	DDX_Control(pDX, IDC_RICHEDIT_CONTENT, m_richEditContent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpellCheck, CDialog)
	//{{AFX_MSG_MAP(CSpellCheck)
	ON_BN_CLICKED(IDC_BUTTON_SIGN, OnButtonSign)
	ON_EN_CHANGE(IDC_RICHEDIT_CONTENT, OnChangeRicheditContent)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpellCheck message handlers

BOOL CSpellCheck::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= ES_SELECTIONBAR;

	return CDialog::PreCreateWindow(cs);
}


void CSpellCheck::PreLoadDll()
{
	m_SpellCheck.m_LPSuggestFunc = (LPVOID)linkfunc_suggestion;
	m_SpellCheck.m_iSuggestCount = REPLACEWORD_MAXCOUNT;

	
	if ( !m_SpellCheck.IsLoadedDLL() )
	{
		MessageBox(_T("IsLoadDLL() Err"));
		exit(0);
	}
}


BOOL CSpellCheck::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 맞춤법 추천어 사용 및 맞춤법 dll 로드
	PreLoadDll();

	
	if(LoadLibrary(_T("Riched32.dll"))==NULL)
		return FALSE;

	// OnChangeRicheditContent 함수 작동 하게 ..
	m_richEditContent.SetEventMask(ENM_CHANGE) ;	

	
	// 본문 내용을 에디트 박스에 넣는다. 
	//m_strContents = m_pParent->m_strContent;
	SetDlgItemText( IDC_RICHEDIT_CONTENT, m_strContents) ;

	// 에러 체크, 표시 
	SpellCheckErrorSign() ;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSpellCheck::ErrorWordSetSel(int m_nFirst, int m_nCount, int flag /* = 0 */) 
{
	// 리치에디터에서 속성 바꾸기
	CHARFORMAT cf;			  
	cf.dwEffects = 0;  // or cf.dwEffects &= ~CFE_AUTOCOLOR;  기존의 잡혀 있던 속성 해제
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;	
	
	if ( m_nFirst == -1 && m_nCount == -1 )			// 초기화
	{
		cf.crTextColor = RGB(0, 0, 0); 

		// 리치에디터 박스에 블록 			
		m_richEditContent.SetSel(0, -1) ; // 범위 전체		
		m_richEditContent.SetSelectionCharFormat(cf);	
	}
	else											// 블럭 적용
	{
		cf.crTextColor = RGB(255, 0, 0); 
					
		// 리치에디터 박스에 블록 			
		m_richEditContent.SetSel(m_nFirst, m_nFirst+m_nCount) ;				
		m_richEditContent.SetSelectionCharFormat(cf);	
	}
	
	
	return ;
}


void CSpellCheck::OnButtonSign() 
{
	// TODO: Add your control notification handler code here
	// 현재 리치에디트 박스에 있는 것을 가지고 와서 처리.
	GetDlgItemText(IDC_RICHEDIT_CONTENT, m_strContents) ;
	ErrorWordSetSel(-1, -1);

	SpellCheckErrorSign() ;
	
	return ;
}
// 에러 전체 표시
void CSpellCheck::SpellCheckErrorSign() 
{	
	USES_CONVERSION;

	WCHAR		repword[30] = {0,} ;
	int			result_count = 0 ; 
	int			error_pos = 0 ;
	int			nNextFlag = 0 ;
	SPLERRHND	errorID ;


	CString strTemp ;
	int count = 1; 


	//if ( (result_count = m_SpellCheck.StartCheck( T2W( m_strContents), m_strContents.GetLength())) > 0 ) 	
	if ( (result_count = m_SpellCheck.StartCheck( (LPCWSTR)( m_strContents), m_strContents.GetLength())) > 0 ) 	
	{	
		while ( result_count-- ) 
		{		
			if ((errorID = m_SpellCheck.GetNextErrorHandle()) == SPLHND_ERROR) 
				break ;

			m_SpellCheck.GetErrorPosLen(errorID, &m_nFirst, &m_nCount) ; 
						
			m_strCurErrWord = m_strContents.Mid( m_nFirst, m_nCount ) ;

			if ( !m_strCurErrWord.IsEmpty())
				strTemp.Format( _T("[%d: %s] : "), count++, m_strCurErrWord) ;

			m_strSuggestList = m_strSuggestList + strTemp ;
			
			// 에디터 박스에 블록 
			ErrorWordSetSel(m_nFirst, m_nCount) ;	

			// 추천어 
			SpellCheckSuggest(); 			
		}  
		
	}  
	SetDlgItemText(IDC_EDIT_LISTSUGGEST, m_strSuggestList ) ;

	m_strSuggestList.Empty() ;
		
	m_SpellCheck.EndCheck();	

	return ;
}


void CSpellCheck::OnChangeRicheditContent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here	
	
	GetDlgItemText(IDC_RICHEDIT_CONTENT, m_strContents) ;

	return ;
}


// 추천어를 찾는 함수 
void CSpellCheck::SpellCheckSuggest()
{
	USES_CONVERSION;

	if ( m_strCurErrWord.IsEmpty())
		return ;
	
	{ 
		WCHAR		repword[30] = {0,} ;
		int errorpos = 0, errorlen = 0, pos=0;
		SPLERRHND errorID;

		memset(_repword_buffer, 0, REPLACEWORD_MAXCOUNT * sizeof(AutoSuggestBufFormat));
		_current_repwordcount = 0;

		//errorID = m_SpellCheck.CheckSpell( T2W(m_strCurErrWord), m_strCurErrWord.GetLength(), &pos, FALSE, FALSE) ;
		errorID = m_SpellCheck.CheckSpell( (LPCWSTR)(m_strCurErrWord), m_strCurErrWord.GetLength(), &pos, FALSE, FALSE) ;
		
		m_SpellCheck.GetErrorPosLen(errorID, &errorpos, &errorlen) ;
		//int nCount = m_SpellCheck.Suggestion(errorID, T2W(m_strCurErrWord), m_strCurErrWord.GetLength(), repword, 29) ;	
		int nCount = m_SpellCheck.Suggestion(errorID, (LPCWSTR)(m_strCurErrWord), m_strCurErrWord.GetLength(), repword, 29) ;	
	
		if (nCount <= 0) 
			m_SpellCheck.FreeErrorHandle(errorID) ;

		
		CString strlist ; 
		for ( int j = 0 ; j < nCount ; j++)	
			strlist = strlist + _T(" ") + W2T(_repword_buffer[j].suggestword) ;
			
		m_strSuggestList = m_strSuggestList + strlist + _T("\r\n");	
	}

	return ; 

}


void CSpellCheck::OnOK() 
{
	// TODO: Add extra validation here
	m_strContents;
	CDialog::OnOK();
}

void CSpellCheck::OnCancel() 
{
	// TODO: Add extra cleanup here	
	CDialog::OnCancel();
}