#include "pch.h"
#include "NoticeSpread.h"



void CNoticeSpread::OnSetUp()
{
	CNoticeDataList* pList = GetDataList();

	if (pList)
	{
	//	SetBool(SSB_VIRTUALMODE, FALSE);
		SetBool(SSB_HORZSCROLLBAR, TRUE);
		SetBool(SSB_VERTSCROLLBAR, TRUE);

		SetMaxRows(MAX_NOTICE_NUM);

		SetTypeEdit(&m_editCellType,
			SSS_ALIGN_CENTER | SSS_ALIGN_VCENTER,
			1000,
			SS_CHRSET_CHR,
			SS_CASE_NOCASE);


		SetTypeCheckBox(&m_checkBoxCellType, BS_CENTER | SSS_ALIGN_VCENTER, BLANK_STRING,
		                nullptr, BT_NONE,
		                nullptr, BT_NONE,
		                nullptr, BT_NONE,
		                nullptr, BT_NONE,
		                nullptr, BT_NONE,
		                nullptr, BT_NONE);

		SetTypeStaticText(&m_readOnlyCellType, SS_TEXT_WORDWRAP | SS_TEXT_VCENTER | SS_TEXT_CENTER);

		RefreshTemplateFiles();

		SetColumnsInfo(GetFields());

		SetFonts();

		SetSelectionColor(_COLOR_SELECT_);

		SetOperationMode(SS_OP_MODE_NORMAL);
	}
}

void CNoticeSpread::RefreshTemplateFiles(void)
{
	m_pStatus->NoticeConf()->RefreshTemplateFiles();

	SetTypeComboBox(&m_comboBoxCellType, SSS_ALIGN_LEFT, m_pStatus->NoticeConf()->TemplateFilesForSpread());

}

void CNoticeSpread::SetColumnsHeaderText(CFields& Fields)
{
	SS_COORD nCol = 1;
	SS_COORD nRow = 0;

	int nSize = Fields.Size();
	SetMaxCols(nSize);

	for (int i = 0; i < nSize; i++)
	{
		CString strTitle = Fields[i].GetCellTitle();

		SetStringData(nCol++, nRow, strTitle);
	}
}


void CNoticeSpread::SetColumnCellType(int nCol, CFieldInfo& fi)
{

	CELL_TYPE  nCellType = fi.GetCellType();

	switch (nCellType)
	{
	case CELL_TYPE::_NORMAL_TYPE_:
		SetCellTypeRange(nCol, 1, nCol, GetMaxRows(), &m_editCellType);
		break;

	case CELL_TYPE::_READONLY_TYPE_:
		SetCellTypeRange(nCol, 1, nCol, GetMaxRows(), &m_readOnlyCellType);
		break;

	case CELL_TYPE::_CHECK_TYPE_:
		SetCellTypeRange(nCol, 1, nCol, GetMaxRows(), &m_checkBoxCellType);
		break;

	case CELL_TYPE::_COMBOBOX_TYPE_:
		SetCellTypeRange(nCol, 1, nCol, GetMaxRows(), &m_comboBoxCellType);
		break;

	default:
		break;
	}

}

LRESULT CNoticeSpread::EditModeOff(WPARAM wParam, LPARAM lParam) {
	UINT nID = (UINT)(wParam);
	SS_EDITMODEOFF* pEditModeOff = (SS_EDITMODEOFF*)(lParam);

	SS_COORD nCol = pEditModeOff->Col;
	SS_COORD nRow = pEditModeOff->Row;
	BOOL     bDataChage = pEditModeOff->fChangeMade;
	CString strData;
	CNoticeData noticeData;

	if (!bDataChage) return 0;

	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	auto Object = pList->GetAt(nRow - 1);
	
	if (nCol == NOTICE_FIELD_INDEX::ANNOTATION + 1 || nCol == NOTICE_FIELD_INDEX::USE + 1)
	{
		strData = GetStringData(nCol, nRow);
		Object.SetValue(GetFields()[nCol - 1].GetFieldName(), strData);

		pList->SetAt(nRow - 1, Object);

	}

	UpdateRow(nRow, FALSE);

	return 1;
}

