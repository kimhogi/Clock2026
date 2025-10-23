#pragma once

#include "Data/sData/TemplateListSpread_2.hpp"
#include "ClockData.h"


using namespace std;
using namespace libxl;
using namespace ns_Notice;

constexpr auto MAX_NOTICE_NUM = 50;


class CNoticeSpread :public CTemplateListSpread<CNoticeDataList, CNoticeData>
{
public:
	CNoticeSpread() {};
	virtual ~CNoticeSpread() {};
	
private:
	CStatus* m_pStatus;
	CHDdaVinci* m_pHDdaVinci;

public:
	void SetHDdaVinci(CHDdaVinci* pHDdaVinci) { m_pHDdaVinci = pHDdaVinci; }

	void SetStatus(CStatus* pStatus) { m_pStatus = pStatus; }

	void OnSetUp() override;

	void RefreshTemplateFiles(void);

	void SetColumnsHeaderText(CFields& Fields) override;

	void SetColumnCellType(int nCol, CFieldInfo& fi) override;

	LRESULT EditModeOff(WPARAM wParam, LPARAM lParam) override;
	
	LRESULT LButtonClicked(CNoticeData& Object, WPARAM wParam, LPARAM lParam) override;
	LRESULT DBClicked(CNoticeData& Object, WPARAM wParam, LPARAM lParam);


	LRESULT CheckBoxChange(WPARAM wParam, LPARAM lParam) override;
	LRESULT templateChange(WPARAM wParam, LPARAM lParam);
	BOOL UpdateRow(SS_COORD nRow, BOOL bSaveAndValidate) override;
	BOOL UpdateRow(int nSheetIndex, SS_COORD nRow, BOOL bSaveAndValidate) override;
	BOOL UpdateCurrentSheet(BOOL bSaveAndValidate) override;
	BOOL UpdateDataSheet(int nSheetIndex, BOOL bSaveAndValidate, BOOL bNeedOnSetup) override;

	void AllSetUse(CString strData);

	void OnAllUse(void);

	void OnAllDeselectUse(void);

	void OnInvertUse(void);
};