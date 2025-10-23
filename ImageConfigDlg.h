#pragma once
#include "ClockData.h"

// CImageConfigDlg 대화 상자입니다.

class CImageConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageConfigDlg)

public:
	CImageConfigDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImageConfigDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CONFIG_IMAGE };

public:
	CStatus* m_pStatus;
	CImageButton m_btnImage[IMAGE_SHORTCUT_NUM];


public:
	void SetImageShortCut(UINT nImageNumber);
	void SelectImage(UINT nImageNumber);
	BOOL UpdateData(BOOL bSaveAndValidate /* = TRUE */);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButtonSelectImage1();
	afx_msg void OnBnClickedButtonSelectImage2();
	afx_msg void OnBnClickedButtonSelectImage3();
	afx_msg void OnBnClickedButtonSelectImage4();
	afx_msg void OnBnClickedButtonSelectImage5();
	afx_msg void OnBnClickedButtonSelectImage6();
};
