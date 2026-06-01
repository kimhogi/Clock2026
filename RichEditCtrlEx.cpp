// RichEditCtrlEx.cpp : implementation file
//

#include "pch.h"
#include "RichEditCtrlEx.h"
#include ".\richeditctrlex.h"
#include <RichOle.h>
#include "ImageDataObject.h"
#include "ClockData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx

CRichEditCtrlEx::CRichEditCtrlEx()
{
	m_bXMLEdit = FALSE;
}

CRichEditCtrlEx::~CRichEditCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrlEx, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlEx)
	ON_MESSAGE(WM_PASTE, OnPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx message handlers

LRESULT CRichEditCtrlEx::OnPaste(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	return 0;
}

BOOL CRichEditCtrlEx::PreTranslateMessage(MSG* pMsg) 
{
	LONG nStartIndex = 0;
	LONG nEndIndex = 0;
	CCharInfoList charInfoList_Header;
	CCharInfoList charInfoList_Tail;
	CCharInfoList charInfoList_Buffer;
	LONG nPos = 0;


	if( pMsg->message == WM_KEYDOWN )
	{
	//	if (m_bXMLEdit )
		{
			UINT nKeyCode = static_cast<UINT>(pMsg->wParam);	// virtual key code of the key pressed
			if ((nKeyCode == _T('V')) && (::GetKeyState(VK_CONTROL) & 0x8000) )
			{			

				if(OpenClipboard())
				{
					char* buffer = nullptr;

					buffer = (char*)GetClipboardData(CF_RICHEDIT);

					if(buffer == nullptr)
					{
						buffer = (char*)GetClipboardData(CF_TEXT); 

						if(buffer != nullptr)
						{

							CString strData = CString(buffer);
							m_CharInfoListForClipBoard = MakeCharInfoList(strData);
						}
					}
					CloseClipboard();
				}

				GetSel(nStartIndex,nEndIndex);  // End Index는 끝에서 +1된 값을 리턴한다.

				GetCharInfoList();

#ifdef _DEBUG
				m_CharInfoList.TraceData(_T("CharInfoList"));
#endif

				m_CharInfoList.Mid(0,nStartIndex, charInfoList_Header);

#ifdef _DEBUG
				charInfoList_Header.TraceData(_T("CharInfoListHeader"));
#endif

			    m_CharInfoList.Mid(nEndIndex,-1, charInfoList_Tail);

#ifdef _DEBUG
				charInfoList_Tail.TraceData(_T("CharInfoListTail"));
#endif

				m_CharInfoList.Remove(nStartIndex,nEndIndex-nStartIndex);

#ifdef _DEBUG
				m_CharInfoList.TraceData(_T("CharInfoList"));
#endif

				m_CharInfoList = charInfoList_Header + m_CharInfoListForClipBoard;
				nPos = (LONG)(m_CharInfoList.Size());

#ifdef _DEBUG
				m_CharInfoList.TraceData(_T("___________________________CharInfoList_______________"));
#endif

				charInfoList_Buffer = m_CharInfoList + charInfoList_Tail;
		
#ifdef _DEBUG
				charInfoList_Buffer.TraceData(_T("___________________________ charInfoList_Buffer _______________"));
#endif
			    SetCharInfoList(charInfoList_Buffer);

				SetSel(nPos, nPos);

				return TRUE;
			}

			if ((nKeyCode == _T('C')) && (::GetKeyState(VK_CONTROL) & 0x8000) )
			{			

				GetSel(nStartIndex,nEndIndex);

				GetCharInfoList();

				m_CharInfoList.Mid(nStartIndex,nEndIndex, m_CharInfoListForClipBoard); //GetSelCharInfoList();
#ifdef _DEBUG
				m_CharInfoListForClipBoard.TraceData(_T("ClipBoard"));

#endif
				SetSel(nStartIndex,nEndIndex);

				if(OpenClipboard())
				{
					HGLOBAL clipbuffer;
					EmptyClipboard();

					CString strClipData = RICHEDIT_CLIPBOARD_DATA;
					
					WCHAR *buffer;

					INT nSize = (strClipData.GetLength()+1)*sizeof(WCHAR);

					clipbuffer = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE,nSize); // Unicode이므로 2를 곱해본다

					buffer = (WCHAR *)GlobalLock(clipbuffer);

					::ZeroMemory(buffer,nSize);

					wcscpy(buffer,strClipData.GetBuffer());

					strClipData.ReleaseBuffer();			
                    
					GlobalUnlock(clipbuffer);
					SetClipboardData(CF_RICHEDIT,clipbuffer);
					CloseClipboard();
				}

				return TRUE;
			}

			if ((nKeyCode == _T('X') ) && 	(::GetKeyState(VK_CONTROL) & 0x8000) )
			{			
				
				GetSel(nStartIndex,nEndIndex);

				GetCharInfoList();

				m_CharInfoList.Mid(nStartIndex,nEndIndex, m_CharInfoListForClipBoard); //GetSelCharInfoList();

				m_CharInfoList.Remove(nStartIndex,nEndIndex-(nStartIndex+1));
#ifdef _DEBUG
				m_CharInfoList.TraceData(_T("CharInfoList After Remove "));
				m_CharInfoListForClipBoard.TraceData(_T("ClipBoard"));
#endif

				SetCharInfoList(m_CharInfoList);

				SetSel(nStartIndex,nStartIndex);

				return TRUE;
			}

			if ((nKeyCode == _T('A') /*|| nKeyCode == _T('X') || nKeyCode == _T('V')*/) && 
				(::GetKeyState(VK_CONTROL) & 0x8000) )
			{			
				// GetParent()->SendMessage(WM_PASTE);

			//	GetSel(0,-1);

				GetCharInfoList();

				m_CharInfoList.Mid(0,-1, m_CharInfoListForClipBoard); //GetSelCharInfoList();
#ifdef _DEBUG
				m_CharInfoListForClipBoard.TraceData(_T("ClipBoard"));

#endif
				SetSel(0,-1);

				return TRUE;
			}

		}				
	
	}		
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CRichEditCtrlEx::SetDefaultFontAndColor(CString strDefaultFontName,ULONG ulDefaultTextColor)
{ 
//	m_strDefaultFontName = strDefaultFontName; 
//	m_ulDefaultTextColor = ulDefaultTextColor; 
	SetDefaultFont(m_pStatus->NewsConf()->DefaultFontName());
//	SetDefaultFont(m_strDefaultFontName);
	SetDefaultColor(ULONGToRGB(m_pStatus->NewsConf()->TextColor()));
	SetSelectionColor(ULONGToRGB(m_pStatus->NewsConf()->TextColor()));
}

void CRichEditCtrlEx::SetTextBackgroundColor(ULONG ulTextBackGroundColor)
{
	// CHARFORMAT2 구조체 초기화
	CHARFORMAT2 cf;
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_BACKCOLOR; // 배경색을 설정할 수 있도록 마스크 설정
	cf.crBackColor = ULONGToRGB(ulTextBackGroundColor); // 원하는 배경색 지정

	// 선택한 텍스트에 스타일을 적용
	SetSelectionCharFormat(cf);
}

void CRichEditCtrlEx::SetSelectionFont(CString strFaceName)
{
	CHARFORMAT CharFormat;

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |= CFM_FACE | CFM_CHARSET;
	CharFormat.dwMask &=~CFM_PROTECTED;

	GetSelectionCharFormat(CharFormat);

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |= CFM_FACE | CFM_CHARSET;
	CharFormat.dwMask &=~CFM_PROTECTED;

//	if(strFaceName == m_pStatus->m_strSymbolFontName)
//	{		
//	CharFormat.bCharSet = HANGUL_CHARSET;
	//}

	_tcscpy(CharFormat.szFaceName,strFaceName.GetBuffer());

	SetSelectionCharFormat(CharFormat);

//	GetParent()->SendMessage(UWM_ITEM_CHANGE,0,0);
}
void CRichEditCtrlEx::SetSelectionColor(COLORREF crText)
{
	CHARFORMAT CharFormat;

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_COLOR;
	CharFormat.dwMask &=~CFM_PROTECTED;
	CharFormat.dwEffects &=~CFE_AUTOCOLOR;
	CharFormat.dwEffects &=~CFE_PROTECTED;

	GetSelectionCharFormat(CharFormat);

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_COLOR;
	CharFormat.dwMask &=~CFM_PROTECTED;
	CharFormat.dwEffects &=~CFE_AUTOCOLOR;
	CharFormat.dwEffects &=~CFE_PROTECTED;
	CharFormat.crTextColor = crText;

	SetSelectionCharFormat (CharFormat);
//	GetParent()->SendMessage(UWM_ITEM_CHANGE,0,0);

}

void CRichEditCtrlEx::SetSelectionSize(INT nSize)
{
	CHARFORMAT CharFormat;
	CCharInfo CharInfo;

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_SIZE;
	CharFormat.dwMask &=~CFM_PROTECTED;

	GetSelectionCharFormat(CharFormat);

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_SIZE;
	CharFormat.dwMask &=~CFM_PROTECTED;

	CharFormat.yHeight = nSize * DEFAULT_FONT_SIZE;

	SetSelectionCharFormat(CharFormat);

}

void CRichEditCtrlEx::SetDefaultFont(CString strFaceName)
{
	CHARFORMAT CharFormat;

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |= CFM_FACE | CFM_CHARSET ;
	CharFormat.dwMask &=~CFM_PROTECTED;

	GetSelectionCharFormat(CharFormat);

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |= CFM_FACE  | CFM_CHARSET ;
	CharFormat.dwMask &=~CFM_PROTECTED;

	CharFormat.bCharSet = HANGUL_CHARSET;

	_tcscpy(CharFormat.szFaceName,strFaceName.GetBuffer());

	SetDefaultCharFormat(CharFormat);
//	GetParent()->SendMessage(UWM_ITEM_CHANGE,0,0);
}

void CRichEditCtrlEx::SetDefaultColor(COLORREF crText)
{
	CHARFORMAT CharFormat;

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_COLOR;
	CharFormat.dwMask &=~CFM_PROTECTED;
	CharFormat.dwEffects &=~CFE_AUTOCOLOR;
	CharFormat.dwEffects &=~CFE_AUTOBACKCOLOR;
	CharFormat.dwEffects &=~CFE_PROTECTED;

	GetDefaultCharFormat(CharFormat);

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_COLOR;
	CharFormat.dwMask &=~CFM_PROTECTED;
	CharFormat.dwEffects &=~CFE_AUTOCOLOR;
	CharFormat.dwEffects &=~CFE_AUTOBACKCOLOR;
	CharFormat.dwEffects &=~CFE_PROTECTED;
	CharFormat.crTextColor = crText;

	SetDefaultCharFormat(CharFormat);
//	GetParent()->SendMessage(UWM_ITEM_CHANGE,0,0);
}

void CRichEditCtrlEx::SetDefaultSize(INT nSize)
{
	CHARFORMAT CharFormat;

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_SIZE;
	CharFormat.dwMask &=~CFM_PROTECTED;

	GetDefaultCharFormat(CharFormat);

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_SIZE;
	CharFormat.dwMask &=~CFM_PROTECTED;
	CharFormat.yHeight = nSize;

	SetDefaultCharFormat(CharFormat);
}

CString CRichEditCtrlEx::GetUnicodeString(void) 
{ 
	GETTEXTLENGTHEX getTextLengthEx;
	getTextLengthEx.codepage=1200;
	getTextLengthEx.flags=GTL_DEFAULT;

	int nLength = static_cast<int>(this->SendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&getTextLengthEx, (LPARAM)0)); //edit->GetTextLengthEx(GTL_DEFAULT,1200);
	LPWSTR lpszWChar = new WCHAR[nLength+1];

	GETTEXTEX getTextEx;
	getTextEx.cb=(nLength+1)*sizeof(WCHAR);
	getTextEx.codepage=1200;
	getTextEx.flags=GT_DEFAULT;
	getTextEx.lpDefaultChar=nullptr;
	getTextEx.lpUsedDefChar=nullptr;

	this->SendMessage(EM_GETTEXTEX, (WPARAM)&getTextEx, (LPARAM)lpszWChar); 

	CString strReturn = CString(lpszWChar);

	delete [] lpszWChar;

	return strReturn;
}
void CRichEditCtrlEx::SetUnicodeString(CString str)
{
	SETTEXTEX setTextEx;
	setTextEx.codepage=1200;
	setTextEx.flags=ST_DEFAULT;

	this->SendMessage(EM_SETTEXTEX, (WPARAM)&setTextEx, (LPARAM)str.GetBuffer());
}

