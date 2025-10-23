
// Clock2023Dlg.h : header file
//

#pragma once

#include "ClockData.h"
#include "Data/sData/TemplateListSpread_2.hpp"
#include "NewsSpread.h"
#include "WeatherSpread.h"
#include "RichEditCtrlEx.h"
#include "AliasSpread.h"
#include "NoticeSpread.h"

#include <mutex>

#include "LiveVideoInput/CLiveVideo.h"


static UINT BASED_CODE	indicators[] = {
	ID_INDICATOR_FILENAME,
	ID_INDICATOR_ONAIR,
	ID_INDICATOR_SUPER,
	ID_INDICATOR_MOUSE_POS,
	ID_INDICATOR_TIME,
	ID_INDICATOR_BUILD

};

constexpr auto DURATION_AGE_TIMER = 500;

constexpr auto ID_TIMER_STATUSBAR_TIME = 10;
constexpr auto ID_TIMER_TIME = 20;
constexpr auto ID_TIMER_DISPLAY_CLOCK = 30;
constexpr auto ID_TIMER_DISPLAY_BIG_CLOCK = 40;
constexpr auto ID_TIMER_DISPLAY_STATION_LOGO = 50;
constexpr auto ID_TIMER_SYNC = 60;
constexpr auto ID_TIMER_AUTO_WEATHER_READING = 70;
constexpr auto ID_TIMER_NEWS_DATA = 80;
constexpr auto ID_TIMER_WEATHER_DATA = 90;
constexpr auto ID_TIMER_AUTO_SAVE = 100;
constexpr auto ID_TIMER_LOG = 110;
constexpr auto ID_TIMER_DISPLAY_NOTICE = 120;
constexpr auto ID_TIMER_DISPLAY_AGE = 230;


// CClock2023Dlg dialog
class CClock2023Dlg : public CDialogEx
{
// Construction
public:
	CClock2023Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOCK2023_DIALOG };
