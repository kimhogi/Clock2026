#include "pch.h"
#include "NewsSpread.h"

void CNewsSpread::OnSetUp()
{
	CNewsDataList* pList = GetDataList();

	if (pList)
	{
	//	SetBool(SSB_VIRTUALMODE, FALSE);
		SetBool(SSB_HORZSCROLLBAR, TRUE);
		SetBool(SSB_VERTSCROLLBAR, TRUE);

		SetMaxRows(MAX_NEWS_NUM);

		SetTypeCheckBox(&m_checkBoxCellType, BS_CENTER | SSS_ALIGN_VCENTER, _T(""),
			nullptr, BT_NONE,
			nullptr, BT_NONE,
			nullptr, BT_NONE,
			nullptr, BT_NONE,
			nullptr, BT_NONE,
			nullptr, BT_NONE);

		SetTypeStaticText(&m_readOnlyCellType, SS_TEXT_WORDWRAP | SS_TEXT_VCENTER | SS_TEXT_CENTER);

		SetTypeComboBox(&m_comboBoxCellType, SSS_ALIGN_LEFT, m_pStatus->NewsConf()->GetCategoriesForSpread());

		SetColumnsInfo(GetFields());

		SetRowHeight(SS_ALLROWS, 20);

		SetFonts();

		SetSelectionColor(_COLOR_SELECT_);

		SetOperationMode(SS_OP_MODE_NORMAL);
	}
}

void CNewsSpread::SetFonts(void)
{
	HFONT hFont = CreateFont(15, 8, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("YD윤고딕 340"));

	SetFontRange(SS_ALLCOLS, SS_ALLROWS, SS_ALLCOLS, SS_ALLROWS, hFont, TRUE);
}

void CNewsSpread::RefreshComboString()
{
	SetTypeComboBox(&m_comboBoxCellType, SSS_ALIGN_LEFT, m_pStatus->NewsConf()->GetCategoriesForSpread());
	SetCellTypeRange(NEWS_FIELD_INDEX::CATEGORY+1, 1, NEWS_FIELD_INDEX::CATEGORY + 1, GetMaxRows(), &m_comboBoxCellType);
}