BOOL CRichEditCtrlEx::GetImageInfo(INT nPos,CCharInfo &CharInfo)
{
	INT nCount = this->GetIRichEditOle()->GetObjectCount();
	REOBJECT rObject;
	HRESULT hr;
	CHARFORMAT CharFormat;

	SetSel(nPos,nPos+1);

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |=CFM_SIZE;
	CharFormat.dwMask &=~CFM_PROTECTED;

	GetSelectionCharFormat(CharFormat);

	for(int i=0;i<nCount;i++)
	{
		::ZeroMemory(&rObject, sizeof(REOBJECT));
		rObject.cbStruct = sizeof(REOBJECT);
		rObject.cp = i;

		hr = this->GetIRichEditOle()->GetObject(i,&rObject,REO_GETOBJ_ALL_INTERFACES);

		if(SUCCEEDED(hr))
		{
			if(rObject.cp == nPos) // nPos번째 아이템인지 확인
			{
				for( int j=0; j < CImageDataObject::m_vecStoragePath.size(); ++j)
				{
					if( rObject.pstg == CImageDataObject::m_vecStoragePath[j].pStorage)
					{
						CharInfo.Char(CImageDataObject::m_vecStoragePath[j].strPath);  // 이미지 경로 이름을 가져온다
						rObject.poleobj->Release();
						rObject.polesite->Release();
						rObject.pstg->Release();

						return TRUE;
					}
				}			
			}
		}
	}

	return FALSE;

}

