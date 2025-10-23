#pragma once

#include "ClockData.h"


using namespace std;
using namespace libxl;
using namespace ns_Weather;

constexpr auto COL_ALIAS = 1;
constexpr auto COL_VALUE = 2;
constexpr auto COL_TYPE =  3;


constexpr auto COL_ALIAS_WIDTH = 10;
constexpr auto COL_VALUE_WIDTH = 20;

class CAliasSpread :public TSpread
{
public:
	CAliasSpread() 
	{
		m_pAliasStringList = new CStringList;
		m_pImageAliasStringList = new CStringList;
	}

	virtual ~CAliasSpread() 
	{
		Detach();
		SAFE_DELETE(m_pAliasStringList);
		SAFE_DELETE(m_pImageAliasStringList);
	}
	
private:
	SS_CELLTYPE m_editCellType;
	SS_CELLTYPE m_readOnlyCellType;

	CHDdaVinci* m_pHDdaVinci;
	CStatus* m_pStatus;

	CWnd* m_pOwnerWnd;

	CNoticeData m_NoticeData;
	CStringList* m_pAliasStringList;
	CStringList* m_pImageAliasStringList;

	INT m_nRows;

	CFont		  m_defFont;
	CFont         m_boldFont;

public:

	CHDdaVinci* GetHDdaVinci(void) { return m_pHDdaVinci; }

	void SetHDdaVinci(CHDdaVinci* pHDdaVinci) { m_pHDdaVinci = pHDdaVinci; }

	CStatus* GetStatus(void) { return m_pStatus; }

	void SetStatus(CStatus* pStatus) { m_pStatus = pStatus; }

	bool Attach(CWnd* pParentWnd, UINT nID)
	{
		m_pOwnerWnd = pParentWnd;
		CWnd* pWnd = pParentWnd->GetDlgItem(nID);
		HWND  hwnd = pWnd->GetSafeHwnd();

		if (!CWnd::Attach(hwnd))
		{
			return false;
		}
		return true;
	}

	CNoticeData GetNoticeData(void) { return m_NoticeData; }

	void SetNoticeData(CNoticeData& noticeData) { m_NoticeData = noticeData; }

	void SetFonts(void);

	void DrawSelection(SS_COORD nRow, SS_COORD nPrevRow = -1);

	void OnSetUp(CNoticeData& noticeData,BOOL bGetAlias);

	void SetColumnsHeaderText(void);

	void SetCellsType(void);

	void SetColumnsInfo(void);

	void SetColumnsWidth(void);

	void LoadImageResource(INT nIndex);

	CString GetStringData(SS_COORD nCol, SS_COORD nRow)
	{
		wchar_t sData[MAX_CHAR_SIZE] = { 0, };

		GetData(nCol, nRow, sData);

		return CString(sData);
	}

	LRESULT EditModeOff(WPARAM wParam, LPARAM lParam);

	LRESULT LButtonClicked(WPARAM wParam, LPARAM lParam);

	BOOL UpdateRow(SS_COORD nRow, BOOL bSaveAndValidate);
	
	BOOL UpdateRow(int nSheetIndex, SS_COORD nRow, BOOL bSaveAndValidate);

	BOOL UpdateDataSheet(BOOL bSaveAndValidate, BOOL bNeedOnSetup);
	
	BOOL UpdateDataSheet(int nSheetIndex, BOOL bSaveAndValidate, BOOL bNeedOnSetup);

	BOOL PreTranslateMessage(MSG* pMsg) override;
};