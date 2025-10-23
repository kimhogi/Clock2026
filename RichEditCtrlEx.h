#pragma once

// RichEditCtrlEx.h : header file
//

#include "ClockData.h"

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx window

class CRichEditCtrlEx : public CRichEditCtrl
{
	// Construction
public:
	CRichEditCtrlEx();

	// Attributes
public:
	BOOL m_bXMLEdit;
	INT  m_nDefaultImageSize;
	CCharInfoList m_CharInfoList;
	CStatus* m_pStatus;

	// Operations
//	CString m_strDefaultFontName;
//	ULONG   m_ulDefaultTextColor;

	CCharInfoList m_CharInfoListForClipBoard;


public :
	void SetStatus(CStatus* pStatus) { m_pStatus = pStatus; }
    void SetDefaultFontAndColor(CString strDefaultFontName,ULONG ulDefaultTextColor);
	void SetTextBackgroundColor(ULONG ulTextBackGroundColor);
	void SetSelectionFont(CString strFaceName);
	void SetSelectionColor(COLORREF crText);
	void SetSelectionSize(INT nSize);
	void SetDefaultFont(CString strFaceName);
	void SetDefaultColor(COLORREF crText);
	void SetDefaultSize(INT nSize);
	CCharInfoList GetCharInfoList(void);
	void SetCharInfoList(CCharInfoList CharInfoList);
	CString GetUnicodeString(void) ;
	void SetUnicodeString(CString str);

	CCharInfoList GetSelCharInfoList(void);
	CString GetFontAt(INT nIndex);
	CCharInfoList MakeCharInfoList(CString strData);
	int GetFileType(CString ext);	
	void InsertImgToRichEditCtrl(CString strFilename,INT nSize = 40);
	BOOL GetImageInfo(INT nPos,CCharInfo &CharInfo);
	CRect GetImageRect(CString strFilename);
	BOOL ExistImageInSelection(void);
	void ClearImageVector(void);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlEx)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CRichEditCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichEditCtrlEx)
	afx_msg	LRESULT OnPaste(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:	
};