BOOL CRichEditCtrlEx::ExistImageInSelection(void)
{
	CCharInfo CharInfo;
	CHARFORMAT CharFormat;
	long nStart = -1;
	long nEnd = -1;
	CString strFaceImage = IMAGE_FILE_TAG;

	GetSel(nStart,nEnd);

	for(long i=nStart;i<=nEnd;i++)
	{
		SetSel(i,i+1);

		CharFormat.cbSize = sizeof(CHARFORMAT);
		CharFormat.dwMask |= CFM_FACE;
		CharFormat.dwMask &=~CFM_PROTECTED;
		CharFormat.dwEffects &=~CFE_PROTECTED;

		GetSelectionCharFormat(CharFormat);

		if(strFaceImage.Compare(CharFormat.szFaceName) == 0)
		{
			SetSel(nStart,nEnd);
			return TRUE;
		}
	}

	SetSel(nStart,nEnd);
	return FALSE;		
}



CCharInfoList CRichEditCtrlEx::GetSelCharInfoList()
{
	CCharInfoList charInfoList;
	CString strBuffer;
	LONG nStartIndex = 0;
	LONG nEndIndex =0;

	GetSel(nStartIndex,nEndIndex);
	strBuffer = GetSelText();

	charInfoList.Clear();
	CCharInfo CharInfo;
	CHARFORMAT CharFormat;

	for(int i=nStartIndex;i<nEndIndex;i++)
	{
		CharInfo.Clear();

		SetSel(i,i+1); // +1에서 정확한 자기위치의 데이터를 읽어온다.

		CharFormat.cbSize = sizeof(CHARFORMAT);
		CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE | CFM_CHARSET;
		CharFormat.dwMask &=~CFM_PROTECTED;
		CharFormat.dwEffects &=~CFE_AUTOCOLOR;
		CharFormat.dwEffects &=~CFE_AUTOBACKCOLOR;
		CharFormat.dwEffects &=~CFE_PROTECTED;

		GetSelectionCharFormat(CharFormat);

		CharInfo.Char(strBuffer.Mid(i-nStartIndex,1));

		if(CharInfo.Char() == SPACE_STRING)   // 이미지는 스페이스로 처리되므로 스페이스가 이미지인지 아닌지 판단한다.
		{
			if(GetImageInfo(i,CharInfo))
			{	
				CharInfo.Font(IMAGE_FILE_TAG);
			}
			else
			{
				CharInfo.Font(CString(CharFormat.szFaceName));

				//if(CharInfo.m_strFaceName == m_strDefaultFontName)
				//{
				//	CharInfo.m_strFaceName = XML_TAG_DEFAULT_FONT;
				//}
			}
		}
		else
		{
			CharInfo.Font(CString(CharFormat.szFaceName));

			//if(CharInfo.m_strFaceName == m_strDefaultFontName)
			//{
			//	CharInfo.m_strFaceName = XML_TAG_DEFAULT_FONT;
			//}
		}

		CharInfo.Size(CharFormat.yHeight / DEFAULT_FONT_SIZE);

		CharInfo.Color(RGBToULONG(CharFormat.crTextColor));

		CharInfo.CharSet(CharFormat.bCharSet);

		charInfoList.CharInfoList().emplace_back(CharInfo);
	}

	return charInfoList;
}

