#pragma once

#include "Data/sData/TemplateListSpread_2.hpp"
#include "ClockData.h"


using namespace std;
using namespace libxl;
using namespace ns_News;


class CNewsSpread : public CTemplateListSpread<CNewsDataList, CNewsData>
{
public:
	CNewsSpread() {};
	virtual ~CNewsSpread() {};

private:
	CStatus* m_pStatus;

public:
	void SetStatus(CStatus* pStatus) { m_pStatus = pStatus; }

	void OnSetUp() override;

	void SetFonts(void) override;

	void RefreshComboString();
	void SetColumnCellType(int nCol, CFieldInfo& fi) override;

	void AllSetUse(CString strData);

	void OnAllUse(void);

	void OnAllDeselectUse(void);

	void OnInvertUse(void);

	LRESULT LButtonClicked(CNewsData& Object, WPARAM wParam, LPARAM lParam) override;

	LRESULT EditModeOff(WPARAM wParam, LPARAM lParam) override;

	BOOL UpdateRow(int nSheetIndex, SS_COORD nRow, BOOL bSaveAndValidate) override;

	BOOL UpdateDataSheet(int nSheetIndex, BOOL bSaveAndValidate, BOOL bNeedOnSetup) override;

	LRESULT CheckBoxChange(WPARAM wParam, LPARAM lParam) override;
	
};
