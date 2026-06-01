#include "pch.h"
#include "AliasSpread.h"


void CAliasSpread::OnSetUp(CNoticeData& noticeData,BOOL bGetAlias)
{
	SetNoticeData(noticeData);

//	SetBool(SSB_VIRTUALMODE, FALSE);
	SetBool(SSB_HORZSCROLLBAR, TRUE);
	SetBool(SSB_VERTSCROLLBAR, TRUE);

	SetTypeEdit(&m_editCellType,
	            SSS_ALIGN_CENTER | SSS_ALIGN_VCENTER,
	            1000,
	            SS_CHRSET_CHR,
	            SS_CASE_NOCASE);

	SetTypeStaticText(&m_readOnlyCellType, SS_TEXT_WORDWRAP | SS_TEXT_VCENTER | SS_TEXT_CENTER);

	CString strFpg = noticeData.GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

	if (!CFileUtils::ExistFile(strFpg))
	{
		m_nRows = 0;
	}
	else
	{
		if (bGetAlias)
		{
			m_nRows = noticeData.GetAliases(m_pHDdaVinci);
		}
		else
		{
			m_nRows = noticeData.CountAlias();
		}
	}

	SetMaxCols(COL_TYPE);
	SetMaxRows(m_nRows);

	SetColumnsInfo();

	SetFonts();

	SetOperationMode(SS_OP_MODE_NORMAL);

}

void CAliasSpread::SetColumnsHeaderText(void)
{
	SS_COORD nCol = 1;
	SS_COORD nRow = 0;
	
	SetData(COL_ALIAS, nRow, _T("알리아스"));
	SetData(COL_VALUE, nRow, _T("데이터"));
	SetData(nCol, nRow, _T("타입"));
}

void CAliasSpread::SetColumnsInfo(void)
{
	SetColumnsHeaderText();
	SetCellsType();
	SetColumnsWidth();
}

void CAliasSpread::SetColumnsWidth(void)
{
	SetColWidth(COL_ALIAS, COL_ALIAS_WIDTH);
	SetColWidth(COL_VALUE, COL_VALUE_WIDTH);
	SetColWidth(COL_TYPE, 0);
}

void CAliasSpread::SetCellsType(void)
{
	INT nMaxRow = GetMaxRows();

	for (INT nIndex = 0; nIndex < nMaxRow; nIndex++)
	{
		SetCellType(COL_ALIAS, nIndex + 1, &m_readOnlyCellType);

		if (m_NoticeData.GetAliasType(nIndex) == ALIAS_TYPE::TEXT)
		{
			SetCellType(COL_VALUE, nIndex + 1, &m_editCellType);
		}
		else
		{
			SetCellType(COL_VALUE, nIndex + 1, &m_readOnlyCellType);
		}
    }
}

void CAliasSpread::SetFonts(void)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, _T("굴림"));

	SetFontRange(SS_ALLCOLS, SS_ALLROWS, SS_ALLCOLS, SS_ALLROWS, hFont, TRUE);
}

void CAliasSpread::DrawSelection(SS_COORD nRow, SS_COORD nPrevRow)
{
	COLORREF clrSelect = _COLOR_SELECT_;

	if (nPrevRow == -1)
	{
		SetColorRange(-1, -1, -1, -1, CELL_COLOR_WHITE, RGB(0, 0, 0));
	}
	else
	{
		SetColorRange(-1, nPrevRow, -1, nPrevRow, CELL_COLOR_WHITE, RGB(0, 0, 0));
	}

	SetColorRange(-1, nRow, -1, nRow, clrSelect, RGB(0, 0, 0));

}