CString CRichEditCtrlEx::GetFontAt(INT nIndex)
{
	CHARFORMAT CharFormat;

	SetSel(nIndex - 1, nIndex); // +1에서 정확한 자기위치의 데이터를 읽어온다.

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE | CFM_CHARSET;
	CharFormat.dwMask &= ~CFM_PROTECTED;
	CharFormat.dwEffects &= ~CFE_AUTOCOLOR;
	CharFormat.dwEffects &= ~CFE_AUTOBACKCOLOR;
	CharFormat.dwEffects &= ~CFE_PROTECTED;

	GetSelectionCharFormat(CharFormat);

	return CString(CharFormat.szFaceName);
}

CCharInfoList CRichEditCtrlEx::MakeCharInfoList(CString strData)
{
	CCharInfoList charInfoList;
	CString strBuffer;
	LONG nStartIndex = 0;
	LONG nEndIndex =0;
	charInfoList.Clear();
	CCharInfo CharInfo;


	CHARFORMAT CharFormat;

	GetSel(nStartIndex,nEndIndex);

	SetSel(nEndIndex-1,nEndIndex); // +1에서 정확한 자기위치의 데이터를 읽어온다.

	CString strLastFaceName = m_pStatus->NewsConf()->DefaultFontName(); //m_strDefaultFontName;

	for (int i = nEndIndex; i> 0;i--)
	{
	//	strLastFaceName = GetFontAt(i);

		if (strLastFaceName == IMAGE_FILE_TAG)
		{
			strLastFaceName = m_pStatus->NewsConf()->DefaultFontName();;
		}
		else
		{
			break;
		}
	}

	CharFormat.cbSize = sizeof(CHARFORMAT);
	CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE | CFM_CHARSET;
	CharFormat.dwMask &=~CFM_PROTECTED;
	CharFormat.dwEffects &=~CFE_AUTOCOLOR;
	CharFormat.dwEffects &=~CFE_AUTOBACKCOLOR;
	CharFormat.dwEffects &=~CFE_PROTECTED;

	GetSelectionCharFormat(CharFormat);


	for(int i=0;i<strData.GetLength();i++)
	{
		CharInfo.Clear();

		CharInfo.Char(strData.Mid(i,1));
	//	CharInfo.Font(CString(CharFormat.szFaceName));
		CharInfo.Font(strLastFaceName);
	    CharInfo.Size(CharFormat.yHeight / DEFAULT_FONT_SIZE);

		CharInfo.Color(RGBToULONG(CharFormat.crTextColor));

		CharInfo.CharSet(CharFormat.bCharSet);

		charInfoList.CharInfoList().emplace_back(CharInfo);
	}

	SetSel(nStartIndex,nEndIndex);

	return charInfoList;
}

