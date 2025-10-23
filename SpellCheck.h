#if !defined(AFX_SPELLCHECK_H__6093446A_38EA_4191_A594_42B0202715B0__INCLUDED_)
#define AFX_SPELLCHECK_H__6093446A_38EA_4191_A594_42B0202715B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpellCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpellCheck dialog

#include "SpellingCheckDLL.h"


class CLnsLocalDlg ;


class CSpellCheck : public CDialog
{
// Construction
public:
	CSpellCheck(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CSpellCheck)
	enum { IDD = IDD_DIALOG_SPELLCHECK };
	CButton	m_ctrCheck;
	CListBox		m_ctrSuggest;
	CRichEditCtrl	m_richEditContent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpellCheck)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	enum { PASS = 1, CHG, ALLCHG, RESTART, FCLOSE } ; 

	CSpellingCheckDll	m_SpellCheck;
	CLnsLocalDlg		*m_pParent ;
	
	CString				m_strSuggestList;	//IDC_EDIT_LISTSUGGEST
	CString				m_strContents ;		// 검사할 객체 내용
	CString				m_strCurErrWord ;	// 에러 어절	
	int					m_nCount ;			// 오류어 길이 ( 한글 한글자 == 1 ) 
	int					m_nFirst ;			// 오류어 시작점
	HANDLE				m_hThread;
	
		
	
	void PreLoadDll();						// 맞춤법 DLL 로드
	void SpellCheckErrorSign() ;			// 에러 전체 표시
	void SpellCheckSuggest();				// 추천어 
	void ErrorWordSetSel(int m_nFirst, int m_nCount,  int flag = 0) ;	// 오류어에 블록 지정
 
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpellCheck)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSign();
	afx_msg void OnChangeRicheditContent();
	virtual void OnOK();
	virtual void OnCancel();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPELLCHECK_H__6093446A_38EA_4191_A594_42B0202715B0__INCLUDED_)