LRESULT CNoticeSpread::DBClicked(CNoticeData& Object, WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);

	LRESULT lResult = 0;

	int nDataIndex = ss_coord->Row - 1;
	int nFieldIndex = ss_coord->Col - 1;

	SS_COORD nCol = ss_coord->Col;
	SS_COORD nRow = ss_coord->Row;

	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	if (nRow != 0)
	{
		if (nFieldIndex == NOTICE_FIELD_INDEX::TEMPLATE)
		{
			lResult = templateChange(wParam,lParam);

			SetCurrentRow(nRow);
			SetCurrentCol(nCol);
			pList->SetCurIndex(nDataIndex);
			Object = pList->GetAt(nDataIndex);

			DrawSelection(nRow, -1);

		}

	}

	return 1;

}


LRESULT CNoticeSpread::LButtonClicked(CNoticeData& Object, WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);

	LRESULT lResult = 0;

	int nDataIndex = ss_coord->Row - 1;
	int nFieldIndex = ss_coord->Col - 1;

	SS_COORD nCol = ss_coord->Col;
	SS_COORD nRow = ss_coord->Row;

	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);
	CString strFpgName;

	if (pList == nullptr) return 0;

	if (nRow != 0)
	{
		if (nFieldIndex == NOTICE_FIELD_INDEX::USE)
		{
			lResult = CheckBoxChange(wParam, lParam);
		}
	//	else if ( nFieldIndex == NOTICE_FIELD_INDEX::TEMPLATE)
	//	{
	////		lResult = templateChange(wParam,lParam);
	//	}
		
		SetCurrentRow(nRow);
		SetCurrentCol(nCol);
		pList->SetCurIndex(nDataIndex);
		Object = pList->GetAt(nDataIndex);

		strFpgName = Object.GetValue(NOTICE_FIELD_INDEX::TEMPLATE);
		if (!CFileUtils::ExistFile(strFpgName) && strFpgName != BLANK_STRING)
		{
			INT nResult = AfxMessageBox(strFpgName + _T("존재하지 않습니다. 삭제하시겠습니까? "), MB_YESNO);

			if (nResult == IDYES)
			{
				pList->DeleteData(Object);
				UpdateCurrentSheet(FALSE);
			}
		}

		DrawSelection(nRow, -1);
	}

	return 1;

}


LRESULT CNoticeSpread::templateChange(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);

	SS_COORD nRow = ss_coord->Row;
	SS_COORD nCol = ss_coord->Col;

	int nDataIndex = ss_coord->Row - 1;
	int nFieldIndex = ss_coord->Col - 1;

	if (nRow < 1) return 0;

	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	auto Object = pList->GetAt(nDataIndex);

	if (!Object.IsValid())
	{
		Object.Clear();
		Object.MakeKeyString();
	}

	TCHAR szCurrentDirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szCurrentDirectory);

	CImageImportDlg  dlg(TRUE);
	dlg.m_pOFN->lpstrFilter = _T("Fpg Format (*.fpg)\0*.fpg;\0All Formats (*.*)\0*.*\0\0");

	if (dlg.DoModal() == IDOK)
	{
		CString strFpg = dlg.GetPathName();

		Object.SetFpgFile(m_pHDdaVinci, strFpg);

		pList->SetAt(nDataIndex, Object);

		UpdateRow(nRow, FALSE);
	}
	::SetCurrentDirectory(szCurrentDirectory);

	return  1;

}


LRESULT CNoticeSpread::CheckBoxChange(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);

	SS_COORD nRow = ss_coord->Row;
	SS_COORD nCol = ss_coord->Col;

	int nDataIndex = ss_coord->Row - 1;
	int nFieldIndex = ss_coord->Col - 1;

	if (nRow < 1) return 0;

	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	auto Object = pList->GetAt(nDataIndex);

	if (!Object.IsValid())
	{
		Object.Clear();
		Object.MakeKeyString();
	}

	if (nFieldIndex == NOTICE_FIELD_INDEX::USE)
	{
		CString strData = GetStringData(nCol, nRow);

		if (GETINT(strData) == 0)
		{
			strData = TRUE_STRING;
		}
		else
		{
			strData = FALSE_STRING;
		}

		Object.SetValue(NOTICE_FIELD_INDEX::USE, strData);

		pList->SetAt(nDataIndex, Object);

		UpdateRow(nRow, FALSE);
	}

	return  1;
}