LRESULT CAliasSpread::EditModeOff(WPARAM wParam, LPARAM lParam) {
	UINT nID = (UINT)(wParam);
	SS_EDITMODEOFF* pEditModeOff = (SS_EDITMODEOFF*)(lParam);

	SS_COORD nCol = pEditModeOff->Col;
	SS_COORD nRow = pEditModeOff->Row;
	BOOL     bDataChage = pEditModeOff->fChangeMade;

	if (!bDataChage) return 0;

	UpdateRow(nRow, TRUE);

	return 1;
}
void CAliasSpread::LoadImageResource(INT nIndex)
{
	
	TCHAR szCurrentDirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szCurrentDirectory);

	CImageImportDlg dlg(TRUE);
	dlg.m_pOFN->lpstrFilter =  _T("All Format (*.tga *.png *.ccs)\0*.tga;*.png;*.ccs\0TGA Format (*.tga)\0*.tga\0PNG Format (*.png)\0*.png\0CCS Format (*.ccs)\0*.ccs\0All Formats (*.*)\0*.*\0\0");

	if (dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();

		m_NoticeData.SetAliasValue(nIndex, strFileName);

		UpdateRow(nIndex + 1, FALSE);
	}
	::SetCurrentDirectory(szCurrentDirectory);
}


LRESULT CAliasSpread::LButtonClicked(WPARAM wParam, LPARAM lParam) {

	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);
	SS_COORD nCol = ss_coord->Col;
	SS_COORD nRow = ss_coord->Row;
	SS_CELLTYPE cellType;
	LRESULT lResult = 0;

	GetCellType(nCol, nRow, &cellType);

	if (nCol == COL_VALUE)
	{
		switch (cellType.Type)
		{
		case SS_TYPE_STATICTEXT:
			LoadImageResource(nRow - 1);
			break;
		default:
			break;
		}
	}
	else
	{
		DrawSelection(nRow, -1);
	}


	return 1;
}

BOOL CAliasSpread::UpdateRow(SS_COORD nRow, BOOL bSaveAndValidate)
{
	int nSheetIndex = GetActiveSheet();
	return UpdateRow(nSheetIndex, nRow, bSaveAndValidate);
}

BOOL CAliasSpread::UpdateRow(int nSheetIndex, SS_COORD nRow, BOOL bSaveAndValidate)
{
	nSheetIndex = _LIMIT_RANGE_(nSheetIndex, 1, GetSheetCount());
	
	CString strData;

	int nDataIndex = max(0, nRow - 1);

	if (bSaveAndValidate)
	{
		strData = GetStringData(COL_VALUE, nRow); 

		if (m_NoticeData.GetAliasType(nDataIndex) == ALIAS_TYPE::TEXT)
		{
			m_NoticeData.SetAliasValue(nDataIndex, strData);
		}
	}
	else
	{
		strData = m_NoticeData.GetAliasValue(nDataIndex);

		if (m_NoticeData.GetAliasType(nDataIndex) != ALIAS_TYPE::TEXT)
		{
			strData = CFileUtils::ExtractFileName(strData);
		}

		SetData(COL_VALUE, nRow, strData);
	}

	return TRUE;
}

BOOL CAliasSpread::UpdateDataSheet(BOOL bSaveAndValidate, BOOL bNeedOnSetup)
{
	INT nSheetIndex = GetActiveSheet();

	return UpdateDataSheet(nSheetIndex, bSaveAndValidate, bNeedOnSetup);
}