#endif
	CStatus* m_pStatus;
	CHDdaVinci* m_pHDdaVinci;
	            
	CSuperInOutButton m_btnSuperInOut;

	CWnd* m_pTrackMenuOwner;

	CNewsSpread m_SpreadNews;
	CWeatherSpread m_SpreadWeather;
	CAliasSpread m_SpreadAlias;
	CNoticeSpread m_SpreadNotice;

	CListBox m_ListBoxLog;
	int m_nLogCount;

	CToolTipCtrl m_ToolTip;

	BOOL    m_bFileChange;

	LOGFONT lfDefault_StatusBarFont;
	LOGFONT lfBold_StatusBarFont;

	XStatusBar m_statusBar;

	CString m_strPrevTime;

	CComboBox* m_pComboDateFormat;

	CButtonST m_btnDispClock;
	CButtonST m_btnDispBigClock;
	CButtonST m_btnDispLogo;
	
	CButtonST m_btnDispNews;
	CButtonST m_btnNewsScrollPrepare;
	CButtonST m_btnDispNewsScroll;
	CButtonST m_btnDispWeather;

	CButtonST m_btnDispAge;
	CButtonST m_btnDispNotice;
	CButtonST m_btnDispAgeNotice;

	CButtonST m_btnDispManualUp;

	CButtonST m_btnDispLiveVideo;

	COLORREF m_OnAirButtonColor;
	COLORREF m_OffAirButtonColor;

	CRichEditCtrlEx m_RENewsData;

	CComboBox* m_pComboNewsCategory;

	CFontComboBox m_ComboNewsTextFont;

	CComboBox* m_pComboNewsTextSize;

	CMFCColorButton m_btnNewsTextColor;
	CMFCColorButton m_btnREBackColor;

	CImageButton m_btnImageShortCut[IMAGE_SHORTCUT_NUM];

	CPreviewStatic m_PreviewNewsScroll;

	CStatic m_PreviewLiveVideo;

	CLiveVideo* m_pLiveVideo;

	CCGXObject* m_pInputLiveVideoObject;
	CCGXObject* m_pInputLiveVideoMaskObject;
	PULONG pInputLiveVideoFrame;

	CCGXLayer* m_pInputLiveVideoLayer;

	//CCGXObject* m_pClockObjects[FROM_E(CLOCK_OBJECTS::LAST_NUM)];

	//CCGXObject* m_pBigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::LAST_NUM)];

	CCGXLayer* m_pWeatherLayer[FROM_E(WEATHER_LAYERS::LAST_NUM)];
	CCGXObject* m_pWeatherObject[FROM_E(WEATHER_OBJECTS::LAST_NUM)];

	CStringList* m_pWeatherAlias;

	INT m_nWeathterSymbolOffsetX[3];
	INT m_nWeathterSymbolOffsetY[3];
	INT m_nWeathterSymbolOffsetZ[3];

	INT m_nWeatherSize;

	CCGXLayer* m_pNewsLayers[FROM_E(NEWS_LAYERS::LAST_NUM)];
	CCGXObject* m_pNewsObjects[FROM_E(NEWS_OBJECTS::LAST_NUM)];
	CCGXObject* m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::LAST_NUM)];

	CCGXObject* m_pNewsImageSystemObject;
	CStringList* m_pNewsAlias;

	CComboBox* m_pComboNewsScrollTemplate;

	CNewsData m_prevNewsData;
	CNewsData m_curNewsData;

	INT m_nCurNews;
	INT m_nPrevNews;

	CString m_strPrevFont;
	CString m_strPrevFontSize;
	INT m_nPrevHanEngMode;

	BOOL m_bPrepareNewsScroll;

	CProgressWnd* m_pWndProgress;

	INT m_nNewScrollCounter;

	INT m_nCurWeather;

	std::mutex m_mutexSNTP;
	std::mutex m_mutexReadWeather;
	std::mutex m_mutexHandler;
	std::mutex m_mutexLog;
	std::mutex m_mutexButton;

	CPreviewStatic m_PreviewNotice;

	INT m_nNoticeCounter;

	CNoticeData m_CurNoticeData;

	CTime m_timeAgeStart;
	INT m_nCurNoticeCount;
	INT m_nCurNoticeIndex;

	BOOL m_bAgeFirstRun;
	BOOL m_bUseAge;
	BOOL m_bAgeShow;
	BOOL m_bPrevAgeShow;
	CString m_strPrevAgeTime;
	CString m_strAgeFileName;

	INT m_nAgePosX;
	INT m_nAgePosY;

	CComboBox* m_pComboManualUpTemplate;

	CComboBox* m_pComboLiveVideoTemplate;

	DWORD m_dwPrevTickDisplayClock;
	DWORD m_dwPrevTickDisplayBigClock;
	DWORD m_dwPrevTickDisplayNotice;
	DWORD m_dwPrevTickDisplayNews;
	DWORD m_dwPrevTickDisplayWeather;


	ALIAS_COORD m_NewsContentsCoord;
	INT m_nNewsSpaceWidth = 0;
	INT m_nNewsPrevXPos = 0;

	CCGXObject* m_pSystemImageObject;

public:
	BOOL InitCtrls(void);

	BOOL InitEdits(void);

	BOOL InitButtons(void);

	BOOL InitValues(void);
	
	BOOL InitTimer(void);

	BOOL InitProgressWnd(void);

	BOOL InitToolTips(BOOL bNeedCreate = FALSE);

	BOOL DrawPreview(CPreviewStatic& staticPreview, CString strFpgFileName);

	INT FindStringFromComboBox(CString FindString, CComboBox* pCombo);
	void SetComboText(CComboBox* pCombo, std::vector<CString>& vData);
	void GetComboText(CComboBox* pCombo, std::vector<CString>& vData);
	void UpdateComboData(BOOL bUpdataData);

	BOOL InitCombos(void);
	
	BOOL InitLiveVideo(void);

	BOOL InitPreview(void);

	BOOL InitClocks(void);
	
	BOOL InitNews(void);

	BOOL InitWeather(void);

	BOOL InitNotice(void);

	BOOL IsEditFocus(void);
	
	BOOL InitStatusBar(void);


	void SetStautsBar_PaneColor(INT nID, COLORREF clrText, COLORREF clrBack);
	void WriteStatusBar_FileName(CString strFileName);
	void WriteStatusBar_OnAir(void);
	void WriteStatusBar_SuperInOut(void);
	void WriteStatusBar_MousePos(CPoint pt);
	void WriteStatusBar_Time(void);
	void WriteStatusBar_BuildTime(void);

	void SetUpSpreads(BOOL bNeedDetach = FALSE);

	template<typename L, typename O>
	void SetUpSpread(CTemplateListSpread<L, O>& Spread, UINT nIDC, CPtrList* pPtrList) {

		Spread.Attach(this, nIDC);

		Spread.SetPtrList(pPtrList);

		Spread.SetActiveSheet(1);

		Spread.UpdateDataSheet(1, FALSE, TRUE);
	}

	void SetUpSpreadAlias(CAliasSpread& Spread, UINT nIDC)
	{
		Spread.Attach(this, nIDC);

		Spread.SetActiveSheet(1);

		Spread.UpdateDataSheet(1, FALSE, TRUE);
	}

	void DetachSpreads(void);

	void RefreshUI(void);

	void UpdateREUI(BOOL bUpdate);

	void OpenLogFolder(void);
	void Serialize(CArchive& ar) override;

	void SetFonts(void);