//
//LRESULT CNoticeSpread::ComboSelChange(WPARAM wParam, LPARAM lParam)
//{
//
//	UINT nID = (UINT)(wParam);
//	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);
//
//	SS_COORD nRow = ss_coord->Row;
//	SS_COORD nCol = ss_coord->Col;
//
//	int nDataIndex = ss_coord->Row - 1;
//	int nFieldIndex = ss_coord->Col - 1;
//
//	if (ss_coord->Row < 1) return 0;
//
//	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);
//
//	if (pList == nullptr) return 0;
//
//	CNoticeData& Object = pList->GetAt(nDataIndex);
//
//	if (!Object.IsValid())
//	{
//		Object.Clear();
//		Object.MakeKeyString();
//	}
//	
//	if (nFieldIndex == NOTICE_FIELD_INDEX::TEMPLATE)
//	{
//		CString strPrevFpg = Object.GetValue(NOTICE_FIELD_INDEX::TEMPLATE);
//
//		CString strData = GetStringData(nCol, nRow);
//
//		if (strPrevFpg == BLANK_STRING && strData != BLANK_STRING)
//		{
//			Object.SetValue(NOTICE_FIELD_INDEX::USE, TRUE_STRING);
//
//			Object.SetFpgFile(m_pHDdaVinci, GetNoticeDir() + strData);
//		}
//
//		if (strPrevFpg != BLANK_STRING && strData.CompareNoCase(strPrevFpg) != 0)
//		{
//			UINT nYesNo = AfxMessageBox(_T("템플릿 다른것을 선택했습니다. 이전에 입력된 알리아스는 없어집니다. 진행할까요 ?"), MB_YESNO);
//
//			if (nYesNo == IDYES)
//			{
//				Object.SetFpgFile(m_pHDdaVinci, GetNoticeDir() + strData);
//			}
//			else
//			{
//				Object.SetValue(NOTICE_FIELD_INDEX::TEMPLATE, GetNoticeDir() + strPrevFpg);
//			}
//		}
//
//		pList->SetAt(nDataIndex, Object);
//
//		UpdateRow(nRow, FALSE);
//
//	}
//
//	return  0;
//}

BOOL CNoticeSpread::UpdateRow(SS_COORD nRow, BOOL bSaveAndValidate)
{
	int nSheetIndex = GetActiveSheet();
	return UpdateRow(nSheetIndex, nRow, bSaveAndValidate);
}

BOOL CNoticeSpread::UpdateRow(int nSheetIndex, SS_COORD nRow, BOOL bSaveAndValidate)
{
	nSheetIndex = _LIMIT_RANGE_(nSheetIndex, 1, GetSheetCount());
	CNoticeDataList* pList = GetList(nSheetIndex - 1);

	if (pList == nullptr) return FALSE;

	int nDataIndex = max(0, nRow - 1);

	CFields& fields = pList->GetFields();

	int nFieldsCount = fields.GetFieldsCount();

	auto Object = pList->GetAt(nDataIndex);

	if (bSaveAndValidate)
	{

		Object.SetValue(NOTICE_FIELD_INDEX::USE, GetStringData(NOTICE_FIELD_INDEX::USE + 1, nRow));
		Object.SetValue(NOTICE_FIELD_INDEX::ANNOTATION, GetStringData(NOTICE_FIELD_INDEX::ANNOTATION + 1, nRow));

		pList->SetAt(nDataIndex, Object);
	}
	else
	{
		if (!pList->IsValidMember(nDataIndex) || !Object.IsValid()) return FALSE; // 정상적이지 않은 데이터라면

		SetData(NOTICE_FIELD_INDEX::USE + 1, nRow, Object.GetValue(NOTICE_FIELD_INDEX::USE));
		SetData(NOTICE_FIELD_INDEX::TEMPLATE + 1, nRow, CFileUtils::ExtractFileName(Object.GetValue(NOTICE_FIELD_INDEX::TEMPLATE)));
		SetData(NOTICE_FIELD_INDEX::ANNOTATION + 1, nRow, Object.GetValue(NOTICE_FIELD_INDEX::ANNOTATION));
	}

	return TRUE;
}

 BOOL CNoticeSpread::UpdateCurrentSheet(BOOL bSaveAndValidate)
{
	return UpdateDataSheet(GetActiveSheet(), bSaveAndValidate, FALSE);
}