void CNewsSpread::SetColumnCellType(int nCol, CFieldInfo& fi)
{
	int nComboBoxIndex = 0;
	SS_CELLTYPE comboBoxCellType;

	CELL_TYPE  nCellType = fi.GetCellType();

	switch (nCellType)
	{
	case CELL_TYPE::_NORMAL_TYPE_:
		SetCellTypeRange(nCol, 1, nCol, GetMaxRows(), &m_editCellType);
		break;

	case CELL_TYPE::_READONLY_TYPE_:
		SetCellTypeRange(nCol, 1, nCol, GetMaxRows(), &m_readOnlyCellType);
		break;

	case CELL_TYPE::_BUTTON_TYPE_:
		SetCellTypeRange(nCol, 1, nCol, GetMaxRows(), &m_buttonCellType);
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

void CNewsSpread::AllSetUse(CString strData)
{
	auto pList = GetPtrDataByIndex<CNewsDataList>(m_pPtrList, 0);

	vector<CNewsData> vObjects;

	pList->SelectToAllObjects(vObjects);

	for (auto& iter : vObjects)
	{
		iter.SetValue(NEWS_FIELD_INDEX::USE, strData);
	}

	pList->UpdateRecords(vObjects);

	UpdateCurrentSheet(FALSE);
}

void CNewsSpread::OnAllUse(void)
{
	AllSetUse(TRUE_STRING);
}

void CNewsSpread::OnAllDeselectUse(void)
{
	AllSetUse(FALSE_STRING);
}

void CNewsSpread::OnInvertUse(void)
{
	auto pList = GetPtrDataByIndex<CNewsDataList>(m_pPtrList, 0);

	vector<CNewsData> vObjects;

	pList->SelectToAllObjects(vObjects);

	for (auto& iter : vObjects)
	{
		BOOL bUse = GETINT(iter.GetValue(NEWS_FIELD_INDEX::USE));

		iter.SetValue(NEWS_FIELD_INDEX::USE, GETSTR(!bUse));
	}

	pList->UpdateRecords(vObjects);

	UpdateCurrentSheet(FALSE);
}

LRESULT CNewsSpread::EditModeOff(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_EDITMODEOFF* pEditModeOff = (SS_EDITMODEOFF*)(lParam);

	SS_COORD nCol = pEditModeOff->Col;
	SS_COORD nRow = pEditModeOff->Row;
	BOOL     bDataChage = pEditModeOff->fChangeMade;

	if (!bDataChage) return 0;

	auto pList = GetPtrDataByIndex<CNewsDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	auto& Object = pList->GetAt(nRow - 1);
	CString strData = GetStringData(nCol, nRow);

	Object.SetValue(GetFields()[nCol - 1].GetFieldName(), strData);

	pList->SetAt(nRow - 1, Object);

	return 1;
}

BOOL CNewsSpread::UpdateRow(int nSheetIndex, SS_COORD nRow, BOOL bSaveAndValidate)
{
	nSheetIndex = _LIMIT_RANGE_(nSheetIndex, 1, GetSheetCount());

	auto pList = GetPtrDataByIndex<CNewsDataList>(m_pPtrList, nSheetIndex - 1);

	if (pList == nullptr) return FALSE;

	nRow = max(1, nRow);

	int nDataIndex = max(0, nRow - 1);

	CFields& fields = pList->GetFields();

	int nFieldsCount = fields.GetFieldsCount();

	auto& Object = pList->GetAt(nDataIndex);

	if (bSaveAndValidate)
	{
		for (SS_COORD nCol = 1; nCol <= nFieldsCount; nCol++) // 첫번째 컬럼은 숫자이다
		{
			Object.SetValue(fields[nCol - 1].GetFieldName(), GetStringData(nCol, nRow));
		}

		pList->SetAt(nDataIndex, Object);
	}
	else
	{
		if (!pList->IsValidMember(nDataIndex) || !Object.IsValid()) return FALSE; // 정상적이지 않은 데이터라면

		for (int i = 0; i < nFieldsCount; i++)
		{
			CString strData = Object.GetValue(fields[i].GetFieldName());

			if (i == NEWS_FIELD_INDEX::CONTENTS)
			{
				strData = Object.GetContentStringForSpread();
			}

			SetData(i + 1, nRow, strData);
		}
	}

	return TRUE;
}

BOOL CNewsSpread::UpdateDataSheet(int nSheetIndex, BOOL bSaveAndValidate, BOOL bNeedOnSetup)
{
	int nCol, nRow;
	SS_COORD nLastCol = 1;
	SS_COORD nLastRow = 1;

	CString strData;

	CNewsData Object;
	std::vector<CNewsData> vObjects;

	nSheetIndex = _LIMIT_RANGE_(nSheetIndex, 1, GetSheetCount());

	SetSheet(static_cast<short>(nSheetIndex));

	auto pList = GetPtrDataByIndex<CNewsDataList>(m_pPtrList, nSheetIndex - 1);

	if (pList == nullptr) return FALSE;   // 해당사항 없으므로 리턴

	CFields& fields = pList->GetFields();

	int nFieldsCount = fields.GetFieldsCount();

	if (!bSaveAndValidate)
	{
		SetDataRange(1, 1, GetMaxCols(), GetMaxRows(), BLANK_STRING);

		if (bNeedOnSetup) OnSetUp();

		pList->SelectToAllObjects(vObjects);

		nRow = 1;

		for (auto& iter : vObjects)
		{
			for (nCol = 1; nCol <= nFieldsCount; nCol++)
			{
				CString strData = iter[nCol - 1];

				if (nCol - 1 == NEWS_FIELD_INDEX::CONTENTS)
				{
					strData = iter.GetContentStringForSpread();
				}

				SetStringData(nCol, nRow, strData);
			}
			nRow++;
		}
	}
	else
	{
		pList->DeleteAllData();  // 모든 데이터를 지운다.

		GetDataCnt(&nLastCol, &nLastRow);

		for (nRow = 1; nRow <= nLastRow; nRow++)
		{
			Object.DeleteContents();

			if (!Object.IsValidKey()) Object.MakeKeyString();
			Object.SetRowNum(nRow - 1);

			for (nCol = 1; nCol <= nFieldsCount; nCol++)
			{
				GetStringData(nCol, nRow, strData);

				Object.SetValue(fields[nCol - 1].GetFieldName(), strData);
			}

			vObjects.emplace_back(Object);
		}

		pList->AddRecords(vObjects);
	}

	return TRUE;
}

LRESULT CNewsSpread::LButtonClicked(CNewsData& Object, WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);
	SS_COORD nCol = ss_coord->Col;
	SS_COORD nRow = ss_coord->Row;
	SS_CELLTYPE cellType;
	LRESULT lResult = 0;

	auto pList = GetPtrDataByIndex<CNewsDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	GetCellType(ss_coord->Col, ss_coord->Row, &cellType);

	switch (cellType.Type)
	{
	case SS_TYPE_CHECKBOX:
		lResult = CheckBoxChange(wParam, lParam);
		break;

	default:
		break;
	}

	if (nRow == 0)
	{
		pList->Sort(nCol - 1, m_bSortMode);
		ToggleSortMode();
		return 2;
	}
	else
	{
		SetCurrentRow(nRow);
		SetCurrentCol(nCol);
		pList->SetCurIndex(nRow - 1);
		Object = pList->GetAt(nRow - 1);

		DrawSelection(nRow, -1);
	}

	return 1;
}

LRESULT CNewsSpread::CheckBoxChange(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);

	SS_COORD nRow = ss_coord->Row;
	SS_COORD nCol = ss_coord->Col;

	if (nRow < 1) return 0;

	auto pList = GetPtrDataByIndex<CNewsDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	auto Object = pList->GetAt(nRow - 1);

	if (!Object.IsValid())
	{
		Object.Clear();
		Object.MakeKeyString();
	}

	if (nCol - 1 == NEWS_FIELD_INDEX::USE)
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

		Object.SetValue(GetFields()[nCol - 1].GetFieldName(), strData);

		pList->SetAt(nRow - 1, Object);

		UpdateRow(GetActiveSheet(), nRow, FALSE);
	}

	return  1;
}