BOOL CAliasSpread::UpdateDataSheet(int nSheetIndex, BOOL bSaveAndValidate, BOOL bNeedOnSetup)
{
	int nCol, nRow;
	SS_COORD nLastCol = 1;
	SS_COORD nLastRow = 1;

	CString strAlias;
	CString strValue;
	CString strType;

	nSheetIndex = _LIMIT_RANGE_(nSheetIndex, 1, GetSheetCount());

	if (!bSaveAndValidate)
	{
		SetDataRange(1, 1, GetMaxCols(), GetMaxRows(), BLANK_STRING);

		if (bNeedOnSetup)
		{
			OnSetUp(m_NoticeData, FALSE);
		}

		for(INT nIndex = 0; nIndex < m_nRows; nIndex++)
		{
			strAlias = m_NoticeData.GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nIndex);
			strValue = m_NoticeData.GetValue(NOTICE_FIELD_INDEX::VALUE_1 + nIndex);
			strType = m_NoticeData.GetValue(NOTICE_FIELD_INDEX::TYPE_1 + nIndex);


			SetData(COL_ALIAS, nIndex + 1, m_NoticeData.GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nIndex));
			if (strType != GETSTR(FROM_E(ALIAS_TYPE::TEXT)))
			{
				SetData(COL_VALUE, nIndex + 1,  CFileUtils::ExtractFileName(m_NoticeData.GetValue(NOTICE_FIELD_INDEX::VALUE_1 + nIndex)));
			}
			else
			{
				SetData(COL_VALUE, nIndex + 1, m_NoticeData.GetValue(NOTICE_FIELD_INDEX::VALUE_1 + nIndex));
			}

			SetData(COL_TYPE, nIndex + 1, m_NoticeData.GetValue(NOTICE_FIELD_INDEX::TYPE_1 + nIndex));
		}

	}
	else
	{
		CNoticeData noticeBackup = m_NoticeData;
		m_NoticeData.ClearAliasAndValue();
		
		GetDataCnt(&nLastCol, &nLastRow);

		for (nRow = 1; nRow <= nLastRow; nRow++)
		{
			strAlias = GetStringData(COL_ALIAS, nRow);
			strValue = GetStringData(COL_VALUE, nRow);
			strType = GetStringData(COL_TYPE, nRow);

			m_NoticeData.SetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nRow - 1, strAlias);

			if (strType != GETSTR(FROM_E(ALIAS_TYPE::TEXT)))
			{
				strValue = noticeBackup.GetValue(NOTICE_FIELD_INDEX::VALUE_1 + nRow - 1);
			}
			m_NoticeData.SetValue(NOTICE_FIELD_INDEX::VALUE_1 + nRow - 1, strValue);
			m_NoticeData.SetValue(NOTICE_FIELD_INDEX::TYPE_1 + nRow - 1, strType);
			
			
		}
	}

	return TRUE;
}


BOOL CAliasSpread::PreTranslateMessage(MSG* pMsg)
{
	SS_CELLCOORD active_coord;
	SS_CELLCOORD valid_coord;

	
	switch (pMsg->message)
	{

	case WM_KEYDOWN:		// WM_CHAR로 하면 control key들은 먹지 않는다..

		switch (pMsg->wParam)
		{
		case VK_RETURN:
			SetEditMode(FALSE);
			GetActiveCell(&active_coord.Col, &active_coord.Row);
			GetLastValidCell(&valid_coord.Col, &valid_coord.Row);

			if (active_coord.Row == valid_coord.Row)
			{
				UpdateDataSheet(GetActiveSheet(), TRUE, FALSE);
				AfxGetMainWnd()->SendMessage(WM_APPLY_NOTICE, 0, 0);
			}
			else
			{
				SetActiveCell(active_coord.Col, active_coord.Row + 1);
				UpdateDataSheet(GetActiveSheet(), TRUE, FALSE);
				SetEditMode(TRUE);
			}
			break;

		case _T('C'):
			if (IsCTRLpressed())
			{
				ClipboardCopy();
				return TRUE;
			}
			break;

		case _T('V'):
			if (IsCTRLpressed())
			{
				ClipboardPaste();
				UpdateDataSheet(GetActiveSheet(), TRUE, FALSE);
				return TRUE;
			}
			break;

		case _T('X'):
			if (IsCTRLpressed())
			{
				ClipboardCut();
				UpdateDataSheet(GetActiveSheet(), TRUE, FALSE);
				return TRUE;
			}
			break;

		case VK_F2:
			if (m_pOwnerWnd) m_pOwnerWnd->SendMessage(WM_SUPER_INOUT, 0, 0);
			return TRUE;
			break;

		case VK_F3:
			SetEditMode(TRUE);
			break;

		case VK_ESCAPE:
			if (m_pOwnerWnd) m_pOwnerWnd->SendMessage(WM_HDDAVINCI_CLEAR, 0, 0);
			return TRUE;
			break;

		}
		break;
	}

	return TSpread::PreTranslateMessage(pMsg);
}