BOOL CNoticeSpread::UpdateDataSheet(int nSheetIndex, BOOL bSaveAndValidate, BOOL bNeedOnSetup)
{
	int nCol, nRow;
	SS_COORD nLastCol = 1;
	SS_COORD nLastRow = 1;

	CString strData;

	CNoticeData Object;
	std::vector<CNoticeData> vObjects;

	nSheetIndex = _LIMIT_RANGE_(nSheetIndex, 1, GetSheetCount());

	CNoticeDataList* pList = GetList(nSheetIndex - 1);

	if (pList == nullptr) return FALSE;   // 해당사항 없으므로 리턴

	CFields& fields = pList->GetFields();

	int nFieldsCount = fields.GetFieldsCount();

	if (!bSaveAndValidate)
	{

		//BOOL bVMode = GetBool(SSB_VIRTUALMODE);

		//if (bVMode)
		//{
		//	SetBool(SSB_VIRTUALMODE, FALSE);
		//}

		SetDataRange(1, 1, GetMaxCols(), GetMaxRows(), BLANK_STRING);

		//SetBool(SSB_VIRTUALMODE, bVMode);

		if (bNeedOnSetup) OnSetUp();

		pList->SelectToAllObjects(vObjects);

		nRow = 1;

		for (auto& iter : vObjects)
		{
			SetData(NOTICE_FIELD_INDEX::USE + 1, nRow, iter.GetValue(NOTICE_FIELD_INDEX::USE));
			SetData(NOTICE_FIELD_INDEX::TEMPLATE + 1, nRow, CFileUtils::ExtractFileName(iter.GetValue(NOTICE_FIELD_INDEX::TEMPLATE)));
			SetData(NOTICE_FIELD_INDEX::ANNOTATION + 1, nRow, iter.GetValue(NOTICE_FIELD_INDEX::ANNOTATION));
			nRow++;
		}

		SetSheetName(static_cast<short>(nSheetIndex), pList->GetTypeName() + GETSTR(nSheetIndex));
	}
	else
	{
		INT nCount = pList->Size();

		GetDataCnt(&nLastCol, &nLastRow);

		for (nRow = 1; nRow <= nLastRow; nRow++)
		{
			if ((nRow - 1) >= nCount)
			{
				Object.Clear();
				Object.MakeKeyString();
				Object.SetRowNum(nRow - 1);
			}
			else
			{
				Object = pList->GetAt(nRow - 1);
			}

			Object.SetValue(NOTICE_FIELD_INDEX::USE, GetStringData(NOTICE_FIELD_INDEX::USE + 1, nRow));
			Object.SetValue(NOTICE_FIELD_INDEX::ANNOTATION, GetStringData(NOTICE_FIELD_INDEX::ANNOTATION + 1, nRow));
			
			vObjects.emplace_back(Object);
		}

		pList->DeleteAllData();
		pList->AddRecords(vObjects);

	}

	return TRUE;
}

void CNoticeSpread::AllSetUse(CString strData)
{
	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);

	std::vector<CNoticeData> vObjects;

	pList->SelectToAllObjects(vObjects);

	for (auto& iter : vObjects)
	{
		iter.SetValue(NOTICE_FIELD_INDEX::USE, strData);
	}

	pList->UpdateRecords(vObjects);

	UpdateCurrentSheet(FALSE);
}


void CNoticeSpread::OnAllUse(void)
{
	AllSetUse(TRUE_STRING);

}

void CNoticeSpread::OnAllDeselectUse(void)
{
	AllSetUse(FALSE_STRING);
}

void CNoticeSpread::OnInvertUse(void)
{
	CNoticeDataList* pList = GetPtrDataByIndex<CNoticeDataList>(m_pPtrList, 0);

	std::vector<CNoticeData> vObjects;

	pList->SelectToAllObjects(vObjects);

	for (auto& iter : vObjects)
	{
		BOOL bUse = GETINT(iter.GetValue(NOTICE_FIELD_INDEX::USE));

		iter.SetValue(NOTICE_FIELD_INDEX::USE, GETSTR(!bUse));
	}

	pList->UpdateRecords(vObjects);

	UpdateCurrentSheet(FALSE);
}