CCharInfoList CRichEditCtrlEx::GetCharInfoList()
{
	LONG nStartIndex =0;
	LONG nEndIndex =0;

	GetSel(nStartIndex,nEndIndex);

	CString strBuffer;
	SetSel(0,-1);
	strBuffer = GetSelText();

	m_CharInfoList.Clear();
	CCharInfo CharInfo;
	CHARFORMAT CharFormat;

	INT nCount = strBuffer.GetLength();

	for(int i=0;i<nCount;i++)
	{
		CharInfo.Clear();

		SetSel(i,i+1); // +1에서 정확한 자기위치의 데이터를 읽어온다.
 
		CharFormat.cbSize = sizeof(CHARFORMAT);
		CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE | CFM_CHARSET;
		CharFormat.dwMask &=~CFM_PROTECTED;
		CharFormat.dwEffects &=~CFE_AUTOCOLOR;
		CharFormat.dwEffects &=~CFE_AUTOBACKCOLOR;
		CharFormat.dwEffects &=~CFE_PROTECTED;

		GetSelectionCharFormat(CharFormat);

		CharInfo.Char(strBuffer.Mid(i,1));

		if(CharInfo.Char() == SPACE_STRING || CharInfo.Char() == CString(_T("￼")))   // 이미지는 스페이스로 처리되므로 스페이스가 이미지인지 아닌지 판단한다.
		{

			if(GetImageInfo(i,CharInfo))
			{	
				CharInfo.Font(IMAGE_FILE_TAG);
			}
			else
			{
				CharInfo.Font(CString(CharFormat.szFaceName));
			}
		}
		else
		{
			CharInfo.Font(CString(CharFormat.szFaceName));
		}

		CharInfo.Size(CharFormat.yHeight / DEFAULT_FONT_SIZE);

		CharInfo.Color(RGBToULONG(CharFormat.crTextColor));

	
		CharInfo.CharSet(CharFormat.bCharSet);

		m_CharInfoList.CharInfoList().emplace_back(CharInfo);
	}

	SetSel(nStartIndex,nEndIndex);

	m_CharInfoList.CalcMixed();

	return m_CharInfoList;
}

