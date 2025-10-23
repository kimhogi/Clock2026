#include "pch.h"
#include "WeatherSpread.h"

constexpr auto MAX_WEATHER_NUM = 200;

void CWeatherSpread::OnSetUp()
{
	CWeatherDataList* pList = GetDataList();

	if (pList)
	{
//		SetBool(SSB_VIRTUALMODE, FALSE);
		SetBool(SSB_HORZSCROLLBAR, TRUE);
		SetBool(SSB_VERTSCROLLBAR, TRUE);

		SetMaxRows(MAX_WEATHER_NUM);

		SetTypeEdit(&m_editCellType,
			SSS_ALIGN_CENTER | SSS_ALIGN_VCENTER,
			1000,
			SS_CHRSET_CHR,
			SS_CASE_NOCASE);

		SetTypeStaticText(&m_readOnlyCellType, SS_TEXT_WORDWRAP | SS_TEXT_VCENTER | SS_TEXT_CENTER);

		SetColumnsInfo(GetFields());

		SetFonts();

		SetSelectionColor(_COLOR_SELECT_);

		SetOperationMode(SS_OP_MODE_NORMAL);
	}
}

void CWeatherSpread::SetColumnsHeaderText(CFields& Fields)
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

LRESULT CWeatherSpread::EditModeOff(WPARAM wParam, LPARAM lParam) {
	UINT nID = (UINT)(wParam);
	SS_EDITMODEOFF* pEditModeOff = (SS_EDITMODEOFF*)(lParam);

	SS_COORD nCol = pEditModeOff->Col;
	SS_COORD nRow = pEditModeOff->Row;
	BOOL     bDataChage = pEditModeOff->fChangeMade;
	CString strData;
	CWeatherData weatherData;

	if (!bDataChage) return 0;

	if (nCol == WEATHER_FIELD_INDEX::WEATHER_CODE + 1)
	{
		strData = GetStringData(nCol, nRow);

		if (weatherData.IsValidWeatherCode(strData))
		{
			SetStringData(nCol + 1, nRow, weatherData.DecisionWeatherName(strData));
			SetStringData(nCol, nRow, weatherData.MakeWeatherCode(strData));
		}
		else
		{
			SetStringData(nCol + 1, nRow, BLANK_STRING);
			PrintLog_Message(_T("존재하지 않는 날씨 코드입니다.유효한 날씨코드는 1, 2, 4~23 입니다"), TRUE);
		}
	}

	if (nCol == WEATHER_FIELD_INDEX::PM25AVG + 1)
	{
		strData = GetStringData(nCol, nRow);
		if (IsNumberWithDotString(strData))
		{
			SetStringData(nCol + 1, nRow, weatherData.DecisionAirLevel(strData));
		}
		else
		{
			SetStringData(nCol, nRow, BLANK_STRING);
			SetStringData(nCol + 1, nRow, BLANK_STRING);
			PrintLog_Message(_T("숫자 입력이 필요합니다."), TRUE);
		}
	}

	UpdateRow(nRow, TRUE);

	return 1;
}

LRESULT CWeatherSpread::LButtonClicked(CWeatherData& Object, WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);
	SS_COORD nCol = ss_coord->Col;
	SS_COORD nRow = ss_coord->Row;
	LRESULT lResult = 0;

	CWeatherDataList* pList = GetPtrDataByIndex<CWeatherDataList>(m_pPtrList, 0);

	if (pList == nullptr) return 0;

	SetCurrentRow(nRow);
	SetCurrentCol(nCol);
	pList->SetCurIndex(nRow - 1);
	Object = pList->GetAt(nRow - 1);

	DrawSelection(nRow, -1);

	return 1;
}