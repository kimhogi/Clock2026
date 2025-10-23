// ImageConfigDlg.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "Clock2023.h"
#include "ImageConfigDlg.h"
#include ".\imageconfigdlg.h"


// CImageConfigDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CImageConfigDlg, CDialog)
CImageConfigDlg::CImageConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageConfigDlg::IDD, pParent)
{
	m_pStatus = theApp.GetStatus();
}

CImageConfigDlg::~CImageConfigDlg()
{
}

void CImageConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	CString strVal;

	strVal = m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_1);
	DDX_Text(pDX,IDC_STATIC_IMAGE_1,strVal);
	strVal = m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_2);
	DDX_Text(pDX,IDC_STATIC_IMAGE_2, strVal);
	strVal = m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_3);
	DDX_Text(pDX,IDC_STATIC_IMAGE_3, strVal);
	strVal = m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_4);
	DDX_Text(pDX,IDC_STATIC_IMAGE_4, strVal);
	strVal = m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_5);
	DDX_Text(pDX,IDC_STATIC_IMAGE_5, strVal);
	strVal = m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_6);
	DDX_Text(pDX,IDC_STATIC_IMAGE_6, strVal);

	DDX_Control(pDX,IDC_BUTTON_SELECT_IMAGE_1,m_btnImage[IMAGE_SHORTCUT_1]);
	DDX_Control(pDX,IDC_BUTTON_SELECT_IMAGE_2,m_btnImage[IMAGE_SHORTCUT_2]);
	DDX_Control(pDX,IDC_BUTTON_SELECT_IMAGE_3,m_btnImage[IMAGE_SHORTCUT_3]);
	DDX_Control(pDX,IDC_BUTTON_SELECT_IMAGE_4,m_btnImage[IMAGE_SHORTCUT_4]);
	DDX_Control(pDX,IDC_BUTTON_SELECT_IMAGE_5,m_btnImage[IMAGE_SHORTCUT_5]);
	DDX_Control(pDX,IDC_BUTTON_SELECT_IMAGE_6,m_btnImage[IMAGE_SHORTCUT_6]);
}


BEGIN_MESSAGE_MAP(CImageConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE_1, OnBnClickedButtonSelectImage1)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE_2, OnBnClickedButtonSelectImage2)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE_3, OnBnClickedButtonSelectImage3)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE_4, OnBnClickedButtonSelectImage4)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE_5, OnBnClickedButtonSelectImage5)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGE_6, OnBnClickedButtonSelectImage6)
END_MESSAGE_MAP()


// CImageConfigDlg 메시지 처리기입니다.
BOOL CImageConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//for(int i=IMAGE_SHORTCUT_1;i<IMAGE_SHORTCUT_NUM;i++)
	//{
	//	m_btnImage[i].SetFlat(FALSE);
	//	m_btnImage[i].SetColor(RGB(0x00,0x00,0x00));
	//}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CImageConfigDlg::UpdateData(BOOL bSaveAndValidate /* = TRUE */)
{
	if(!bSaveAndValidate)
	{
       for(int i=IMAGE_SHORTCUT_1;i<IMAGE_SHORTCUT_NUM;i++)
	   {
		   m_btnImage[i].SetImageFileName(m_pStatus->NewsConf()->ImageShortCut(i));
	   }
	}

	return CDialog::UpdateData(bSaveAndValidate);
}

void CImageConfigDlg::SetImageShortCut(UINT nImageNumber)
{
	TCHAR szCurrentDirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szCurrentDirectory);

	CImageImportDlg dlg(TRUE,FALSE,_T("PNG"),m_pStatus->NewsConf()->ImageShortCut(nImageNumber));

	dlg.m_ofn.lpstrFilter = IMAGE_FILTER;

	CPXFilePath pathImage(BLANK_STRING);
	CString strFolder = pathImage.GetFolder();
	dlg.m_ofn.lpstrInitialDir = strFolder;
	_tcscpy(dlg.m_ofn.lpstrFile, pathImage.GetFileName());

	if (dlg.DoModal() == IDOK)
	{
		m_pStatus->NewsConf()->ImageShortCut(nImageNumber,dlg.GetPathName());
	}

	UpdateData(FALSE);

	::SetCurrentDirectory(szCurrentDirectory);

}

void CImageConfigDlg::SelectImage(UINT nImageNumber)
{
	if (IsCTRLpressed())
	{
		m_pStatus->NewsConf()->ImageShortCut(nImageNumber, BLANK_STRING);
		UpdateData(FALSE);
	}
	else
	{
		SetImageShortCut(nImageNumber);
	}
}

void CImageConfigDlg::OnBnClickedButtonSelectImage1()
{
	SelectImage(IMAGE_SHORTCUT_1);
}

void CImageConfigDlg::OnBnClickedButtonSelectImage2()
{
	SelectImage(IMAGE_SHORTCUT_2);
}

void CImageConfigDlg::OnBnClickedButtonSelectImage3()
{
	SelectImage(IMAGE_SHORTCUT_3);
}

void CImageConfigDlg::OnBnClickedButtonSelectImage4()
{
	SelectImage(IMAGE_SHORTCUT_4);
}

void CImageConfigDlg::OnBnClickedButtonSelectImage5()
{
	SelectImage(IMAGE_SHORTCUT_5);
}

void CImageConfigDlg::OnBnClickedButtonSelectImage6()
{
	SelectImage(IMAGE_SHORTCUT_6);
}