void CRichEditCtrlEx::SetCharInfoList(CCharInfoList CharInfoList)
{
	LONG nStartIndex = 0;
	LONG nEndIndex = 0;

	GetSel(nStartIndex, nEndIndex);

	CHARFORMAT CharFormat;
	CCharInfo CharInfo;
	CString strTemp;

	//SetSel(0,-1);                 // 먼저 이전데이터를 지우고 시작한다.
	//ReplaceSel(BLANK_STRING);
	this->SetWindowText(BLANK_STRING);

	INT nSize = CharInfoList.Size();

	for (INT i = 0; i < nSize; i++)
	{
		CharInfo = CharInfoList.GetAt(i);

		SetSel(i, i + 1);

		if (CharInfo.Font() == IMAGE_FILE_TAG)
		{
			InsertImgToRichEditCtrl(CharInfo.Char(), CharInfo.Size()); // 리치에디터로 들어갈때는 DEFAULT_IMAGE_SIZE를 곱해줘야한다.
		}
		else
		{
			ReplaceSel(CharInfo.Char());
		}

		CharFormat.cbSize = sizeof(CHARFORMAT);
		CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE | CFM_CHARSET;
		CharFormat.dwMask &= ~CFM_PROTECTED;
		CharFormat.dwEffects &= ~CFE_AUTOCOLOR;
		CharFormat.dwEffects &= ~CFE_PROTECTED;

		GetSelectionCharFormat(CharFormat);

		CharFormat.cbSize = sizeof(CHARFORMAT);
		CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE | CFM_CHARSET;
		CharFormat.dwMask &= ~CFM_PROTECTED;
		CharFormat.dwEffects &= ~CFE_AUTOCOLOR;
		CharFormat.dwEffects &= ~CFE_PROTECTED;

		CharFormat.bCharSet = CharInfo.CharSet();

	
		wcscpy(CharFormat.szFaceName, CharInfo.Font().GetBuffer());

		CharFormat.yHeight = CharInfo.Size() * DEFAULT_FONT_SIZE;
		
		CharFormat.crTextColor = ULONGToRGB(CharInfo.Color());

		SetSel(i, i + 1);       // 이부분을 해줘야 변경된 포맷이 적용된다.
		SetSelectionCharFormat(CharFormat);
		SetSel(i + 1, -1);  // 캐럿을 맨끝으로 이동시켜야 이미지가 자기위치에 들어가게된다.

	}

	SetSel(nStartIndex, nEndIndex);
}