//	static UINT FirstReadWeatherThread(LPVOID lpvoid);

	static UINT AutoReadingWeatherThread(LPVOID lpvoid);
	
	static UINT SyncSNTPThread(LPVOID lpvoid);
	void ConfigTimerForAutoReadingWeather(BOOL bAutoReadingWeather);
	static int Handler(LPVOID me, CString strLayer, int effectNo, int frameIdx, int frameLength, int totalidx, int reversetotalidx, float* in, int* out, LPVOID pAgent);
	static int HandlerForAge(LPVOID me, CString strLayer, int effectNo, int frameIdx, int frameLength, int totalidx, int reversetotalidx, float* in, int* out, LPVOID pAgent);
	static int HandlerForInputVideo(LPVOID me, CString strLayer, int effectNo, int frameIdx, int frameLength, int totalidx, int reversetotalidx, float* in, int* out, LPVOID pAgent);
	static void TimedMessageBox(HWND hWnd, CString strTitle, CString strMessage, UINT uiTime, UINT nFlags = MB_OK, UINT defaultReturn = IDOK, BOOL bShowStatus = FALSE);
	void PrintLog(CString strLog, CString strTitle = BLANK_STRING, BOOL bUseMessageBox = FALSE);

	void UpdateImageButtons(void);
	BOOL UpdateData(BOOL bSaveAndValidate);

	void SetChangeMark(BOOL bChange);
	BOOL GetChangeMark(void);
	BOOL Load(CString& strFileName);
	BOOL LoadFile(CString& strFileName);

	BOOL Save(CString& strFileName);
	BOOL SaveFile(CString& strFileName);
	BOOL SaveAsFile(CString& strFileName);

	void KillTimersForDisplay(void);
	void HDdaVinciClear(void);

	void AutoSync(void);

	BOOL SynchronizeFromSNTP(CString strServerName);

	BOOL OpenFpg(CString strSceneName, int nPage, CString strPath = GetMirDir());

	void ClockMoveUp();
	void ClockMoveLeft();
	void ClockMoveDown();
	void ClockMoveRight();
	void ClockMove(DIRECTION dir);

	void ClockResetPos();

	void DeleteDateFormat();
	void InsertDateFormat();

	void GetDateForClock(CTimeString& timeString, CString& strDate);
	void GetHourForClock(CTimeString& timeString, CString& strHour);
	void GetOrigPosClockObjects();
	void SetPosClockObjects();
	BOOL DisplayClock(void);
	BOOL TakeOutClock(void);

	void BigClockMoveUp();
	void BigClockMoveLeft();
	void BigClockMoveDown();
	void BigClockMoveRight();
	void BigClockMove(DIRECTION dir);

	void BigClockResetPos();

	void GetOrigPosBigClockObjects(void);
	void SetPosBigClockObjects(void);
	BOOL DisplayBigClock(void);

    static UINT DisplayBigClockThread(LPVOID pParam);
	BOOL TakeOutBigClock(void);

	static UINT DisplayLogoThread(LPVOID pParam);
	BOOL TakeOutLogo();
	void LogoMoveUp();
	void LogoMoveLeft();
	void LogoMoveDown();
	void LogoMoveRight();
	void LogoMove(DIRECTION dir);

	void LogoResetPos();

	void SetPosLogoObjects();
	BOOL DisplayStationLogo(void);

	void ClockSnapSave(INT nSnapNumber);
	void ClockSnapLoad(INT nSnapNumber);
	void BigClockSnapSave(INT nSnapNumber);
	void BigClockSnapLoad(INT nSnapNumber);
	void LogoSnapLoad(INT nSnapNumber);
	void LogoSnapSave(INT nSnapNumber);
	void LiveVideoSnapSave(INT nSnapNumber);

	void ChangeNewsTextColor(void);
	void ChangeReBackColor(void);

	void OpenLogFile(void);
	void ApplyRichEditFontAndColorFromFpg(CString strFileName);
	void LoadRichEditFontAndColor(void);

	int GetHanEngMode(void);
	void SetHangulMode(void);
	void SetEnglishMode(void);

	void TrackMenuOpen(UINT nID, SS_RBUTTON* ss_rbutton);

	BOOL AddNewsCategory(CString strCategory);
	BOOL DeleteNewsCategoryFromCombo(void);

	void AddNewsData(CNewsData& newsData);
	void ClearRichEdit();
	void SendNewsDataFromRichEdit();
	BOOL ImportImage();
	void InsertImageShortCut(INT nImageNumber);
	void SpecialChar();
	void ReloadNewsScrollTemplate(void);
	void SelectChangeNewsScrollTemplate(void);

	void ReplaceNewsData(CNewsData& NewsData, CString strLayerAlias, CString strAlias, INT nPage);
	BOOL DisplayNews(BOOL bFirst);
	BOOL TakeOutNews();
	
	static UINT DisplayNewsThread(LPVOID pParam);
	BOOL TakeInNewsScroll(void);
	BOOL TakeOutNewsScroll(BOOL bPrepare);
	BOOL PrepareNewsScroll();

	void LoadWeatherCity(void);

	void UpdateWeatherTime(void);

	void GetWeatherAndAirData(void);

	BOOL RunWeatherThread(void);
	BOOL DisplayWeather(BOOL bFirst);

	BOOL TakeOutWeather(void);
	static UINT DisplayWeatherThread(LPVOID pParam);
	void ReloadNoticeTemplate(void);

	BOOL SelectNotice(CNoticeData& noticeData);
	BOOL ClearNotice(CNoticeData& noticeData);
	void ApplyNotice(void);


	void EnableClockWindow(void);
	void EnableBigClockWindow(void);
	void EnableLogoWindow(void);
	void EnableManualUpWindow(void);
	void EnableAgeWindow(BOOL bChecked);
	void EnableNoticeWindow(BOOL bChecked);
	void EnableLiveVideoWindow(BOOL bChecked);
	void EnableWeatherWindow(BOOL bChecked);
	void EnableNewsScrollWindow(BOOL bChecked);
	void EnableNewsWindow(BOOL bChecked);
	void EnableWindow(UINT nID, BOOL bChecked);


	BOOL DispAge(void);
	BOOL StartDisplayNotice(BOOL bUseAge);

	static UINT DisplayNoticeThread(LPVOID pParam);
	BOOL StartAge(BOOL bOnlyAge);
	void SetAgeImageFile(CCGXObject* pObject);
	void SetAgePos(CCGXObject* pObject);
	void DrawAgeTime(CWnd* pWnd, CString strAgeTime);
	BOOL CalcAgeShow(void);

	BOOL SearchAndModifyFileName(CString& strFileName, CString strPath);

	BOOL DisplayNotice(BOOL bFirst);
	BOOL TakeInNotice(INT nIndex);
	BOOL TakeOutNotice();
	BOOL TakeOutAge();

	void SelectChangeManualUpTemplate(void);
	void ReloadManualUpTemplate(void);

	BOOL PrevManualUp(void);
	BOOL NextManualUp(void);

	BOOL DispManualUp(void);
	BOOL TakeOutManualUp(void);

	void GetOrigPosLiveVideoObjects(void);
	void SetPosLiveVideoObjects(void);
	void LiveVideoResetPos();
	void LiveVideoMoveUp();
	void LiveVideoMoveLeft();
	void LiveVideoMoveDown();
	void LiveVideoMoveRight();
	void LiveVideoMove(DIRECTION dir);

	BOOL DispLiveVideo(void);
	BOOL TakeOutLiveVideo(void);

	void DrawTextWithDoubleBuffering(CWnd* pWnd, CString strText);

	INT LoadFromFPG(CString strFileName, int nPage);

	LRESULT SpreadLButtonClicked(UINT nID, INT nCol, INT nRow);
		
	void SelectChangeLiveVideoTemplate(void);
	void ReloadLiveVideoTemplate(void);

	void CancelPrepareNewsScroll(void);

	CCGXObject* m_pAgeObject;

	protected:
	
	virtual void DoDataExchange(CDataExchange* pDX) override;	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnSuperInOut(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT OnHDdaVinciClear(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT OnPrintLog(WPARAM wParam = 0, LPARAM lParam = 0);


	afx_msg LRESULT OnDispCheck(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT OnApplyNotice(WPARAM wParam = 0, LPARAM lParam = 0);

	static UINT DisplayClockThread(LPVOID pParam);

	afx_msg void OnBnClickedButtonSync();
	afx_msg void OnCbnSelchangeComboTimeServer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnNewsScrollFinish(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT OnNoticeFinish(WPARAM wParam = 0, LPARAM lParam = 0);

	afx_msg void OnBnClickedCheckAutoSync();
	afx_msg void OnCbnSelchangeComboInterval();
	afx_msg void OnEnKillfocusEditNtpAdjustTime();

	afx_msg LRESULT OnSpreadEditModeOff(WPARAM, LPARAM); // 편집이 다되었으면 메세지가 나온다 WPARAM: Spread ID LPARAM: SS_CELLCOORD 구조체
	afx_msg LRESULT OnSpreadRButtonClicked(WPARAM wPararm = 0, LPARAM lParam = 0);

	afx_msg LRESULT OnSpreadDBClicked(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT OnSpreadLButtonClicked(WPARAM wParam = 0, LPARAM lParam = 0);
	afx_msg LRESULT OnSpreadComboSelChange(WPARAM wParam = 0, LPARAM lParam = 0);

	afx_msg void OnSpreadSortAsc();
	afx_msg void OnSpreadSortDesc();
	afx_msg void OnSpreadInsertRow();
	afx_msg void OnSpreadDeleteRow();
	afx_msg void OnSpreadAllUse();
	afx_msg void OnSpreadAllDeselectUse();
	afx_msg void OnSpreadInvertUse();
	afx_msg void OnSpreadClearSheet();
	afx_msg void OnSpreadRestoreSheet();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedCheckDispClock();
	afx_msg void OnBnClickedCheckClockEntireMove();
	afx_msg void OnBnClickedButtonClockUp();
	afx_msg void OnEnKillfocusEditClockMoveSize();
	afx_msg void OnEnKillfocusEditClockSecOver();
	afx_msg void OnEnKillfocusEditClockSecUnder();
	afx_msg void OnBnClickedCheckClockMoveEnable();
	afx_msg void OnBnClickedButtonClockLeft();
	afx_msg void OnBnClickedButtonClockDown();
	afx_msg void OnBnClickedButtonClockRight();
	afx_msg void OnBnClickedButtonClockResetPos();
	afx_msg void OnBnClickedButtonDeleteDateFormat();
	afx_msg void OnCbnKillfocusComboDateFormat();
	afx_msg void OnCbnSelchangeComboDateFormat();
	afx_msg void OnBnClickedCheckClock24Hour();
	afx_msg void OnBnClickedCheckClockDispSecond();
	afx_msg void OnBnClickedCheckClockDispEveryHourSec();
	afx_msg void OnBnClickedCheckClockHideFirstDigit0OfHour();

	afx_msg void OnBnClickedButtonClockSnapSave1();
	afx_msg void OnBnClickedButtonClockSnapSave2();
	afx_msg void OnBnClickedButtonClockSnapSave3();



	afx_msg void OnBnClickedButtonClockSnapLoad1();
	afx_msg void OnBnClickedButtonClockSnapLoad2();
	afx_msg void OnBnClickedButtonClockSnapLoad3();
	afx_msg void OnBnClickedCheckClockDateMove();
	afx_msg void OnBnClickedCheckClockTimeMove();
	afx_msg void OnBnClickedCheckBigClockMoveEnable();
	afx_msg void OnEnKillfocusEditBigClockMoveSize();
	afx_msg void OnBnClickedCheckDispBigClock();
	afx_msg void OnBnClickedButtonBigClockUp();
	afx_msg void OnBnClickedButtonBigClockRight();
	afx_msg void OnBnClickedButtonBigClockDown();
	afx_msg void OnBnClickedButtonBigClockLeft();
	afx_msg void OnBnClickedButtonBigClockResetPos();

	afx_msg void OnBnClickedButtonBigClockSnapSave1();
	afx_msg void OnBnClickedButtonBigClockSnapSave2();
	afx_msg void OnBnClickedButtonBigClockSnapSave3();

	afx_msg void OnBnClickedButtonBigClockSnapLoad1();
	afx_msg void OnBnClickedButtonBigClockSnapLoad2();
	afx_msg void OnBnClickedButtonBigClockSnapLoad3();
	afx_msg void OnBnClickedCheckLogoEntireMove();
	afx_msg void OnBnClickedCheckLogoMove();
	afx_msg void OnBnClickedCheckLogoLiveMove();
	afx_msg void OnBnClickedCheckLogoUhdMove();
	afx_msg void OnBnClickedCheckLogoDongsiMove();
	afx_msg void OnBnClickedCheckLogoEntireShow();
	afx_msg void OnBnClickedCheckLogoShow();
	afx_msg void OnBnClickedCheckLogoLiveShow();
	afx_msg void OnBnClickedCheckLogoUhdShow();
	afx_msg void OnBnClickedCheckLogoDongsiShow();
	afx_msg void OnBnClickedRadioLogoKbs1();
	afx_msg void OnBnClickedRadioLogoKbs2();
	afx_msg void OnBnClickedRadioLogoKbsLocal();
	afx_msg void OnBnClickedCheckDispStationLogo();
	afx_msg void OnEnKillfocusEditLogoMoveSize();
	afx_msg void OnBnClickedButtonLogoSnapSave1();
	afx_msg void OnBnClickedButtonLogoSnapSave2();
	afx_msg void OnBnClickedButtonLogoSnapSave3();

	afx_msg void OnBnClickedButtonLogoSnapLoad1();
	afx_msg void OnBnClickedButtonLogoSnapLoad2();
	afx_msg void OnBnClickedButtonLogoSnapLoad3();
	afx_msg void OnBnClickedButtonLogoResetPos();
	afx_msg void OnBnClickedButtonLogoUp();
	afx_msg void OnBnClickedButtonLogoRight();
	afx_msg void OnBnClickedButtonLogoDown();
	afx_msg void OnBnClickedButtonLogoLeft();
	afx_msg void OnCbnSelchangeComboNewsCategory();
	afx_msg void OnCbnKillfocusComboNewsCategory();
	afx_msg void OnCbnSelchangeComboNewsTextFont();
	afx_msg void OnCbnSelchangeComboNewsTextSize();
	afx_msg void OnCbnKillfocusComboNewsTextSize();
	afx_msg void OnBnClickedButtonDeleteNewsCategoryFromCombo();
	afx_msg void OnBnClickedButtonDeleteNewsTextSizeFromCombo();
	afx_msg void OnCbnKillfocusComboNewsTextFont();
	afx_msg void OnEnKillfocusEditNewsSymbolSize();
	afx_msg void OnKillfocusEditNewsHanjaSize();
	afx_msg void OnBnClickedButtonNewsTextColor();
    afx_msg void OnHotitemchangeButtonNewsTextColor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnHotItemChangeButtonReBackColor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonReBackColor();
	afx_msg void OnBnClickedButtonFileLoad();
	afx_msg void OnBnClickedButtonFileSave();
	afx_msg void OnBnClickedButtonFileSaveAs();
	afx_msg void OnBnClickedButtonOpenLogFolder();
	afx_msg void OnBnClickedButtonOpenLogFile();
	afx_msg void OnBnClickedButtonClearLogMessage();
	afx_msg void OnBnClickedCheckApplyScrollColor();
	afx_msg void OnDestroy();

	afx_msg void OnSelchangeRicheditNewsData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonInsertImage();
	afx_msg void OnBnClickedButtonImageShortcutConfig();
	afx_msg void OnBnClickedButtonImageShortcut1();
	afx_msg void OnBnClickedButtonImageShortcut2();
	afx_msg void OnBnClickedButtonImageShortcut3();
	afx_msg void OnBnClickedButtonImageShortcut4();
	afx_msg void OnBnClickedButtonImageShortcut5();
	afx_msg void OnBnClickedButtonImageShortcut6();
	afx_msg void OnBnClickedButtonNewsClear();
	afx_msg void OnBnClickedButtonNewsSpecialChar();
	afx_msg void OnBnClickedButtonSpellCheck();
	afx_msg void OnBnClickedButtonReloadNewsScrollTemplate();
	afx_msg void OnCbnSelchangeComboNewsScrollTemplate();
	afx_msg void OnKillfocusEditNewsScrollIteration();
	afx_msg void OnKillfocusEditNewsScrollItemGap();
	afx_msg void OnKillfocusEditNewsScrollPrefixGap();
	afx_msg void OnKillfocusEditNewsScrollSpeed();
	afx_msg void OnBnClickedCheckDispNews();
	afx_msg void OnEnKillfocusEditNewsTime();
	afx_msg void OnBnClickedCheckPrepareNewsScroll();

	afx_msg void OnBnClickedCheckDispNewsScroll();
	
	afx_msg void OnBnClickedButtonWeatherCitySelect();

	afx_msg void OnBnClickedButtonGetKbsWeather();
	afx_msg void OnBnClickedButtonClearWeatherData();
	afx_msg void OnBnClickedButtonClearAllWeatherData();
	afx_msg void OnBnClickedCheckAutoWeatherReading();
	afx_msg void OnEnKillfocusEditAutoReadingWeatherPeriodMin();
	afx_msg void OnBnClickedCheckDispWeather();
	afx_msg void OnBnClickedButtonWeatherMoveUp();
	afx_msg void OnBnClickedButtonWeatherMoveDown();
	afx_msg void OnBnClickedButtonNewsMoveUp();
	afx_msg void OnBnClickedButtonNewsMoveDown();
	afx_msg void OnBnClickedButtonReloadNoticeTemplate();
	afx_msg void OnBnClickedButtonApplyNotice();
	afx_msg void OnBnClickedRadioAgeAll();
	afx_msg void OnBnClickedRadioAge7();
	afx_msg void OnBnClickedRadioAge12();
	afx_msg void OnBnClickedRadioAge15();
	afx_msg void OnBnClickedRadioAge19();

	afx_msg void OnBnClickedCheckDispAge();
	afx_msg void OnBnClickedCheckDispNotice();
	afx_msg void OnEnKillfocusEditNoticeRepetition();
	afx_msg void OnBnClickedCheckDispAgeNotice();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonNoticeMoveUp();
	afx_msg void OnBnClickedButtonNoticeMoveDown();

	afx_msg void OnCbnSelchangeComboManualupTemplates();
	afx_msg void OnBnClickedButtonReloadManualupTemplate();

	afx_msg void OnBnClickedCheckDispManualUp();
	afx_msg void OnBnClickedButtonManualupPrev();
	afx_msg void OnBnClickedButtonManualupNext();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButtonLoadFpr();

	afx_msg void OnBnClickedCheckDispLiveVideo();
	afx_msg void OnBnClickedCheckLiveVideoMoveEnable();

	afx_msg void OnBnClickedButtonLiveVideoResetPos();
	afx_msg void OnBnClickedButtonLiveVideoUp();
	afx_msg void OnBnClickedButtonLiveVideoLeft();
	afx_msg void OnBnClickedButtonLiveVideoDown();
	afx_msg void OnBnClickedButtonLiveVideoRight();
	afx_msg void OnEnKillfocusEditLiveVideoMoveSize();

	afx_msg void OnBnClickedButtonLiveVideoSnapSave1();
	afx_msg void OnBnClickedButtonLiveVideoSnapSave2();
	afx_msg void OnBnClickedButtonLiveVideoSnapSave3();
	afx_msg void OnBnClickedButtonLiveVideoSnapLoad1();
	afx_msg void OnBnClickedButtonLiveVideoSnapLoad2();
	afx_msg void OnBnClickedButtonLiveVideoSnapLoad3();
	afx_msg void OnCbnSelchangeComboLiveVideoTemplates();
	afx_msg void OnBnClickedButtonReloadLiveVideoTemplate();
	afx_msg void OnEnKillfocusEditWeatherDisplayDuration();
	afx_msg void OnBnClickedRadioLogoKbsAnimation();
	afx_msg void OnBnClickedButtonDateFormat();
	BOOL Unlock_Activation();
	afx_msg void OnBnClickedButton1();
};