int CRichEditCtrlEx::GetFileType(CString ext)
{
	int type = 0;

	ext.MakeLower();

	if (ext == _T("bmp"))					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext==_T("jpg") || ext==_T("jpeg"))	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (ext == _T("gif"))				type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == _T("png"))				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (ext==_T("mng")||ext==_T("jng"))	type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (ext == _T("ico"))				type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext==_T("tiff")||ext==_T("tif"))	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext==_T("tga"))				type = CXIMAGE_FORMAT_TGA;
	else if (ext==_T("win"))				type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (ext==_T("pcx"))				type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
	else if (ext==_T("wbmp"))				type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
	else if (ext==_T("wmf")||ext==_T("emf"))	type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
	else if (ext==_T("j2k")||ext==_T("jp2"))	type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
	else if (ext==_T("jbg"))				type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
	else if (ext==_T("jp2")||ext==_T("j2k"))	type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
	else if (ext==_T("jpc")||ext==_T("j2c"))	type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
	else if (ext==_T("pgx"))				type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
	else if (ext==_T("ras"))				type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
	else if (ext==_T("pnm")||ext==_T("pgm")||ext==_T("ppm")) type = CXIMAGE_FORMAT_PNM;
#endif
	else type = CXIMAGE_FORMAT_UNKNOWN;

	return type;
}

void CRichEditCtrlEx::ClearImageVector(void)
{
	CImageDataObject::m_vecStoragePath.clear();
}

void CRichEditCtrlEx::InsertImgToRichEditCtrl(CString strFilename,INT nSize)
{
	CString strExt = strFilename.Right(3);
	int type = GetFileType(strExt);

	int nType = type;

	FILE* hFile = nullptr;	//file handle to read the image
	if (type > 0 && type < CMAX_IMAGE_FORMATS)
	{
		hFile=_tfopen(strFilename,_T("rb"));

		if(hFile == nullptr) return ;
	}
	CxImage cxImage;

	if(cxImage.Load(hFile, type))		
	{
		if (!cxImage.IsValid())
		{	
			fclose(hFile);
			return ;
		}
		// 061229 KBB m_cxImg.Resample( 16, 16);
		int nWidth = cxImage.GetWidth();
		int nHeight = cxImage.GetHeight();
		int nEditImageSize = static_cast<int>(nSize*0.4f);
		cxImage.Resample(static_cast<int>(nEditImageSize * float(nWidth) / float(nHeight)), nEditImageSize);

		// 이부분에 있어서두 소스 수정이 불가피하다.

		CDC* pDC = this->GetDC();
		HBITMAP bitmap = cxImage.MakeBitmap(pDC->GetSafeHdc());
		IRichEditOle *pRichEditOle = this->GetIRichEditOle();
		if(pRichEditOle)
		{
			CImageDataObject::InsertBitmap(pRichEditOle,bitmap,strFilename);
		}			
		DeleteObject(bitmap);
		ReleaseDC(pDC);

	}

	if(hFile != nullptr) fclose(hFile);
}

CRect CRichEditCtrlEx::GetImageRect(CString strFilename)
{
	CRect rt;

	CString strExt = strFilename.Right(3);
	int type = GetFileType(strExt);

	FILE* hFile = nullptr;	//file handle to read the image
	if (type > 0 && type < CMAX_IMAGE_FORMATS)
	{
		hFile=_tfopen(strFilename,_T("rb"));

		if(hFile == nullptr) return CRect(0,0,0,0);
	}
	CxImage cxImage;

	if(cxImage.Load(hFile, type))		
	{
		if (!cxImage.IsValid())
		{	
			fclose(hFile);
			return CRect(0,0,0,0);
		}
		// 061229 KBB m_cxImg.Resample( 16, 16);
		INT nWidth = cxImage.GetWidth();
		INT nHeight = cxImage.GetHeight();

		rt.SetRect(0,0,nWidth,nHeight);
	}

	if(hFile != nullptr) fclose(hFile);

	return rt;
}