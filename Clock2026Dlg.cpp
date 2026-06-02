// Clock2026Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Clock2026.h"
#include "Clock2026Dlg.h"
#include "afxdialogex.h"
#include "SNTP.h"
#include "ImageConfigDlg.h"

#include "SpellCheck.h"
#include "WeatherSelectDlg.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR

#define _TAKE_IN_LOCK_ 0
#define _TIMER_TRANS_MODE_ 0
//#define _THREAD_TRANS_MODE_ 0

#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CClock2026Dlg dialog

CClock2026Dlg::CClock2026Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOCK2026_DIALOG, pParent)
	, m_pMutexSNTP(std::make_unique<std::mutex>())
	, m_pMutexReadWeather(std::make_unique<std::mutex>())
	, m_pMutexHandler(std::make_unique<std::mutex>())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pHDdaVinci = theApp.GetHDdaVinci();
	m_pStatus = theApp.GetStatus();
	m_btnSuperInOut.SetHDdaVinci(m_pHDdaVinci);

	m_pLiveVideo = nullptr;

	InitializeMutexes();

	InitValues();

}

void CClock2026Dlg::DoDataExchange(CDataExchange* pDX)
{
	INT nVal;
	BOOL bVal;
	CString strVal;

	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SUPER, m_btnSuperInOut);
	DDX_Control(pDX, IDC_LIST_LOG_MESSAGE, m_ListBoxLog);

	nVal = m_pStatus->NTPConf()->ServerIndex();
	DDX_CBIndex(pDX, IDC_COMBO_TIME_SERVER, nVal);

	bVal = m_pStatus->NTPConf()->AutoSync();
	DDX_Check(pDX, IDC_CHECK_AUTO_SYNC, bVal);
	
	strVal = m_pStatus->NTPConf()->Interval();
	DDX_Text(pDX, IDC_COMBO_INTERVAL, strVal);
	
	strVal = m_pStatus->NTPConf()->AdjustTime();
	DDX_Text(pDX, IDC_EDIT_NTP_ADJUST_TIME, strVal);

	bVal = m_pStatus->ClockConf()->MoveEnable();
	DDX_Check(pDX, IDC_CHECK_CLOCK_MOVE_ENABLE, bVal);

	nVal = m_pStatus->ClockConf()->FormatIndex();
	DDX_CBIndex(pDX, IDC_COMBO_DATE_FORMAT, nVal);

	strVal = m_pStatus->ClockConf()->Format();
	DDX_Text(pDX, IDC_COMBO_DATE_FORMAT, strVal);

	bVal = m_pStatus->ClockConf()->EntireMove();
	DDX_Check(pDX, IDC_CHECK_CLOCK_ENTIRE_MOVE, bVal);

	bVal = m_pStatus->ClockConf()->DateMove();
	DDX_Check(pDX, IDC_CHECK_CLOCK_DATE_MOVE, bVal);

	bVal = m_pStatus->ClockConf()->TimeMove();
	DDX_Check(pDX, IDC_CHECK_CLOCK_TIME_MOVE, bVal);

	nVal = m_pStatus->ClockConf()->MovingSize();
	DDX_Text(pDX, IDC_EDIT_CLOCK_MOVE_SIZE, nVal);

	bVal = m_pStatus->ClockConf()->B24Hour();
	DDX_Check(pDX, IDC_CHECK_CLOCK_24_HOUR, bVal);

	bVal = m_pStatus->ClockConf()->DispSecond();
	DDX_Check(pDX, IDC_CHECK_CLOCK_DISP_SECOND, bVal);

	bVal = m_pStatus->ClockConf()->DispEveryHourSecond();
	DDX_Check(pDX, IDC_CHECK_CLOCK_DISP_EVERY_HOUR_SEC, bVal);

	strVal = m_pStatus->ClockConf()->SecOver();
	DDX_Text(pDX, IDC_EDIT_CLOCK_SEC_OVER, strVal);

	strVal = m_pStatus->ClockConf()->SecUnder();
	DDX_Text(pDX, IDC_EDIT_CLOCK_SEC_UNDER, strVal);

	bVal = m_pStatus->ClockConf()->HideFirstDigit0OfHour();
	DDX_Check(pDX, IDC_CHECK_CLOCK_HIDE_FIRST_DIGIT_0_OF_HOUR, bVal);

	bVal = m_pStatus->BigClockConf()->MoveEnable();
	DDX_Check(pDX, IDC_CHECK_BIG_CLOCK_MOVE_ENABLE, bVal);

	nVal = m_pStatus->BigClockConf()->MovingSize();
	DDX_Text(pDX, IDC_EDIT_BIG_CLOCK_MOVE_SIZE, nVal);

	bVal = m_pStatus->LogoConf()->EntireMove();
	DDX_Check(pDX, IDC_CHECK_LOGO_ENTIRE_MOVE, bVal);

	bVal = m_pStatus->LogoConf()->LogoMove();
	DDX_Check(pDX, IDC_CHECK_LOGO_MOVE, bVal);

	bVal = m_pStatus->LogoConf()->LiveMove();
	DDX_Check(pDX, IDC_CHECK_LOGO_LIVE_MOVE, bVal);

	bVal = m_pStatus->LogoConf()->UHDMove();
	DDX_Check(pDX, IDC_CHECK_LOGO_UHD_MOVE, bVal);

	bVal = m_pStatus->LogoConf()->DongSiMove();
	DDX_Check(pDX, IDC_CHECK_LOGO_DONGSI_MOVE, bVal);

	bVal = m_pStatus->LogoConf()->EntireShow();
	DDX_Check(pDX, IDC_CHECK_LOGO_ENTIRE_SHOW, bVal);

	bVal = m_pStatus->LogoConf()->LogoShow();
	DDX_Check(pDX, IDC_CHECK_LOGO_SHOW, bVal);

	bVal = m_pStatus->LogoConf()->LiveShow();
	DDX_Check(pDX, IDC_CHECK_LOGO_LIVE_SHOW, bVal);

	bVal = m_pStatus->LogoConf()->UHDShow();
	DDX_Check(pDX, IDC_CHECK_LOGO_UHD_SHOW, bVal);

	bVal = m_pStatus->LogoConf()->DongSiShow();
	DDX_Check(pDX, IDC_CHECK_LOGO_DONGSI_SHOW, bVal);

	nVal = m_pStatus->LogoConf()->KBSLogo();
	DDX_Radio(pDX, IDC_RADIO_LOGO_KBS1, nVal);

	nVal = m_pStatus->LogoConf()->MovingSize();
	DDX_Text(pDX, IDC_EDIT_LOGO_MOVE_SIZE, nVal);

	bVal = m_pStatus->LiveVideoConf()->MoveEnable();
	DDX_Check(pDX, IDC_CHECK_LIVE_VIDEO_MOVE_ENABLE, bVal);

	nVal = m_pStatus->LiveVideoConf()->MovingSize();
	DDX_Text(pDX, IDC_EDIT_LIVE_VIDEO_MOVE_SIZE, nVal);

	nVal = m_pStatus->LiveVideoConf()->Offset().X;
	DDX_Text(pDX, IDC_EDIT_LIVE_VIDEO_OFFSET_X, nVal);

	nVal = m_pStatus->LiveVideoConf()->Offset().Y;
	DDX_Text(pDX, IDC_EDIT_LIVE_VIDEO_OFFSET_Y, nVal);

	DDX_Control(pDX, IDC_RICHEDIT_NEWS_DATA, m_RENewsData);

	strVal = m_pStatus->NewsConf()->Category();
	DDX_Text(pDX, IDC_COMBO_NEWS_CATEGORY, strVal);

	DDX_Control(pDX, IDC_COMBO_NEWS_TEXT_FONT, m_ComboNewsTextFont);

	strVal = m_pStatus->NewsConf()->DefaultFontName();
	DDX_Text(pDX, IDC_COMBO_NEWS_TEXT_FONT, strVal);

	strVal = m_pStatus->NewsConf()->TextSize();
	DDX_Text(pDX, IDC_COMBO_NEWS_TEXT_SIZE, strVal);

	strVal = m_pStatus->NewsConf()->SymbolSize();
	DDX_Text(pDX, IDC_EDIT_NEWS_SYMBOL_SIZE, strVal);

	strVal = m_pStatus->NewsConf()->HanjaSize();
	DDX_Text(pDX, IDC_EDIT_NEWS_HANJA_SIZE, strVal);

	DDX_Control(pDX, IDC_BUTTON_NEWS_TEXT_COLOR, m_btnNewsTextColor);
	DDX_Control(pDX, IDC_BUTTON_RE_BACK_COLOR, m_btnREBackColor);

	bVal = m_pStatus->NewsConf()->ApplyScrollColor();
	DDX_Check(pDX, IDC_CHECK_APPLY_SCROLL_COLOR, bVal);

	DDX_Control(pDX, IDC_BUTTON_IMAGE_SHORTCUT_1, m_btnImageShortCut[IMAGE_SHORTCUT_1]);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_SHORTCUT_2, m_btnImageShortCut[IMAGE_SHORTCUT_2]);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_SHORTCUT_3, m_btnImageShortCut[IMAGE_SHORTCUT_3]);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_SHORTCUT_4, m_btnImageShortCut[IMAGE_SHORTCUT_4]);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_SHORTCUT_5, m_btnImageShortCut[IMAGE_SHORTCUT_5]);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_SHORTCUT_6, m_btnImageShortCut[IMAGE_SHORTCUT_6]);

	DDX_Control(pDX, IDC_STATIC_NEWS_SCROLL_PREVIEW, m_PreviewNewsScroll);

	strVal = m_pStatus->NewsConf()->ScrollTemplate();
	DDX_Text(pDX, IDC_COMBO_NEWS_SCROLL_TEMPLATE, strVal);

	nVal = m_pStatus->NewsConf()->ScrollIteration();
	DDX_Text(pDX, IDC_EDIT_NEWS_SCROLL_ITERATION, nVal);
	DDV_MinMaxInt(pDX, IDC_EDIT_NEWS_SCROLL_ITERATION, MIN_NEWS_SCROLL_ITERATION, MAX_NEWS_SCROLL_ITERATION);

	nVal = m_pStatus->NewsConf()->ScrollItemGap();
	DDX_Text(pDX, IDC_EDIT_NEWS_SCROLL_ITEM_GAP, nVal);
	DDV_MinMaxInt(pDX, IDC_EDIT_NEWS_SCROLL_ITEM_GAP, MIN_NEWS_SCROLL_ITEM_GAP, MAX_NEWS_SCROLL_ITEM_GAP);


	nVal = m_pStatus->NewsConf()->ScrollPrefixGap();
	DDX_Text(pDX, IDC_EDIT_NEWS_SCROLL_PREFIX_GAP, nVal);
	DDV_MinMaxInt(pDX, IDC_EDIT_NEWS_SCROLL_PREFIX_GAP, MIN_NEWS_SCROLL_PREFIX_GAP, MAX_NEWS_SCROLL_PREFIX_GAP);

	nVal = m_pStatus->NewsConf()->ScrollSpeed();
	DDX_Text(pDX, IDC_EDIT_NEWS_SCROLL_SPEED, nVal);
	DDV_MinMaxInt(pDX, IDC_EDIT_NEWS_SCROLL_SPEED, MIN_NEWS_SCROLL_SPEED, MAX_NEWS_SCROLL_SPEED);

	nVal = m_pStatus->NewsConf()->DisplayDuration();
	DDX_Text(pDX, IDC_EDIT_NEWS_DISPLAY_DURATION, nVal);
	DDV_MinMaxInt(pDX, IDC_EDIT_NEWS_DISPLAY_DURATION, MIN_NEWS_DISPLAY_DURATION, MAX_NEWS_DISPLAY_DURATION);


	nVal = m_pStatus->WeatherConf()->DisplayDuration();
	DDX_Text(pDX, IDC_EDIT_WEATHER_DISPLAY_DURATION, nVal);
	DDV_MinMaxInt(pDX, IDC_EDIT_WEATHER_DISPLAY_DURATION, MIN_WEATHER_DISPLAY_DURATION, MAX_WEATHER_DISPLAY_DURATION);

	nVal = m_pStatus->WeatherConf()->ExceptAir();
	DDX_Check(pDX, IDC_CHECK_EXCEPT_AIR, nVal);


	DDX_Control(pDX, IDC_CHECK_DISP_CLOCK, m_btnDispClock);
	DDX_Control(pDX, IDC_CHECK_DISP_BIG_CLOCK, m_btnDispBigClock);
	DDX_Control(pDX, IDC_CHECK_DISP_STATION_LOGO, m_btnDispLogo);
	DDX_Control(pDX, IDC_CHECK_DISP_NEWS, m_btnDispNews);
	DDX_Control(pDX, IDC_CHECK_DISP_NEWS_SCROLL, m_btnDispNewsScroll);
	DDX_Control(pDX, IDC_CHECK_PREPARE_NEWS_SCROLL, m_btnNewsScrollPrepare);

	DDX_Check(pDX, IDC_CHECK_PREPARE_NEWS_SCROLL, m_bPrepareNewsScroll);

	DDX_Text(pDX, IDC_STATIC_NEWS_SCROLL_COUNTER, m_nNewScrollCounter);

	bVal = m_pStatus->WeatherConf()->AutoReading();
	DDX_Check(pDX, IDC_CHECK_AUTO_WEATHER_READING, bVal);

	nVal = m_pStatus->WeatherConf()->AutoReadingPeriod();
	DDX_Text(pDX, IDC_EDIT_AUTO_READING_WEATHER_PERIOD_MIN, nVal);
	DDV_MinMaxInt(pDX,nVal, 20, 60);

	DDX_Text(pDX, IDC_STATIC_NOTICE_COUNTER, m_nNoticeCounter);

	DDX_Control(pDX, IDC_STATIC_NOTICE_PREVIEW, m_PreviewNotice);
		
	DDX_Text(pDX,IDC_STATIC_NOTICE_COUNTER, m_nNoticeCounter);

	nVal = m_pStatus->NoticeConf()->Repetition();
	DDX_Text(pDX, IDC_EDIT_NOTICE_REPETITION, nVal);
		
	nVal = m_pStatus->NoticeConf()->AgeMode();
	DDX_Radio(pDX, IDC_RADIO_AGE_ALL, nVal);

	DDX_Control(pDX, IDC_CHECK_DISP_AGE, m_btnDispAge);
	DDX_Control(pDX, IDC_CHECK_DISP_NOTICE, m_btnDispNotice);
	DDX_Control(pDX, IDC_CHECK_DISP_AGE_NOTICE, m_btnDispAgeNotice);


	strVal = m_pStatus->ManualUpConf()->ManualUpTemplate();
	DDX_Text(pDX, IDC_COMBO_MANUALUP_TEMPLATES, strVal);

	DDX_Control(pDX, IDC_CHECK_DISP_MANUALUP, m_btnDispManualUp);


	strVal = m_pStatus->LiveVideoConf()->Template();
	DDX_Text(pDX, IDC_COMBO_LIVE_VIDEO_TEMPLATES, strVal);

	DDX_Control(pDX, IDC_CHECK_DISP_LIVE_VIDEO, m_btnDispLiveVideo);

	/*========================== DISP ===========================================*/

	DDX_Control(pDX, IDC_CHECK_DISP_CLOCK, m_btnDispClock);
	DDX_Control(pDX, IDC_CHECK_DISP_BIG_CLOCK, m_btnDispBigClock);
	DDX_Control(pDX, IDC_CHECK_DISP_STATION_LOGO, m_btnDispLogo);
	DDX_Control(pDX, IDC_CHECK_DISP_NEWS, m_btnDispNews);
	DDX_Control(pDX, IDC_CHECK_DISP_NEWS_SCROLL, m_btnDispNewsScroll);
	DDX_Control(pDX, IDC_CHECK_PREPARE_NEWS_SCROLL, m_btnNewsScrollPrepare);

	DDX_Control(pDX, IDC_CHECK_DISP_WEATHER, m_btnDispWeather);

	nVal = m_pStatus->GetDispState(DISP_UPPER_CLOCK);
	DDX_Check(pDX, IDC_CHECK_DISP_CLOCK, nVal);

	nVal = m_pStatus->GetDispState(DISP_BIG_CLOCK);
	DDX_Check(pDX, IDC_CHECK_DISP_BIG_CLOCK, nVal);

	nVal = m_pStatus->GetDispState(DISP_STATION_LOGO);
	DDX_Check(pDX, IDC_CHECK_DISP_STATION_LOGO, nVal);

	nVal = m_pStatus->GetDispState(DISP_MANUALUP);
	DDX_Check(pDX, IDC_CHECK_DISP_MANUALUP, nVal);

	nVal = m_pStatus->GetDispState(DISP_NEWS);
	DDX_Check(pDX, IDC_CHECK_DISP_NEWS, nVal);
	
	DDX_Check(pDX, IDC_CHECK_PREPARE_NEWS_SCROLL, m_bPrepareNewsScroll);

	nVal = m_pStatus->GetDispState(DISP_NEWS_SCROLL);
	DDX_Check(pDX, IDC_CHECK_DISP_NEWS_SCROLL, nVal);

	nVal = m_pStatus->GetDispState(DISP_WEATHER);
	DDX_Check(pDX, IDC_CHECK_DISP_WEATHER, nVal);

	nVal = m_pStatus->GetDispState(DISP_AGE);
	DDX_Check(pDX, IDC_CHECK_DISP_AGE, nVal);

	nVal = m_pStatus->GetDispState(DISP_NOTICE);
	DDX_Check(pDX, IDC_CHECK_DISP_NOTICE, nVal);

	nVal = m_pStatus->GetDispState(DISP_AGE_NOTICE);
	DDX_Check(pDX, IDC_CHECK_DISP_AGE_NOTICE, nVal);

	DDX_Text(pDX, IDC_STATIC_NOTICE_COUNTER, m_nCurNoticeCount);

	DDX_Control(pDX, IDC_STATIC_LIVE_VIDEO_INPUT_PREVIEW, m_PreviewLiveVideo);

	DDX_Check(pDX, IDC_CHECK_DISP_LIVE_VIDEO, m_pStatus->m_bDispState[DISP_LIVE_VIDEO]);

}

BEGIN_MESSAGE_MAP(CClock2026Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_SUPER_INOUT, OnSuperInOut)
	ON_MESSAGE(WM_HDDAVINCI_CLEAR, OnHDdaVinciClear)
	ON_MESSAGE(WM_PRINT_LOG,OnPrintLog)
	ON_MESSAGE(WM_DISP_CHECK, OnDispCheck)

	ON_MESSAGE(WM_NEWS_SCROLL_FINISH, OnNewsScrollFinish)
	ON_MESSAGE(WM_NOTICE_FINISH, OnNoticeFinish)

	ON_MESSAGE(WM_APPLY_NOTICE,OnApplyNotice)

	ON_MESSAGE(SSM_EDITMODEOFF, &CClock2026Dlg::OnSpreadEditModeOff)
	ON_MESSAGE(SSM_RBUTTON, &CClock2026Dlg::OnSpreadRButtonClicked)
	ON_MESSAGE(SSM_CLICK, &CClock2026Dlg::OnSpreadLButtonClicked)
	ON_MESSAGE(SSM_DBLCLK, &CClock2026Dlg::OnSpreadDBClicked)
	ON_MESSAGE(SSM_COMBOSELCHANGE, &CClock2026Dlg::OnSpreadComboSelChange)

	ON_COMMAND(ID_SPREAD_SORT_ASC, &CClock2026Dlg::OnSpreadSortAsc)
	ON_COMMAND(ID_SPREAD_SORT_ASC, &CClock2026Dlg::OnSpreadSortAsc)
	ON_COMMAND(ID_SPREAD_SORT_DESC, &CClock2026Dlg::OnSpreadSortDesc)
	ON_COMMAND(ID_SPREAD_INSERT_ROW, &CClock2026Dlg::OnSpreadInsertRow)
	ON_COMMAND(ID_SPREAD_DELETE_ROW, &CClock2026Dlg::OnSpreadDeleteRow)
	ON_COMMAND(ID_SPREAD_ALL_USE, &CClock2026Dlg::OnSpreadAllUse)
	ON_COMMAND(ID_SPREAD_ALL_DESELECT_USE, &CClock2026Dlg::OnSpreadAllDeselectUse)
	ON_COMMAND(ID_SPREAD_INVERT_USE, &CClock2026Dlg::OnSpreadInvertUse)

	ON_COMMAND(ID_SPREAD_CLEAR_SHEET, &CClock2026Dlg::OnSpreadClearSheet)
	ON_COMMAND(ID_SPREAD_RESTORE_SHEET, &CClock2026Dlg::OnSpreadRestoreSheet)

	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CClock2026Dlg::OnBnClickedButtonClear)

	ON_BN_CLICKED(IDC_BUTTON_SYNC, &CClock2026Dlg::OnBnClickedButtonSync)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_SERVER, &CClock2026Dlg::OnCbnSelchangeComboTimeServer)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_AUTO_SYNC, &CClock2026Dlg::OnBnClickedCheckAutoSync)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERVAL, &CClock2026Dlg::OnCbnSelchangeComboInterval)
	ON_EN_KILLFOCUS(IDC_EDIT_NTP_ADJUST_TIME, &CClock2026Dlg::OnEnKillfocusEditNtpAdjustTime)

	ON_BN_CLICKED(IDC_CHECK_DISP_CLOCK, &CClock2026Dlg::OnBnClickedCheckDispClock)
	ON_BN_CLICKED(IDC_CHECK_CLOCK_ENTIRE_MOVE, &CClock2026Dlg::OnBnClickedCheckClockEntireMove)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_UP, &CClock2026Dlg::OnBnClickedButtonClockUp)
	ON_EN_KILLFOCUS(IDC_EDIT_CLOCK_MOVE_SIZE, &CClock2026Dlg::OnEnKillfocusEditClockMoveSize)
	ON_EN_KILLFOCUS(IDC_EDIT_CLOCK_SEC_OVER, &CClock2026Dlg::OnEnKillfocusEditClockSecOver)
	ON_EN_KILLFOCUS(IDC_EDIT_CLOCK_SEC_UNDER, &CClock2026Dlg::OnEnKillfocusEditClockSecUnder)
	ON_BN_CLICKED(IDC_CHECK_CLOCK_MOVE_ENABLE, &CClock2026Dlg::OnBnClickedCheckClockMoveEnable)

	ON_BN_CLICKED(IDC_BUTTON_CLOCK_LEFT, &CClock2026Dlg::OnBnClickedButtonClockLeft)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_DOWN, &CClock2026Dlg::OnBnClickedButtonClockDown)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_RIGHT, &CClock2026Dlg::OnBnClickedButtonClockRight)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_RESET_POS, &CClock2026Dlg::OnBnClickedButtonClockResetPos)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_DATE_FORMAT, &CClock2026Dlg::OnBnClickedButtonDeleteDateFormat)
	ON_CBN_KILLFOCUS(IDC_COMBO_DATE_FORMAT, &CClock2026Dlg::OnCbnKillfocusComboDateFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_DATE_FORMAT, &CClock2026Dlg::OnCbnSelchangeComboDateFormat)

	ON_BN_CLICKED(IDC_CHECK_CLOCK_24_HOUR, &CClock2026Dlg::OnBnClickedCheckClock24Hour)
	ON_BN_CLICKED(IDC_CHECK_CLOCK_DISP_SECOND, &CClock2026Dlg::OnBnClickedCheckClockDispSecond)
	ON_BN_CLICKED(IDC_CHECK_CLOCK_DISP_EVERY_HOUR_SEC, &CClock2026Dlg::OnBnClickedCheckClockDispEveryHourSec)
	ON_BN_CLICKED(IDC_CHECK_CLOCK_HIDE_FIRST_DIGIT_0_OF_HOUR, &CClock2026Dlg::OnBnClickedCheckClockHideFirstDigit0OfHour)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_SNAP_SAVE_1, &CClock2026Dlg::OnBnClickedButtonClockSnapSave1)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_SNAP_SAVE_2, &CClock2026Dlg::OnBnClickedButtonClockSnapSave2)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_SNAP_SAVE_3, &CClock2026Dlg::OnBnClickedButtonClockSnapSave3)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_SNAP_LOAD_1, &CClock2026Dlg::OnBnClickedButtonClockSnapLoad1)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_SNAP_LOAD_2, &CClock2026Dlg::OnBnClickedButtonClockSnapLoad2)
	ON_BN_CLICKED(IDC_BUTTON_CLOCK_SNAP_LOAD_3, &CClock2026Dlg::OnBnClickedButtonClockSnapLoad3)
	ON_BN_CLICKED(IDC_CHECK_CLOCK_DATE_MOVE, &CClock2026Dlg::OnBnClickedCheckClockDateMove)
	ON_BN_CLICKED(IDC_CHECK_CLOCK_TIME_MOVE, &CClock2026Dlg::OnBnClickedCheckClockTimeMove)
	ON_BN_CLICKED(IDC_CHECK_BIG_CLOCK_MOVE_ENABLE, &CClock2026Dlg::OnBnClickedCheckBigClockMoveEnable)
	ON_EN_KILLFOCUS(IDC_EDIT_BIG_CLOCK_MOVE_SIZE, &CClock2026Dlg::OnEnKillfocusEditBigClockMoveSize)
	ON_BN_CLICKED(IDC_CHECK_DISP_BIG_CLOCK, &CClock2026Dlg::OnBnClickedCheckDispBigClock)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_UP, &CClock2026Dlg::OnBnClickedButtonBigClockUp)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_RIGHT, &CClock2026Dlg::OnBnClickedButtonBigClockRight)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_DOWN, &CClock2026Dlg::OnBnClickedButtonBigClockDown)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_LEFT, &CClock2026Dlg::OnBnClickedButtonBigClockLeft)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_RESET_POS, &CClock2026Dlg::OnBnClickedButtonBigClockResetPos)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_SNAP_SAVE_1, &CClock2026Dlg::OnBnClickedButtonBigClockSnapSave1)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_SNAP_SAVE_2, &CClock2026Dlg::OnBnClickedButtonBigClockSnapSave2)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_SNAP_SAVE_3, &CClock2026Dlg::OnBnClickedButtonBigClockSnapSave3)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_1, &CClock2026Dlg::OnBnClickedButtonBigClockSnapLoad1)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_2, &CClock2026Dlg::OnBnClickedButtonBigClockSnapLoad2)
	ON_BN_CLICKED(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_3, &CClock2026Dlg::OnBnClickedButtonBigClockSnapLoad3)
	ON_BN_CLICKED(IDC_CHECK_LOGO_ENTIRE_MOVE, &CClock2026Dlg::OnBnClickedCheckLogoEntireMove)
	ON_BN_CLICKED(IDC_CHECK_LOGO_MOVE, &CClock2026Dlg::OnBnClickedCheckLogoMove)
	ON_BN_CLICKED(IDC_CHECK_LOGO_LIVE_MOVE, &CClock2026Dlg::OnBnClickedCheckLogoLiveMove)
	ON_BN_CLICKED(IDC_CHECK_LOGO_UHD_MOVE, &CClock2026Dlg::OnBnClickedCheckLogoUhdMove)
	ON_BN_CLICKED(IDC_CHECK_LOGO_DONGSI_MOVE, &CClock2026Dlg::OnBnClickedCheckLogoDongsiMove)
	ON_BN_CLICKED(IDC_CHECK_LOGO_ENTIRE_SHOW, &CClock2026Dlg::OnBnClickedCheckLogoEntireShow)
	ON_BN_CLICKED(IDC_CHECK_LOGO_SHOW, &CClock2026Dlg::OnBnClickedCheckLogoShow)
	ON_BN_CLICKED(IDC_CHECK_LOGO_LIVE_SHOW, &CClock2026Dlg::OnBnClickedCheckLogoLiveShow)
	ON_BN_CLICKED(IDC_CHECK_LOGO_UHD_SHOW, &CClock2026Dlg::OnBnClickedCheckLogoUhdShow)
	ON_BN_CLICKED(IDC_CHECK_LOGO_DONGSI_SHOW, &CClock2026Dlg::OnBnClickedCheckLogoDongsiShow)
	ON_BN_CLICKED(IDC_RADIO_LOGO_KBS1, &CClock2026Dlg::OnBnClickedRadioLogoKbs1)
	ON_BN_CLICKED(IDC_RADIO_LOGO_KBS2, &CClock2026Dlg::OnBnClickedRadioLogoKbs2)
	ON_BN_CLICKED(IDC_RADIO_LOGO_KBS_LOCAL, &CClock2026Dlg::OnBnClickedRadioLogoKbsLocal)
	ON_BN_CLICKED(IDC_CHECK_DISP_STATION_LOGO, &CClock2026Dlg::OnBnClickedCheckDispStationLogo)
	ON_EN_KILLFOCUS(IDC_EDIT_LOGO_MOVE_SIZE, &CClock2026Dlg::OnEnKillfocusEditLogoMoveSize)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_SNAP_SAVE_1, &CClock2026Dlg::OnBnClickedButtonLogoSnapSave1)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_SNAP_SAVE_2, &CClock2026Dlg::OnBnClickedButtonLogoSnapSave2)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_SNAP_SAVE_3, &CClock2026Dlg::OnBnClickedButtonLogoSnapSave3)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_SNAP_LOAD_1, &CClock2026Dlg::OnBnClickedButtonLogoSnapLoad1)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_SNAP_LOAD_2, &CClock2026Dlg::OnBnClickedButtonLogoSnapLoad2)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_SNAP_LOAD_3, &CClock2026Dlg::OnBnClickedButtonLogoSnapLoad3)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_RESET_POS, &CClock2026Dlg::OnBnClickedButtonLogoResetPos)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_UP, &CClock2026Dlg::OnBnClickedButtonLogoUp)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_RIGHT, &CClock2026Dlg::OnBnClickedButtonLogoRight)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_DOWN, &CClock2026Dlg::OnBnClickedButtonLogoDown)
	ON_BN_CLICKED(IDC_BUTTON_LOGO_LEFT, &CClock2026Dlg::OnBnClickedButtonLogoLeft)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWS_CATEGORY, &CClock2026Dlg::OnCbnSelchangeComboNewsCategory)
	ON_CBN_KILLFOCUS(IDC_COMBO_NEWS_CATEGORY, &CClock2026Dlg::OnCbnKillfocusComboNewsCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWS_TEXT_FONT, &CClock2026Dlg::OnCbnSelchangeComboNewsTextFont)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWS_TEXT_SIZE, &CClock2026Dlg::OnCbnSelchangeComboNewsTextSize)
	ON_CBN_KILLFOCUS(IDC_COMBO_NEWS_TEXT_SIZE, &CClock2026Dlg::OnCbnKillfocusComboNewsTextSize)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_NEWS_CATEGORY_FROM_COMBO, &CClock2026Dlg::OnBnClickedButtonDeleteNewsCategoryFromCombo)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_NEWS_TEXT_SIZE_FROM_COMBO, &CClock2026Dlg::OnBnClickedButtonDeleteNewsTextSizeFromCombo)
	ON_CBN_KILLFOCUS(IDC_COMBO_NEWS_TEXT_FONT, &CClock2026Dlg::OnCbnKillfocusComboNewsTextFont)
	ON_EN_KILLFOCUS(IDC_EDIT_NEWS_SYMBOL_SIZE, &CClock2026Dlg::OnEnKillfocusEditNewsSymbolSize)
	ON_EN_KILLFOCUS(IDC_EDIT_NEWS_HANJA_SIZE, &CClock2026Dlg::OnKillfocusEditNewsHanjaSize)
	ON_BN_CLICKED(IDC_BUTTON_NEWS_TEXT_COLOR, &CClock2026Dlg::OnBnClickedButtonNewsTextColor)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON_NEWS_TEXT_COLOR, &CClock2026Dlg::OnHotitemchangeButtonNewsTextColor)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_BUTTON_RE_BACK_COLOR, &CClock2026Dlg::OnBnHotItemChangeButtonReBackColor)
	ON_BN_CLICKED(IDC_BUTTON_RE_BACK_COLOR, &CClock2026Dlg::OnBnClickedButtonReBackColor)
	ON_BN_CLICKED(IDC_BUTTON_FILE_LOAD, &CClock2026Dlg::OnBnClickedButtonFileLoad)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAVE, &CClock2026Dlg::OnBnClickedButtonFileSave)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAVE_AS, &CClock2026Dlg::OnBnClickedButtonFileSaveAs)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_LOG_FOLDER, &CClock2026Dlg::OnBnClickedButtonOpenLogFolder)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_LOG_FILE, &CClock2026Dlg::OnBnClickedButtonOpenLogFile)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_LOG_MESSAGE, &CClock2026Dlg::OnBnClickedButtonClearLogMessage)
	ON_BN_CLICKED(IDC_CHECK_APPLY_SCROLL_COLOR, &CClock2026Dlg::OnBnClickedCheckApplyScrollColor)
	ON_WM_DESTROY()
	ON_NOTIFY(EN_SELCHANGE, IDC_RICHEDIT_NEWS_DATA, &CClock2026Dlg::OnSelchangeRicheditNewsData)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_IMAGE, &CClock2026Dlg::OnBnClickedButtonInsertImage)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SHORTCUT_CONFIG, &CClock2026Dlg::OnBnClickedButtonImageShortcutConfig)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SHORTCUT_1, &CClock2026Dlg::OnBnClickedButtonImageShortcut1)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SHORTCUT_2, &CClock2026Dlg::OnBnClickedButtonImageShortcut2)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SHORTCUT_3, &CClock2026Dlg::OnBnClickedButtonImageShortcut3)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SHORTCUT_4, &CClock2026Dlg::OnBnClickedButtonImageShortcut4)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SHORTCUT_5, &CClock2026Dlg::OnBnClickedButtonImageShortcut5)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_SHORTCUT_6, &CClock2026Dlg::OnBnClickedButtonImageShortcut6)
	ON_BN_CLICKED(IDC_BUTTON_NEWS_CLEAR, &CClock2026Dlg::OnBnClickedButtonNewsClear)
	ON_BN_CLICKED(IDC_BUTTON_NEWS_SPECIAL_CHAR, &CClock2026Dlg::OnBnClickedButtonNewsSpecialChar)
	ON_BN_CLICKED(IDC_BUTTON_SPELL_CHECK, &CClock2026Dlg::OnBnClickedButtonSpellCheck)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_NEWS_SCROLL_TEMPLATE, &CClock2026Dlg::OnBnClickedButtonReloadNewsScrollTemplate)
	ON_CBN_SELCHANGE(IDC_COMBO_NEWS_SCROLL_TEMPLATE, &CClock2026Dlg::OnCbnSelchangeComboNewsScrollTemplate)
	ON_EN_KILLFOCUS(IDC_EDIT_NEWS_SCROLL_ITERATION, &CClock2026Dlg::OnKillfocusEditNewsScrollIteration)
	ON_EN_KILLFOCUS(IDC_EDIT_NEWS_SCROLL_ITEM_GAP, &CClock2026Dlg::OnKillfocusEditNewsScrollItemGap)
	ON_EN_KILLFOCUS(IDC_EDIT_NEWS_SCROLL_PREFIX_GAP, &CClock2026Dlg::OnKillfocusEditNewsScrollPrefixGap)
	ON_EN_KILLFOCUS(IDC_EDIT_NEWS_SCROLL_SPEED, &CClock2026Dlg::OnKillfocusEditNewsScrollSpeed)
	ON_BN_CLICKED(IDC_CHECK_DISP_NEWS, &CClock2026Dlg::OnBnClickedCheckDispNews)
	ON_EN_KILLFOCUS(IDC_EDIT_NEWS_DISPLAY_DURATION, &CClock2026Dlg::OnEnKillfocusEditNewsTime)
	ON_BN_CLICKED(IDC_CHECK_PREPARE_NEWS_SCROLL, &CClock2026Dlg::OnBnClickedCheckPrepareNewsScroll)
	ON_BN_CLICKED(IDC_CHECK_DISP_NEWS_SCROLL, &CClock2026Dlg::OnBnClickedCheckDispNewsScroll)
	ON_BN_CLICKED(IDC_BUTTON_WEATHER_CITY_SELECT, &CClock2026Dlg::OnBnClickedButtonWeatherCitySelect)
	ON_BN_CLICKED(IDC_BUTTON_GET_KBS_WEATHER, &CClock2026Dlg::OnBnClickedButtonGetKbsWeather)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_WEATHER_DATA, &CClock2026Dlg::OnBnClickedButtonClearWeatherData)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL_WEATHER_DATA, &CClock2026Dlg::OnBnClickedButtonClearAllWeatherData)
	ON_BN_CLICKED(IDC_CHECK_AUTO_WEATHER_READING, &CClock2026Dlg::OnBnClickedCheckAutoWeatherReading)
	ON_EN_KILLFOCUS(IDC_EDIT_AUTO_READING_WEATHER_PERIOD_MIN, &CClock2026Dlg::OnEnKillfocusEditAutoReadingWeatherPeriodMin)
	ON_BN_CLICKED(IDC_CHECK_DISP_WEATHER, &CClock2026Dlg::OnBnClickedCheckDispWeather)
	ON_BN_CLICKED(IDC_BUTTON_WEATHER_MOVE_UP, &CClock2026Dlg::OnBnClickedButtonWeatherMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_WEATHER_MOVE_DOWN, &CClock2026Dlg::OnBnClickedButtonWeatherMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_NEWS_MOVE_UP, &CClock2026Dlg::OnBnClickedButtonNewsMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_NEWS_MOVE_DOWN, &CClock2026Dlg::OnBnClickedButtonNewsMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_NOTICE_TEMPLATE, &CClock2026Dlg::OnBnClickedButtonReloadNoticeTemplate)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_NOTICE, &CClock2026Dlg::OnBnClickedButtonApplyNotice)
	ON_BN_CLICKED(IDC_RADIO_AGE_ALL, &CClock2026Dlg::OnBnClickedRadioAgeAll)
	ON_BN_CLICKED(IDC_RADIO_AGE_7, &CClock2026Dlg::OnBnClickedRadioAge7)
	ON_BN_CLICKED(IDC_RADIO_AGE_12, &CClock2026Dlg::OnBnClickedRadioAge12)
	ON_BN_CLICKED(IDC_RADIO_AGE_15, &CClock2026Dlg::OnBnClickedRadioAge15)
	ON_BN_CLICKED(IDC_RADIO_AGE_19, &CClock2026Dlg::OnBnClickedRadioAge19)
	ON_BN_CLICKED(IDC_CHECK_DISP_AGE, &CClock2026Dlg::OnBnClickedCheckDispAge)
	ON_BN_CLICKED(IDC_CHECK_DISP_NOTICE, &CClock2026Dlg::OnBnClickedCheckDispNotice)
	ON_EN_KILLFOCUS(IDC_EDIT_NOTICE_REPETITION, &CClock2026Dlg::OnEnKillfocusEditNoticeRepetition)
	ON_BN_CLICKED(IDC_CHECK_DISP_AGE_NOTICE, &CClock2026Dlg::OnBnClickedCheckDispAgeNotice)
		ON_WM_MOUSEMOVE()
		ON_BN_CLICKED(IDC_BUTTON_NOTICE_MOVE_UP, &CClock2026Dlg::OnBnClickedButtonNoticeMoveUp)
		ON_BN_CLICKED(IDC_BUTTON_NOTICE_MOVE_DOWN, &CClock2026Dlg::OnBnClickedButtonNoticeMoveDown)
		ON_CBN_SELCHANGE(IDC_COMBO_MANUALUP_TEMPLATES, &CClock2026Dlg::OnCbnSelchangeComboManualupTemplates)
		ON_BN_CLICKED(IDC_BUTTON_RELOAD_MANUALUP_TEMPLATE, &CClock2026Dlg::OnBnClickedButtonReloadManualupTemplate)
		ON_BN_CLICKED(IDC_CHECK_DISP_MANUALUP, &CClock2026Dlg::OnBnClickedCheckDispManualUp)
		ON_BN_CLICKED(IDC_BUTTON_MANUALUP_PREV, &CClock2026Dlg::OnBnClickedButtonManualupPrev)
		ON_BN_CLICKED(IDC_BUTTON_MANUALUP_NEXT, &CClock2026Dlg::OnBnClickedButtonManualupNext)
		ON_BN_CLICKED(IDCANCEL, &CClock2026Dlg::OnBnClickedCancel)
		ON_BN_CLICKED(IDC_BUTTON_LOAD_FPR, &CClock2026Dlg::OnBnClickedButtonLoadFpr)
		ON_BN_CLICKED(IDC_CHECK_DISP_LIVE_VIDEO, &CClock2026Dlg::OnBnClickedCheckDispLiveVideo)
		ON_BN_CLICKED(IDC_CHECK_LIVE_VIDEO_MOVE_ENABLE, &CClock2026Dlg::OnBnClickedCheckLiveVideoMoveEnable)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_RESET_POS, &CClock2026Dlg::OnBnClickedButtonLiveVideoResetPos)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_UP, &CClock2026Dlg::OnBnClickedButtonLiveVideoUp)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_LEFT, &CClock2026Dlg::OnBnClickedButtonLiveVideoLeft)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_DOWN, &CClock2026Dlg::OnBnClickedButtonLiveVideoDown)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_RIGHT, &CClock2026Dlg::OnBnClickedButtonLiveVideoRight)
		ON_EN_KILLFOCUS(IDC_EDIT_LIVE_VIDEO_MOVE_SIZE, &CClock2026Dlg::OnEnKillfocusEditLiveVideoMoveSize)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_SNAP_SAVE_1, &CClock2026Dlg::OnBnClickedButtonLiveVideoSnapSave1)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_SNAP_SAVE_2, &CClock2026Dlg::OnBnClickedButtonLiveVideoSnapSave2)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_SNAP_SAVE_3, &CClock2026Dlg::OnBnClickedButtonLiveVideoSnapSave3)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_1, &CClock2026Dlg::OnBnClickedButtonLiveVideoSnapLoad1)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_2, &CClock2026Dlg::OnBnClickedButtonLiveVideoSnapLoad2)
		ON_BN_CLICKED(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_3, &CClock2026Dlg::OnBnClickedButtonLiveVideoSnapLoad3)
		ON_CBN_SELCHANGE(IDC_COMBO_LIVE_VIDEO_TEMPLATES, &CClock2026Dlg::OnCbnSelchangeComboLiveVideoTemplates)
		ON_BN_CLICKED(IDC_BUTTON_RELOAD_LIVE_VIDEO_TEMPLATE, &CClock2026Dlg::OnBnClickedButtonReloadLiveVideoTemplate)
		ON_EN_KILLFOCUS(IDC_EDIT_WEATHER_DISPLAY_DURATION, &CClock2026Dlg::OnEnKillfocusEditWeatherDisplayDuration)
		
		ON_BN_CLICKED(IDC_RADIO_LOGO_KBS_ANIMATION, &CClock2026Dlg::OnBnClickedRadioLogoKbsAnimation)
		ON_BN_CLICKED(IDC_BUTTON_DATE_FORMAT, &CClock2026Dlg::OnBnClickedButtonDateFormat)
		ON_BN_CLICKED(IDC_BUTTON1, &CClock2026Dlg::OnBnClickedButton1)
		ON_BN_CLICKED(IDC_CHECK_EXCEPT_AIR, &CClock2026Dlg::OnBnClickedCheckExceptAir)
		END_MESSAGE_MAP()

// CClock2026Dlg message handlers

BOOL CClock2026Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitCtrls();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClock2026Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClock2026Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClock2026Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CClock2026Dlg::InitValues(void)
{
	m_strPrevTime = BLANK_STRING;
	m_bFileChange = FALSE;

	m_nCurNews = 0;
	m_nPrevNews = -1;

	m_nNewScrollCounter = 1;

	m_nNoticeCounter = 0;

	m_bUseAge = FALSE;
	m_bAgeShow = FALSE;
	m_bPrevAgeShow = FALSE;

	m_nAgePosX = 0;
	m_nAgePosY = 0;

	return TRUE;
}

class SafeMutexLock {
	std::mutex& m_mutex;
	bool m_locked;

public:
	SafeMutexLock(std::mutex& mutex) : m_mutex(mutex), m_locked(false) {
		if (mutex.try_lock()) {
			m_locked = true;
		}
	}

	~SafeMutexLock() {
		if (m_locked) {
			m_mutex.unlock();
		}
	}

	bool isLocked() const { return m_locked; }
};

#define VERIFY_MUTEX(mutex_obj, error_msg)  


UINT CClock2026Dlg::SyncSNTPThread(LPVOID lpvoid)
{
	
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(lpvoid);

	if (!pDlg) return 1;
	

	try {
		// mutex 사용 전 상태 검증
		if (!pDlg->m_pMutexSNTP) {
			pDlg->PrintLog(_T("SNTP mutex is not initialized"), _T("Error"), TRUE);
			return 1;
		}

		std::lock_guard<std::mutex> lock(pDlg->m_mutexSNTP);

		pDlg->SynchronizeFromSNTP(pDlg->m_pStatus->NTPConf()->ServerName());
		
	return 0;
}
catch (const std::system_error& e) {
	pDlg->PrintLog(CString(_T("Mutex error: ")) + CString(CA2T(e.what())), _T("Error"), TRUE);
	return 1;
}

	return 0;
}

BOOL CClock2026Dlg::InitTimer(void)
{
	AutoSync();

	if (m_pStatus->NTPConf()->AutoSync())
	{
		AfxBeginThread(SyncSNTPThread, this);
	}

	ConfigTimerForAutoReadingWeather(m_pStatus->WeatherConf()->AutoReading());

	return TRUE;
}

BOOL CClock2026Dlg::InitProgressWnd(void)
{
	m_pWndProgress = new CProgressWnd();
	m_pWndProgress->Create(this, _T("뉴스 스크롤 생성 중..."), TRUE);
	m_pWndProgress->ShowWindow(SW_HIDE);

	return TRUE;
}

BOOL CClock2026Dlg::InitToolTips(BOOL bNeedCreate)
{

	if (bNeedCreate)
	{
		if (!m_ToolTip.Create(this, TTS_ALWAYSTIP | TTS_BALLOON))  // TTS_BALLOON 스타일로 말풍선 모양
		{
			PrintLog(_T("툴팁 컨트롤 생성 실패!"));
			return FALSE;
		}
	}

	// 버튼에 툴팁 추가
	CString strTooltip = _T("%M : 연도, %M : 월, %D 일, %A 요일");
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_DATE_FORMAT), strTooltip );
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_CLOCK_SNAP_LOAD_1), m_pStatus->ClockSnap(0).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_CLOCK_SNAP_LOAD_2), m_pStatus->ClockSnap(1).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_CLOCK_SNAP_LOAD_3), m_pStatus->ClockSnap(2).Title());

	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_1), m_pStatus->BigClockSnap(0).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_2), m_pStatus->BigClockSnap(1).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_3), m_pStatus->BigClockSnap(2).Title());

	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LOGO_SNAP_LOAD_1), m_pStatus->LogoSnap(0).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LOGO_SNAP_LOAD_2), m_pStatus->LogoSnap(1).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LOGO_SNAP_LOAD_3), m_pStatus->LogoSnap(2).Title());

	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_1), m_pStatus->LiveVideoSnap(0).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_2), m_pStatus->LiveVideoSnap(1).Title());
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_3), m_pStatus->LiveVideoSnap(2).Title());

	// 툴팁 활성화

	m_ToolTip.SetDelayTime(TTDT_AUTOPOP, 2000);
	m_ToolTip.Activate(TRUE);

	return TRUE;
}

BOOL CClock2026Dlg::DrawPreview(CPreviewStatic& staticPreview, CString strFpgFileName)
{

	if (!CFileUtils::ExistFile(strFpgFileName))
	{
		PrintLog(strFpgFileName + CString(" 파일이 존재하지 않습니다."));
		return FALSE;
	}

	if (CFileUtils::ExtractFileExt(strFpgFileName).CompareNoCase(_T("Fpg")) != 0)
	{
		PrintLog(strFpgFileName + CString(" 는 프리뷰가 불가능한 파일입니다."));
		return FALSE;
	}

	staticPreview.SetFpgFile(strFpgFileName);

	return TRUE;
}

BOOL CClock2026Dlg::InitCombos(void)
{
	INT nIndex = -1;
	CString strFpgFileName;

	m_ComboNewsTextFont.InitFonts();

	m_pComboDateFormat = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_DATE_FORMAT));
	m_pComboNewsCategory = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_NEWS_CATEGORY));
	m_pComboNewsTextSize = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_NEWS_TEXT_SIZE));

	m_pComboNewsScrollTemplate = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_NEWS_SCROLL_TEMPLATE));

	m_pComboManualUpTemplate = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_MANUALUP_TEMPLATES));

	m_pComboLiveVideoTemplate = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_LIVE_VIDEO_TEMPLATES));

	ReloadNewsScrollTemplate();

	m_pComboNewsScrollTemplate->GetLBText(0, strFpgFileName);

	m_pStatus->NewsConf()->ScrollTemplate(strFpgFileName);

	ReloadManualUpTemplate();

	ReloadLiveVideoTemplate();

	UpdateComboData(FALSE);

	return TRUE;
}

BOOL CClock2026Dlg::InitLiveVideo(void)
{
	CString strFpgFileName = m_pStatus->LiveVideoConf()->Template();

	if (!OpenFpg(strFpgFileName, LIVE_VIDEO_PAGE_NUMBER, GetLiveVideoDir()))
	{
		PrintLog(strFpgFileName + _T("이 없어 라이브비디오 기능을 초기화 할 수 없습니다!!!"), _T("오류"), TRUE);
		return FALSE;
	}

	CCGXObject* pObject = m_pHDdaVinci->GetCGXObject(LiveVideoObjects[FROM_E(LIVE_VIDEO_OBJECTS::EXPRESSION_AREA)], LIVE_VIDEO_PAGE_NUMBER);

	if (pObject == nullptr)
	{
		PrintLog(strFpgFileName + _T("에 표출영역 알리아스가 없습니다"), _T("경고"), TRUE);
		return FALSE;
	}

	INT nWidth = pObject->GetWidth();
	INT nHeight = pObject->GetHeight();

	if (m_pLiveVideo != nullptr)
	{
		m_pLiveVideo->StopCapture();
		SAFE_DELETE(m_pLiveVideo);
	}

	m_pLiveVideo = new CLiveVideo;

	m_pLiveVideo->InitDevice(nWidth, nHeight, &m_PreviewLiveVideo);

	return TRUE;
}

BOOL CClock2026Dlg::InitPreview(void)
{

	m_pHDdaVinci->InitCanvas(this->GetSafeHwnd(), 0);

	m_PreviewNewsScroll.SetCanvas(m_pHDdaVinci->GetCanvas());

	CString strFpgFileName = m_pStatus->NewsConf()->ScrollTemplate();

	if (strFpgFileName != BLANK_STRING)
	{
		DrawPreview(m_PreviewNewsScroll, GetScrollDir() + strFpgFileName);
	}

	m_PreviewNotice.SetCanvas(m_pHDdaVinci->GetCanvas());

	strFpgFileName = m_CurNoticeData.GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

	if (strFpgFileName != BLANK_STRING)
	{
		DrawPreview(m_PreviewNotice, GetNoticeDir() + strFpgFileName);
	}

	return TRUE;
}

BOOL CClock2026Dlg::InitClocks(void)
{


	GetOrigPosClockObjects();

	GetOrigPosBigClockObjects();

	return TRUE;
}


BOOL CClock2026Dlg::InitNews(void)
{
	CString strData = m_pStatus->NewsConf()->TextSize();

	INT nIndex = m_pComboNewsTextSize->FindString(-1, strData);

	if (nIndex != CB_ERR)
	{
		m_pComboNewsTextSize->SetCurSel(nIndex);
		m_RENewsData.SetDefaultSize(GETINT(strData) * DEFAULT_FONT_SIZE);
	}

	ReloadNewsScrollTemplate();

	m_strPrevFont = m_pStatus->NewsConf()->DefaultFontName();
	m_strPrevFontSize = m_pStatus->NewsConf()->TextSize();

	CString strScrollFpgFileName;

	m_pComboNewsScrollTemplate->GetLBText(0, strScrollFpgFileName);

	m_pStatus->NewsConf()->ScrollTemplate(strScrollFpgFileName);

	return TRUE;
}


BOOL CClock2026Dlg::InitWeather(void)
{
	return TRUE;
}

BOOL CClock2026Dlg::InitNotice(void)
{
	ReloadNoticeTemplate();

	return TRUE;
}


BOOL CClock2026Dlg::InitEdits(void)
{
	InitNews();

	m_RENewsData.SetStatus(m_pStatus);

	m_RENewsData.SendMessage(EM_SETLANGOPTIONS, 0, (LPARAM)(m_RENewsData.SendMessage(EM_GETLANGOPTIONS, 0, 0) & ~IMF_AUTOFONT)); // 한영전환시 폰트가 자동으로 변하는 기능을 못하게 함.

	::SendMessage(GetDlgItem(IDC_RICHEDIT_NEWS_DATA)->GetSafeHwnd(), EM_SETEVENTMASK, 0, ENM_SELCHANGE | ENM_CHANGE); // 리치에디터 기능사용하기 위해 이벤트 설정

	LoadRichEditFontAndColor();

	return TRUE;
}

BOOL CClock2026Dlg::InitButtons(void)
{

	m_btnSuperInOut.SuperIn(TRUE);

	m_OnAirButtonColor = _COLOR_RED_;
	m_OffAirButtonColor = _COLOR_DARK_GREEN_;

	m_btnNewsScrollPrepare.SetFlat(FALSE);
	
	return TRUE;
}


BOOL CClock2026Dlg::InitCtrls(void)
{
	InitTimer();
	
	InitCombos();
		
	InitPreview();
		
	InitEdits();
		
	InitButtons();
		
	InitProgressWnd();
		
	SetFonts();
	
	SetUpSpreads(FALSE);
		
	InitStatusBar();
		
	InitToolTips(TRUE);
		
	InitClocks();
		
	InitNews();
	
	InitWeather();
		
	InitNotice();
		
	InitLiveVideo();

	if (CFileUtils::ExistFile(m_pStatus->m_strFileName))
	{
		Load(m_pStatus->m_strFileName);
	}
	else
	{
		if (m_pStatus->WeatherConf()->AutoReading())
		{
			m_pStatus->GetWeatherList()->ResetReadTime();

			RunWeatherThread();
		}
	}

	UpdateData(FALSE);

	return TRUE;
}

INT CClock2026Dlg::FindStringFromComboBox(CString FindString, CComboBox* pCombo)
{
	CString strBuffer;

	if (pCombo == nullptr) return -1;

	for (int i = 0; i < pCombo->GetCount(); i++)
	{
		pCombo->GetLBText(i, strBuffer);

		if (strBuffer == FindString)
		{
			return i;
		}
	}
	return CB_ERR;
}

void CClock2026Dlg::SetComboText(CComboBox* pCombo, std::vector<CString>& vData)
{
	if (pCombo == nullptr) return;

	pCombo->ResetContent();

	for (auto& iter : vData)
	{
		pCombo->AddString(iter);
	}
	pCombo->UpdateData(FALSE);
}

void CClock2026Dlg::GetComboText(CComboBox* pCombo, std::vector<CString>& vData)
{
	if (pCombo == nullptr) return;

	vData.clear();

	CString strData;

	for (int i = 0; i < pCombo->GetCount(); i++)
	{
		pCombo->GetLBText(i, strData);
		vData.emplace_back(strData);
	}
}

void CClock2026Dlg::UpdateComboData(BOOL bUpdataData)
{
	if (bUpdataData)
	{
		std::vector<CString> vFormats = m_pStatus->ClockConf()->Formats();
		GetComboText(m_pComboDateFormat, vFormats);
		m_pStatus->ClockConf()->Formats(vFormats);

		std::vector<CString> vCategories = m_pStatus->NewsConf()->Categories();
		GetComboText(m_pComboNewsCategory, vCategories);
		m_pStatus->NewsConf()->Categories(vCategories);
		
		std::vector<CString> vTextSizes = m_pStatus->NewsConf()->TextSizes();
		GetComboText(m_pComboNewsTextSize, vTextSizes);
		m_pStatus->NewsConf()->TextSizes(vTextSizes);
	}
	else
	{
		auto formats = m_pStatus->ClockConf()->Formats();
		auto categories = m_pStatus->NewsConf()->Categories();
		auto textSizes = m_pStatus->NewsConf()->TextSizes();
		SetComboText(m_pComboDateFormat, formats);
		SetComboText(m_pComboNewsCategory, categories);
		SetComboText(m_pComboNewsTextSize, textSizes);

		m_pComboDateFormat->SetCurSel(m_pStatus->ClockConf()->FormatIndex());
	}
}

void CClock2026Dlg::TimedMessageBox(HWND hWnd, CString strTitle, CString strMessage, UINT uiTime, UINT nFlags /*= MB_OK*/, UINT defaultReturn /*= IDOK*/, BOOL bShowStatus /*= FALSE*/)
{
	BOOL		stoppedByUser = FALSE;
	UINT	    erg;
	CString     strResult;

	if (uiTime > 0)
		erg = CDlgTimedMessageBox::TimedMessageBox(nFlags, strMessage, strTitle,
			uiTime, defaultReturn, _T(" %lu Sec.."),
			hWnd, &stoppedByUser);
	else
		erg = ::MessageBox(hWnd, strMessage, strTitle, MB_OK);

	if (!bShowStatus)
		return;

	switch (erg)
	{
	case IDOK:		    strResult = _T("IDOK");     break;
	case IDCANCEL:	strResult = _T("IDCANCEL");     break;
	case IDABORT:	    strResult = _T("IDABORT");  break;
	case IDRETRY:	    strResult = _T("IDRETRY");  break;
	case IDIGNORE:	strResult = _T("IDIGNORE");		break;
	case IDYES:		    strResult = _T("IDYES");  	break;
	case IDNO:		    strResult = _T("IDNO");		break;
	default:       strResult.Format(_T("%d"), erg); break;
	}

	strMessage.Format(_T("The MessageBox returned %s, User-Stop=%d"), strResult, stoppedByUser);
	::MessageBox(nullptr, strMessage, _T("Info"), MB_OK);
}

void CClock2026Dlg::PrintLog(CString strLog, CString strTitle, BOOL bUseMessageBox)
{

	if (strLog.Trim() == BLANK_STRING) return;

	m_nLogCount++;

	if (m_nLogCount > 100)
	{
		m_ListBoxLog.ResetContent();
		m_nLogCount = 0;
	}

	wchar_t chCR = 0x0D;
	wchar_t chLF = 0x0A;
	CString strCRLF = CString(chCR) + CString(chLF);
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("%H:%M:%S | "));
	CString strLine = strTime + strLog + strCRLF;
	m_ListBoxLog.InsertString(0, strLine);

	if (bUseMessageBox)
	{
		TimedMessageBox(this->GetSafeHwnd(), strTitle, strLog, 1500, MB_ICONEXCLAMATION | MB_OK);
	}

	CString strLogPath = GetLogsDir();

	if (!CFileUtils::ExistFile(strLogPath))
	{
		_wmkdir(strLogPath);
	}

	char* pLocale = setlocale(LC_ALL, "Korean");

	CString strFileName = strLogPath + _T("LOG_") + time.Format(_T("%Y_%m_%d")) + _T(".Log");

	FILE* fStream;
 	// Open for read (will fail if file "crt_fopen_s.c" does not exist)

    errno_t err = _wfopen_s(&fStream, strFileName.GetBuffer(), _T("a+t"));
	if (err != 0 || fStream == nullptr)
	{
		TimedMessageBox(this->GetSafeHwnd(), _T("Warning"), _T("Log File Write Error!!!"), 5000, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	CStdioFile file(fStream);

	file.WriteString(strLine);

	file.Close();
}

void CClock2026Dlg::UpdateImageButtons(void)
{

	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_1)->EnableWindow(CFileUtils::ExistFile(m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_1)));
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_2)->EnableWindow(CFileUtils::ExistFile(m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_2)));
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_3)->EnableWindow(CFileUtils::ExistFile(m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_3)));
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_4)->EnableWindow(CFileUtils::ExistFile(m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_4)));
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_5)->EnableWindow(CFileUtils::ExistFile(m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_5)));
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_6)->EnableWindow(CFileUtils::ExistFile(m_pStatus->NewsConf()->ImageShortCut(IMAGE_SHORTCUT_6)));

	for (int i = IMAGE_SHORTCUT_1; i < IMAGE_SHORTCUT_NUM; i++)
	{
		m_btnImageShortCut[i].SetImageFileName(m_pStatus->NewsConf()->ImageShortCut(i));
	}
}

BOOL CClock2026Dlg::UpdateData(BOOL bSaveAndValidate)
{
	BOOL bVal = FALSE;

	if (bSaveAndValidate)
	{
	}
	else
	{
		m_bPrepareNewsScroll ? m_btnNewsScrollPrepare.SetColor(m_OnAirButtonColor) : m_btnNewsScrollPrepare.SetColor(::GetSysColor(COLOR_BTNFACE));

		m_pStatus->m_bDispState[DISP_UPPER_CLOCK] ? m_btnDispClock.SetColor(m_OnAirButtonColor) : m_btnDispClock.SetColor(m_OffAirButtonColor);
		m_pStatus->m_bDispState[DISP_BIG_CLOCK] ? m_btnDispBigClock.SetColor(m_OnAirButtonColor) : m_btnDispBigClock.SetColor(m_OffAirButtonColor);
		m_pStatus->m_bDispState[DISP_STATION_LOGO] ? m_btnDispLogo.SetColor(m_OnAirButtonColor) : m_btnDispLogo.SetColor(m_OffAirButtonColor);

		m_pStatus->m_bDispState[DISP_NEWS] ? m_btnDispNews.SetColor(m_OnAirButtonColor) : m_btnDispNews.SetColor(m_OffAirButtonColor);
		m_pStatus->m_bDispState[DISP_NEWS_SCROLL] ? m_btnDispNewsScroll.SetColor(m_OnAirButtonColor) : m_btnDispNewsScroll.SetColor(m_OffAirButtonColor);
		m_pStatus->m_bDispState[DISP_WEATHER] ? m_btnDispWeather.SetColor(m_OnAirButtonColor) : m_btnDispWeather.SetColor(m_OffAirButtonColor);

		m_pStatus->m_bDispState[DISP_AGE] ? m_btnDispAge.SetColor(m_OnAirButtonColor) : m_btnDispAge.SetColor(m_OffAirButtonColor);
		m_pStatus->m_bDispState[DISP_NOTICE] ? m_btnDispNotice.SetColor(m_OnAirButtonColor) : m_btnDispNotice.SetColor(m_OffAirButtonColor);
		m_pStatus->m_bDispState[DISP_AGE_NOTICE] ? m_btnDispAgeNotice.SetColor(m_OnAirButtonColor) : m_btnDispAgeNotice.SetColor(m_OffAirButtonColor);
		m_pStatus->m_bDispState[DISP_MANUALUP] ? m_btnDispManualUp.SetColor(m_OnAirButtonColor) : m_btnDispManualUp.SetColor(m_OffAirButtonColor);

		m_pStatus->m_bDispState[DISP_LIVE_VIDEO] ? m_btnDispLiveVideo.SetColor(m_OnAirButtonColor) : m_btnDispLiveVideo.SetColor(m_OffAirButtonColor);
	}

	return CDialogEx::UpdateData(bSaveAndValidate);
}

void CClock2026Dlg::SetChangeMark(BOOL bChange)
{
	m_bFileChange = bChange;
	SetWindowText(MAIN_DIALOG_TITLE + m_pStatus->m_strFileName + (bChange ? CString(_T("*")) : BLANK_STRING));
}

BOOL CClock2026Dlg::GetChangeMark(void)
{
	return m_bFileChange;
}

void CClock2026Dlg::SetFonts(void)
{
	GetDlgItem(IDC_STATIC_TIME)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_STATIC_TIMER));

	GetDlgItem(IDC_COMBO_NEWS_CATEGORY)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_COMBO_SMALL));

	GetDlgItem(IDC_CHECK_DISP_CLOCK)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_DISP_BIG_CLOCK)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_DISP_STATION_LOGO)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_DISP_NEWS)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_PREPARE_NEWS_SCROLL)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_DISP_NEWS_SCROLL)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));

	GetDlgItem(IDC_STATIC_WEATHER_TIME)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_STATIC_BOLD));

	GetDlgItem(IDC_CHECK_DISP_WEATHER)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));

	GetDlgItem(IDC_STATIC_NOTICE_COUNTER)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_EDIT_MEDIUM));
	GetDlgItem(IDC_EDIT_NOTICE_REPETITION)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_EDIT_MEDIUM));

	GetDlgItem(IDC_STATIC_AGE_TIMER)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_STATIC_TIMER));
	GetDlgItem(IDC_STATIC_AGE_TIMER_STOP)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_STATIC_TIMER));

	GetDlgItem(IDC_CHECK_DISP_AGE)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_DISP_NOTICE)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_DISP_AGE_NOTICE)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));
	GetDlgItem(IDC_CHECK_DISP_MANUALUP)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));

	GetDlgItem(IDC_CHECK_DISP_LIVE_VIDEO)->SetFont(m_pStatus->UIFonts()->GetFont(FONT_BUTTON_MEDIUM));

}

void CClock2026Dlg::SetStautsBar_PaneColor(INT nID, COLORREF clrText, COLORREF clrBack)
{
	m_statusBar.SetFgColor(nID, clrText, RGB(180, 180, 180));
	m_statusBar.SetBkColor(nID, clrBack, RGB(180, 180, 180));
}

void CClock2026Dlg::WriteStatusBar_FileName(CString strFileName)
{
	SetWindowText(MAIN_DIALOG_TITLE + strFileName);
	CString strPaneText = _T("File : ") + strFileName;
	m_statusBar.SetPaneText(ID_INDICATOR_FILENAME - ID_INDICATOR_FILENAME, strPaneText);
}

void CClock2026Dlg::WriteStatusBar_OnAir(void)
{
	BOOL bOnAir = m_pStatus->IsOnAir();
	CString strPaneText = bOnAir ? _T("On Air") : _T("Off Air");

	m_statusBar.SetPaneText(ID_INDICATOR_ONAIR - ID_INDICATOR_FILENAME, strPaneText);

	if (bOnAir)
	{
		SetStautsBar_PaneColor(ID_INDICATOR_ONAIR - ID_INDICATOR_FILENAME, _COLOR_WHITE_, _COLOR_RED_);
		m_statusBar.SetFont(ID_INDICATOR_ONAIR - ID_INDICATOR_FILENAME, lfBold_StatusBarFont);
	}
	else
	{
		SetStautsBar_PaneColor(ID_INDICATOR_ONAIR - ID_INDICATOR_FILENAME, _COLOR_BLACK_, _COLOR_DEFAULT_);
		m_statusBar.SetFont(ID_INDICATOR_ONAIR - ID_INDICATOR_FILENAME, lfDefault_StatusBarFont);
	}

}

void CClock2026Dlg::WriteStatusBar_SuperInOut(void)
{
	BOOL bSuperIn = m_btnSuperInOut.IsSuperIn();

	CString strPaneText = bSuperIn ? _T("Super In") : _T("Super Out");

	m_statusBar.SetPaneText(ID_INDICATOR_SUPER - ID_INDICATOR_FILENAME, strPaneText);

	if (bSuperIn)
	{
		SetStautsBar_PaneColor(ID_INDICATOR_SUPER - ID_INDICATOR_FILENAME, _COLOR_WHITE_, _COLOR_RED_);
		m_statusBar.SetFont(ID_INDICATOR_SUPER - ID_INDICATOR_FILENAME, lfBold_StatusBarFont);
	}
	else
	{
		SetStautsBar_PaneColor(ID_INDICATOR_SUPER - ID_INDICATOR_FILENAME, _COLOR_BLACK_, _COLOR_DEFAULT_);
		m_statusBar.SetFont(ID_INDICATOR_SUPER - ID_INDICATOR_FILENAME, lfDefault_StatusBarFont);
	}

}

void CClock2026Dlg::WriteStatusBar_MousePos(CPoint pt)
{
	CString strPaneText;

	strPaneText.Format(_T("X Pos : %4d , Y Pos : %4d"), pt.x, pt.y);

    m_statusBar.ReDraw();

	m_statusBar.SetPaneText(ID_INDICATOR_MOUSE_POS - ID_INDICATOR_FILENAME, strPaneText);

}

void CClock2026Dlg::WriteStatusBar_Time(void)
{
	CString strPaneText = CTime::GetCurrentTime().Format(_T("%Y %m %d (%a) %H:%M:%S"));

	m_statusBar.SetPaneText(ID_INDICATOR_TIME - ID_INDICATOR_FILENAME, strPaneText);
}

void CClock2026Dlg::WriteStatusBar_BuildTime(void)
{
	char chDate[MAX_PATH];
	sprintf(chDate, ("KBS MPT 2025(x64) Build : %s.%s"), __DATE__, __TIME__);  // 빌드 시간을 표시한다.

	USES_CONVERSION;
	CString strPaneText = A2T(chDate);

	m_statusBar.SetPaneText(ID_INDICATOR_BUILD - ID_INDICATOR_FILENAME, strPaneText);
}

void CClock2026Dlg::SetUpSpreads(BOOL bNeedDetach /*= FALSE*/)
{
	if (bNeedDetach) DetachSpreads();

	m_SpreadNews.SetStatus(m_pStatus);
	SetUpSpread<CNewsDataList, CNewsData>(m_SpreadNews, IDC_SPREAD_NEWS, m_pStatus->PtrListNews());

	m_SpreadWeather.SetStatus(m_pStatus);
	SetUpSpread<CWeatherDataList, CWeatherData>(m_SpreadWeather, IDC_SPREAD_WEATHER, m_pStatus->PtrListWeather());

	m_SpreadNotice.SetStatus(m_pStatus);
	m_SpreadNotice.SetHDdaVinci(m_pHDdaVinci);
	SetUpSpread<CNoticeDataList, CNoticeData>(m_SpreadNotice, IDC_SPREAD_NOTICE, m_pStatus->PtrListNotice());

	m_SpreadAlias.SetStatus(m_pStatus);
	SetUpSpreadAlias(m_SpreadAlias, IDC_SPREAD_ALIAS);

}

BOOL CClock2026Dlg::InitStatusBar(void)
{
	if (!m_statusBar.CreateStatusBar(this, indicators, sizeof(indicators) / sizeof(UINT)))
	{
		return false;
	}

	CRect rect;
	GetClientRect(&rect); //Size the two panes
	m_statusBar.SetPaneInfo(ID_INDICATOR_FILENAME - ID_INDICATOR_FILENAME, ID_INDICATOR_FILENAME, SBPS_NORMAL, 700);
	m_statusBar.SetPaneInfo(ID_INDICATOR_ONAIR - ID_INDICATOR_FILENAME, ID_INDICATOR_FILENAME, SBPS_NORMAL, 70);
	m_statusBar.SetPaneInfo(ID_INDICATOR_SUPER - ID_INDICATOR_FILENAME, ID_INDICATOR_FILENAME, SBPS_NORMAL, 70);
	m_statusBar.SetPaneInfo(ID_INDICATOR_MOUSE_POS - ID_INDICATOR_FILENAME, ID_INDICATOR_FILENAME, SBPS_NORMAL, 150);
	m_statusBar.SetPaneInfo(ID_INDICATOR_TIME - ID_INDICATOR_FILENAME, ID_INDICATOR_FILENAME, SBPS_NORMAL, 250);
	m_statusBar.SetPaneInfo(ID_INDICATOR_BUILD - ID_INDICATOR_FILENAME, ID_INDICATOR_TIME, SBPS_STRETCH, 0); //This is where we actually draw it on the screen

	m_statusBar.SetMode(ID_INDICATOR_FILENAME - ID_INDICATOR_FILENAME, XSB_TEXT | DT_LEFT);
	m_statusBar.SetMode(ID_INDICATOR_ONAIR - ID_INDICATOR_FILENAME, XSB_TEXT | DT_CENTER | DT_VCENTER);
	m_statusBar.SetMode(ID_INDICATOR_SUPER - ID_INDICATOR_FILENAME, XSB_TEXT | DT_CENTER | DT_VCENTER);
	m_statusBar.SetMode(ID_INDICATOR_MOUSE_POS - ID_INDICATOR_FILENAME, XSB_TEXT | DT_CENTER | DT_VCENTER);
	m_statusBar.SetMode(ID_INDICATOR_TIME - ID_INDICATOR_FILENAME, XSB_TEXT | DT_CENTER | DT_VCENTER);
	m_statusBar.SetMode(ID_INDICATOR_BUILD - ID_INDICATOR_FILENAME, XSB_TEXT | DT_CENTER | DT_VCENTER); //This is where we actually draw it on the screen

	m_statusBar.SetFont(ID_INDICATOR_FILENAME - ID_INDICATOR_FILENAME, _T("MS Shell Dlg"), 80);
	m_statusBar.SetFont(ID_INDICATOR_TIME - ID_INDICATOR_FILENAME, _T("MS Shell Dlg"), 80);

	lfDefault_StatusBarFont = m_statusBar.GetFont(0);
	lfBold_StatusBarFont = lfDefault_StatusBarFont;

	lfBold_StatusBarFont.lfWeight = 700;  // Normal 400, Bold 700

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_BUILD);

	SetTimer(ID_TIMER_TIME, 200, nullptr);

	WriteStatusBar_FileName(BLANK_STRING);
	WriteStatusBar_BuildTime();

	return TRUE;
}

BOOL CClock2026Dlg::IsEditFocus(void)
{
	CWnd* pWnd = GetFocus();

	if (pWnd == GetDlgItem(IDC_EDIT_NTP_ADJUST_TIME)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_CLOCK_MOVE_SIZE)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_CLOCK_SEC_OVER)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_CLOCK_SEC_UNDER)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_BIG_CLOCK_MOVE_SIZE)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_LOGO_MOVE_SIZE)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_LIVE_VIDEO_MOVE_SIZE)) return TRUE;

	if (pWnd == m_pComboDateFormat->GetDlgItem(1001)) return TRUE;
	if (pWnd == m_pComboNewsCategory->GetDlgItem(1001)) return TRUE;
	if (pWnd == m_ComboNewsTextFont.GetDlgItem(1001)) return TRUE;
	if (pWnd == m_pComboNewsTextSize->GetDlgItem(1001)) return TRUE;

	if (pWnd == GetDlgItem(IDC_EDIT_NEWS_SYMBOL_SIZE)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_NEWS_HANJA_SIZE)) return TRUE;

	if (pWnd == GetDlgItem(IDC_EDIT_NEWS_SCROLL_ITERATION)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_NEWS_SCROLL_ITEM_GAP)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_NEWS_SCROLL_PREFIX_GAP)) return TRUE;
	if (pWnd == GetDlgItem(IDC_EDIT_NEWS_SCROLL_SPEED)) return TRUE;

	if (pWnd == GetDlgItem(IDC_EDIT_NEWS_DISPLAY_DURATION)) return TRUE;

	if (pWnd == GetDlgItem(IDC_EDIT_AUTO_READING_WEATHER_PERIOD_MIN)) return TRUE;

	if (pWnd == GetDlgItem(IDC_EDIT_NOTICE_REPETITION)) return TRUE;

	if (pWnd == GetDlgItem(IDC_EDIT_WEATHER_DISPLAY_DURATION)) return TRUE;

	return FALSE;
}

void CClock2026Dlg::DetachSpreads(void)
{
	m_SpreadNews.Detach();
	m_SpreadWeather.Detach();
	m_SpreadNotice.Detach();
	m_SpreadAlias.Detach();
}

void CClock2026Dlg::RefreshUI(void)
{
	m_SpreadNews.RefreshComboString();
	m_SpreadNews.UpdateDataSheet(1, FALSE, FALSE);
	m_SpreadNotice.UpdateDataSheet(1, FALSE, FALSE);
	SelectNotice(m_CurNoticeData);
	m_SpreadWeather.UpdateDataSheet(1, FALSE, FALSE);
	
	DrawPreview(m_PreviewNewsScroll, GetScrollDir() + m_pStatus->NewsConf()->ScrollTemplate());
	LoadRichEditFontAndColor();
	UpdateREUI(FALSE);
	UpdateComboData(FALSE);
	UpdateImageButtons();

	EnableClockWindow();
	EnableBigClockWindow();

	EnableLogoWindow();

	InitToolTips();

	UpdateData(FALSE);
}

void CClock2026Dlg::Serialize(CArchive& ar)
{
	if (m_pStatus) m_pStatus->Serialize(ar);
	
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

BOOL CClock2026Dlg::Load(CString& strFileName)
{
	CFileException fe;
	CFile file;
	if (file.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		CArchive loadArchive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
		TRY
		{
			CWaitCursor wait;
			if (file.GetLength() != 0)
			{
				Serialize(loadArchive);
			}
			loadArchive.Close();
			file.Close();
		}
			CATCH_ALL(e)
		{
			file.Abort();

			TRY
			{
				//	ReportSaveLoadException(strFileName, e,	FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			}
			END_TRY

				do { e->Delete(); } while (0);

			return FALSE;
		}
		END_CATCH_ALL
	}
	else
	{
		//	ReportSaveLoadException(strFileName, &fe,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		return FALSE;
	}

	m_curNewsData.Clear();
	m_CurNoticeData.Clear();

	m_pStatus->GetWeatherList()->ClearWeatherAndAirData(); // 날씨 데이터를 지운다

	RefreshUI();

	if (m_pStatus->WeatherConf()->AutoReading())
	{
		m_pStatus->GetWeatherList()->ResetReadTime();

		RunWeatherThread();
	}

	m_nNewScrollCounter = 1;

	SetChangeMark(FALSE);

	WriteStatusBar_FileName(strFileName);

	UpdateData(FALSE);

	return TRUE;
}

BOOL CClock2026Dlg::LoadFile(CString& strFileName)
{

	if (m_pStatus->IsOnAir())
	{
		PrintLog(_T("송출 중일 때는 파일을 로딩할 수 없습니다."), _T("경고"), TRUE);
		return FALSE;
	}

	CFileDialog dlg(TRUE, MPT_EXT, strFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT, MPT_FILTER);
	
	if (dlg.DoModal() == IDOK)
	{
	
		CString strTempFileName = dlg.GetPathName();
		if (!Load(strTempFileName))
		{
			PrintLog(dlg.GetPathName() + _T("파일을 읽지 못했습니다."), _T("오류"), TRUE);
			return FALSE;
		}
		else
		{
			strFileName = dlg.GetPathName();
			PrintLog(strFileName + _T("을 정상적으로 열었습니다."), _T("알림"), FALSE);
		}
	}

	return TRUE;
}

void CClock2026Dlg::OnBnClickedButtonFileLoad()
{
	if (GetChangeMark())
	{
		if (IDYES == MessageBox(_T("파일을 저장하시겠습니까?"), _T("저장하기"), MB_YESNO | MB_ICONQUESTION))
		{
			SaveFile(m_pStatus->m_strFileName);
		}
	}

	LoadFile(m_pStatus->m_strFileName);
}

BOOL CClock2026Dlg::Save(CString strFileName)
{
	CFileException fe;
	CFile file;
	if (file.Open(strFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
		TRY
		{
			CWaitCursor wait;
			Serialize(saveArchive);
			saveArchive.Close();
			file.Close();
		}
			CATCH_ALL(e)
		{
			file.Abort();

			do { e->Delete(); } while (false);
			return FALSE;
		}
		END_CATCH_ALL
	}
	else
	{
		return FALSE;
	}

	SetChangeMark(FALSE);

	WriteStatusBar_FileName(strFileName);

	return TRUE;
}

BOOL CClock2026Dlg::SaveAsFile(CString& strFileName)
{
	CFileDialog dlg(FALSE, MPT_EXT, strFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER |OFN_OVERWRITEPROMPT, MPT_FILTER);


	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;

		if (!Save(dlg.GetPathName()))
		{
			PrintLog(dlg.GetPathName() + _T("파일을 저장하지 못했습니다."), _T("오류"), TRUE);
			return FALSE;
		}
		strFileName = dlg.GetPathName();

		PrintLog(strFileName + _T("을 정상적으로 저장했습니다."), _T("알림"), FALSE);
	}

	return TRUE;
}

BOOL CClock2026Dlg::SaveFile(CString& strFileName)
{
	if (CFileUtils::IsValidFileName(strFileName))
	{
		if (Save(strFileName))
		{
			PrintLog(strFileName + _T("을 정상적으로 저장했습니다."), _T(""), FALSE);
			return true;
		}
	}

	return SaveAsFile(strFileName);
}

void CClock2026Dlg::OnBnClickedButtonFileSave()
{
	SaveFile(m_pStatus->m_strFileName);
}

void CClock2026Dlg::OnBnClickedButtonFileSaveAs()
{
	SaveAsFile(m_pStatus->m_strFileName);
}


BOOL CClock2026Dlg::SynchronizeFromSNTP(CString strServerName)
{
	WSADATA wsaData;
	BYTE wsMajorVersion = 1;
	BYTE wsMinorVersion = 1;
	WORD wVersionRequested = MAKEWORD(wsMinorVersion, wsMajorVersion);

	CString strResult = BLANK_STRING;

	if (WSAStartup(wVersionRequested, &wsaData) != 0)          // 랜연결이 안되었을때....
	{
		PrintLog(_T("소켓을 만드는데 실패하였습니다"));
		return FALSE;
	}
	if (LOBYTE(wsaData.wVersion) != wsMajorVersion || HIBYTE(wsaData.wVersion) != wsMinorVersion)
	{
		PrintLog(_T("소켓이 버전을 지원하지 않습니다."));
		return FALSE;
	}

	CSNTPClient sntp;
	NtpServerResponse response;
	if (sntp.GetServerTime(strServerName, response))
	{
		CString strLock;
		strResult = strServerName + _T("서버로부터 성공적으로 시간을 받았습니다.");

		strLock.Format(_T("%f 초 안에서 동기화 되었습니다.\n"), response.m_LocalClockOffset);

		if (fabs(response.m_LocalClockOffset) < 3600)
		{

			CNtpTime newTime(CNtpTime::GetCurrentTime() + (response.m_LocalClockOffset + GETDOUBLE(m_pStatus->NTPConf()->AdjustTime()) / 1000.0f + 0.0f));

			if (sntp.SetClientTime(newTime))
			{
				strResult = strServerName + _T("에 성공적으로 동기화하였습니다. ") + strLock;
				KillTimer(ID_TIMER_TIME);
				SetTimer(ID_TIMER_TIME, 200, nullptr);
			}
			else
			{
				strResult = _T("동기화에 실패하였습니다. 다시 시도하세요");
				PrintLog(strResult);
				return FALSE;
			}
		}
		else
		{
			strResult = _T("서버와의 시간차가 1시간 이상입니다. PC 시간을 표준시간과 1시간 이내로 조정하세요 ");
			PrintLog(strResult);
			return FALSE;
		}

	}
	else
	{
		strResult = _T("서버로부터 응답을 받지 못했습니다.");
		PrintLog(strResult);
		return FALSE;
	}

	WSACleanup();

	PrintLog(strResult);
	return TRUE;
}

void CClock2026Dlg::OnBnClickedButtonSync()
{
	GetDlgItem(IDC_BUTTON_SYNC)->EnableWindow(FALSE);

	AfxBeginThread(SyncSNTPThread, this);

	GetDlgItem(IDC_BUTTON_SYNC)->EnableWindow(TRUE);
}

void CClock2026Dlg::OnCbnSelchangeComboTimeServer()
{
	CComboBox* pCombo = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_TIME_SERVER));
	CString strNewServerName;

	INT nIndex = pCombo->GetCurSel();

	m_pStatus->NTPConf()->ServerIndex(nIndex);

	pCombo->GetLBText(nIndex, strNewServerName);

	if (strNewServerName != m_pStatus->NTPConf()->ServerName())
	{
		m_pStatus->NTPConf()->ServerName(strNewServerName);
		SynchronizeFromSNTP(m_pStatus->NTPConf()->ServerName());
	}
	UpdateData(FALSE);
}

void CClock2026Dlg::OnTimer(UINT_PTR nIDEvent)
{
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("%H:%M:%S"));

	switch (nIDEvent)
	{

	case ID_TIMER_TIME:

		if (m_strPrevTime != strTime)
		{
#ifdef _TIMER_TRANS_MODE_
			if (m_pStatus->GetDispState(DISP_UPPER_CLOCK))
			{
				if (!m_pHDdaVinci->IsTransmitting(CLOCK_PAGE_NUMBER))
				{
					DisplayClock();
				}
			}
			if (m_pStatus->GetDispState(DISP_BIG_CLOCK))
			{
				if (!m_pHDdaVinci->IsTransmitting(CLOCK_PAGE_NUMBER))
				{
					DisplayBigClock();
				}
			}
#endif
			SetDlgItemText(IDC_STATIC_TIME, m_pStatus->m_TimeString.GetString(TRUE));

			WriteStatusBar_Time();
		}

		m_strPrevTime = strTime;
		break;

	case ID_TIMER_DISPLAY_CLOCK:
#ifdef _THREAD_TRANS_MODE_
		AfxBeginThread(DisplayClockThread, this);
		KillTimer(ID_TIMER_DISPLAY_CLOCK);
#endif
		break;

	case ID_TIMER_DISPLAY_BIG_CLOCK:
#ifdef _THREAD_TRANS_MODE_
		AfxBeginThread(DisplayBigClockThread, this);
		KillTimer(ID_TIMER_DISPLAY_BIG_CLOCK);
#endif
		break;

	case ID_TIMER_DISPLAY_STATION_LOGO:
#ifdef _TIMER_TRANS_MODE_
		if (m_pStatus->GetDispState(DISP_STATION_LOGO))
		{
			DisplayStationLogo();
		}
#endif 
#ifdef _THREAD_TRANS_MODE_
		AfxBeginThread(DisplayLogoThread, this);
		KillTimer(ID_TIMER_DISPLAY_STATION_LOGO);
#endif
		break;

	case ID_TIMER_SYNC:
		SynchronizeFromSNTP(m_pStatus->NTPConf()->ServerName());
		break;

	case ID_TIMER_LOG:
		//if (CurTime.GetSecond() != 0) return;  // 정각 분단위 일때만..

		//if (CurTime.GetDayOfWeek() != m_prevTime.GetDayOfWeek())
		//{
		//	OnBnClickedButtonClearLogMessage();
		//}

		//if (CurTime.GetYear() != m_prevTime.GetYear())
		//{
		//	CFileUtils::RemoveAllFile(GetLogsDir());  // 연도가 바뀌면 로그 파일 모두를 지운다.
		//}

		//m_prevTime = CurTime;
		break;


	case ID_TIMER_NEWS_DATA:
#ifdef _THREAD_TRANS_MODE_
		AfxBeginThread(DisplayNewsThread, this);
		KillTimer(ID_TIMER_NEWS_DATA);
#endif
#ifdef _TIMER_TRANS_MODE_
		if (m_pStatus->GetDispState(DISP_NEWS))
		{
			if (!m_pHDdaVinci->IsTransmitting(NEWS_PAGE_NUMBER))
			{
				DisplayNews(FALSE); 
				SetTimer(ID_TIMER_NEWS_DATA, m_pStatus->NewsConf()->DisplayDuration() * THOUSAND, nullptr);
			}
			else
			{
				SetTimer(ID_TIMER_NEWS_DATA,THOUSAND, nullptr);
			}
		}
#endif
		break;

	case ID_TIMER_WEATHER_DATA:
#ifdef _THREAD_TRANS_MODE_
	//	AfxBeginThread(DisplayWeatherThread, this);
	//	KillTimer(ID_TIMER_WEATHER_DATA);
		if (m_pStatus->GetDispState(DISP_WEATHER))
		{
			if (!m_pHDdaVinci->IsTransmitting(WEATHER_PAGE_NUMBER))
			{
				DisplayWeather(FALSE);
				SetTimer(ID_TIMER_WEATHER_DATA, m_pStatus->WeatherConf()->DisplayDuration() * THOUSAND, nullptr);
			}
			else
			{
				SetTimer(ID_TIMER_WEATHER_DATA, THOUSAND, nullptr);
			}
		}
#endif
#ifdef _TIMER_TRANS_MODE_
		if (m_pStatus->GetDispState(DISP_WEATHER))
		{
			if (!m_pHDdaVinci->IsTransmitting(WEATHER_PAGE_NUMBER))
			{
				DisplayWeather(FALSE);
				SetTimer(ID_TIMER_WEATHER_DATA, m_pStatus->WeatherConf()->DisplayDuration() * THOUSAND, nullptr);
			}
			else
			{
				SetTimer(ID_TIMER_WEATHER_DATA,THOUSAND, nullptr);
			}
		}
#endif
		break;

	case ID_TIMER_AUTO_WEATHER_READING:
		if (m_pStatus->GetDispState(DISP_WEATHER))
		{
			PrintLog(_T("날씨 송출 중에는 날씨 데이터를 업데이트하지 않습니다!!!"));
		}
		else
		{
			RunWeatherThread();
		}
		break;

	case ID_TIMER_DISPLAY_NOTICE:
#ifdef _THREAD_TRANS_MODE_
		AfxBeginThread(DisplayNoticeThread, this);
		KillTimer(ID_TIMER_DISPLAY_NOTICE);
#endif
#ifdef _TIMER_TRANS_MODE_
		if (m_pStatus->GetDispState(DISP_NOTICE) || m_pStatus->GetDispState(DISP_AGE_NOTICE))
		{
			DisplayNotice(FALSE);
		}
#endif
		break;

	case ID_TIMER_DISPLAY_AGE:
		if (m_pStatus->GetDispState(DISP_AGE) || m_pStatus->GetDispState(DISP_AGE_NOTICE))
		{
			DispAge();
		}
		break;

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CClock2026Dlg::AutoSync(void)
{
	if (m_pStatus->NTPConf()->AutoSync())
	{
		SetTimer(ID_TIMER_SYNC, GETINT(m_pStatus->NTPConf()->Interval()) * 60 * THOUSAND, nullptr);
	}
	else
	{
		KillTimer(ID_TIMER_SYNC);
	}
}

void CClock2026Dlg::OnBnClickedCheckAutoSync()
{
	m_pStatus->NTPConf()->AutoSync(IsDlgButtonChecked(IDC_CHECK_AUTO_SYNC));

	AutoSync();
}

void CClock2026Dlg::OnCbnSelchangeComboInterval()
{
	CComboBox* pCombo = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_INTERVAL));

	CString strNewInterval;

	int nIndex = pCombo->GetCurSel();

	m_pStatus->NTPConf()->IntervalIndex(nIndex);

	pCombo->GetLBText(nIndex, strNewInterval);
	m_pStatus->NTPConf()->Interval(strNewInterval);

	AutoSync();

	UpdateData(FALSE);
}

void CClock2026Dlg::OnEnKillfocusEditNtpAdjustTime()
{
	CString strBuffer;
	INT nTime = 0;

	GetDlgItemText(IDC_EDIT_NTP_ADJUST_TIME, strBuffer);

	if (IsNumberString(strBuffer))
	{
		nTime = min(1800000, max(-1800000, GETINT(strBuffer)));

		m_pStatus->NTPConf()->AdjustTime(GETSTR(nTime));
	}
}

BOOL CClock2026Dlg::PreTranslateMessage(MSG* pMsg)
{
	CWnd* pWnd;
	CString strFontName;
	CString strFontSize;

	switch (pMsg->message)
	{
	case WM_KEYDOWN:		// WM_CHAR로 하면 control key들은 먹지 않는다..
		pWnd = GetFocus();

		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (pWnd == GetDlgItem(IDC_RICHEDIT_NEWS_DATA))
			{
				UpdateREUI(TRUE);
			}

			if (IsEditFocus())
			{
				this->GetDlgItem(IDC_LIST_LOG_MESSAGE)->SetFocus();
			}

			UpdateData(FALSE);
			return TRUE;
			break;

		case VK_ESCAPE:
			OnBnClickedButtonClear();
			return TRUE;
			break;

		case VK_F2:
			m_btnSuperInOut.Toggle();
			return TRUE;
			break;

		case VK_F4:
			m_ComboNewsTextFont.GetWindowText(strFontName);
			m_pComboNewsTextSize->GetWindowText(strFontSize);

			if (strFontName != m_pStatus->NewsConf()->SymbolFont())  // 심볼폰트 상태로 전환
			{
				m_strPrevFont = strFontName;
				m_strPrevFontSize = strFontSize;

				m_nPrevHanEngMode = GetHanEngMode();

				SetEnglishMode();

				int n = m_ComboNewsTextFont.FindString(m_pStatus->NewsConf()->SymbolFont());
				if (n != CB_ERR) 
				{
					m_ComboNewsTextFont.SetCurSel(n);
					m_RENewsData.SetSelectionFont(m_pStatus->NewsConf()->SymbolFont());
					
				}
				else
				{
					m_pStatus->NewsConf()->SymbolFont(_T("KBSsymbol1"));
					n = m_ComboNewsTextFont.FindString(m_pStatus->NewsConf()->SymbolFont());
					if (n != CB_ERR)
					{
						m_ComboNewsTextFont.SetCurSel(n);
						m_RENewsData.SetSelectionFont(m_pStatus->NewsConf()->SymbolFont());
					}
				}

				n = FindStringFromComboBox(m_pStatus->NewsConf()->SymbolSize(), m_pComboNewsTextSize);
				if (n != CB_ERR)
				{
					m_pComboNewsTextSize->SetCurSel(n);
					m_RENewsData.SetSelectionSize(GETINT(m_pStatus->NewsConf()->SymbolSize()));
				}
				else
				{
					m_pStatus->NewsConf()->SymbolSize(_T("100"));
					n = FindStringFromComboBox(m_pStatus->NewsConf()->SymbolSize(), m_pComboNewsTextSize);
					if (n != CB_ERR)
					{
						m_pComboNewsTextSize->SetCurSel(n);
						m_RENewsData.SetSelectionSize(GETINT(m_pStatus->NewsConf()->SymbolSize()));
					}
				}
			}
			else                                               // 원상태로 전환
			{
				(m_nPrevHanEngMode == HANGUL_MODE) ? SetHangulMode() : SetEnglishMode();

				int n = m_ComboNewsTextFont.FindString(m_strPrevFont);
				if (n != CB_ERR)
				{
					m_ComboNewsTextFont.SetCurSel(n);
					m_RENewsData.SetSelectionFont(m_strPrevFont);
				}

				n = FindStringFromComboBox(m_strPrevFontSize, m_pComboNewsTextSize);
				if (n != CB_ERR)
				{
					m_pComboNewsTextSize->SetCurSel(n);
					m_RENewsData.SetSelectionSize(GETINT(m_strPrevFontSize));
				}

			}
			return TRUE;
			break;


		case VK_F1:
		case VK_F3:
			return TRUE;
			break;
		}
		break;
	}

	if (m_statusBar)
	{
		WriteStatusBar_OnAir();
		WriteStatusBar_SuperInOut();
	}

	if (m_ToolTip.GetSafeHwnd()) // 툴팁 컨트롤이 유효한지 확인
	{
		m_ToolTip.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CClock2026Dlg::OnSuperInOut(WPARAM wParam, LPARAM lParam)
{
	m_btnSuperInOut.Toggle();

	return 0;
}

LRESULT CClock2026Dlg::OnHDdaVinciClear(WPARAM wParam, LPARAM lParam)
{
	HDdaVinciClear();

	return 0;
}

LRESULT CClock2026Dlg::OnPrintLog(WPARAM wParam, LPARAM lParam)
{
	WND_ARG* pArg = reinterpret_cast<WND_ARG*>(lParam);

	PrintLog(pArg->strData, _T("알림"), pArg->nData);

	SAFE_DELETE(pArg);

	return 0;
}

void CClock2026Dlg::KillTimersForDisplay(void)
{
	KillTimer(ID_TIMER_NEWS_DATA);
	KillTimer(ID_TIMER_WEATHER_DATA);
	KillTimer(ID_TIMER_DISPLAY_NOTICE);
	KillTimer(ID_TIMER_DISPLAY_AGE);
	KillTimer(ID_TIMER_DISPLAY_STATION_LOGO);
}

void CClock2026Dlg::HDdaVinciClear(void)
{
#ifdef _THREAD_TRANS_MODE_
	_MEASURE_START_(1)
	m_bPrepareNewsScroll = FALSE;

	KillTimersForDisplay();

	if (m_pStatus->GetDispState(DISP_UPPER_CLOCK)) TakeOutClock();
	if (m_pStatus->GetDispState(DISP_BIG_CLOCK)) TakeOutBigClock();
	if (m_pStatus->GetDispState(DISP_MANUALUP)) TakeOutManualUp();
	if (m_pStatus->GetDispState(DISP_STATION_LOGO)) TakeOutLogo();
	if (m_pStatus->GetDispState(DISP_AGE) || m_pStatus->GetDispState(DISP_AGE_NOTICE)) TakeOutAge();
	if (m_pStatus->GetDispState(DISP_NOTICE) || m_pStatus->GetDispState(DISP_AGE_NOTICE)) TakeOutNotice();
	if (m_pStatus->GetDispState(DISP_LIVE_VIDEO)) TakeOutLiveVideo();
	if (m_pStatus->GetDispState(DISP_NEWS)) TakeOutNews();
	if (m_pStatus->GetDispState(DISP_NEWS_SCROLL)) TakeOutNewsScroll(FALSE);
	if (m_pStatus->GetDispState(DISP_WEATHER)) TakeOutWeather();

	m_pStatus->SetDispStateOff();

	m_pHDdaVinci->SetPageHandler(this, nullptr);

	for (int i = 0; i < MAX_SCENE_NUM; i++)
	{
		m_pHDdaVinci->TakeOut(i);
	}

	PrintLog(_T("모든 송출을 중단했습니다."));

	UpdateData(FALSE);
	_MEASURE_END_(1,_T("Clear"))
#endif

#ifdef _TIMER_TRANS_MODE_

	CancelPrepareNewsScroll();

	KillTimersForDisplay();

	m_pStatus->SetDispStateOff();

	m_pHDdaVinci->SetPageHandler(this, nullptr);

	for (int i = 0; i < MAX_SCENE_NUM; i++)
	{
		m_pHDdaVinci->TakeOut(i);
	}

	PrintLog(_T("모든 송출을 중단했습니다."));

	UpdateData(FALSE);

#endif
}

void CClock2026Dlg::OnBnClickedButtonClear()
{
	HDdaVinciClear();
	EnableWindow(0, FALSE);
}

BOOL CClock2026Dlg::OpenFpg(CString strSceneName, int nPage,CString strPath)
{
	CString strFpgFileName = strPath + strSceneName;

	CString strExt = CFileUtils::ExtractFileExt(strFpgFileName);

	if (strExt.CompareNoCase(_T("fpg")) != 0)
	{
		strFpgFileName += FPG_EXT;
	}

	if (!CFileUtils::ExistFile(strFpgFileName))
	{
		PrintLog(strFpgFileName + _T(" 파일이 존재하지 않습니다."), _T("오류"), TRUE);
		return FALSE;
	}

	m_pHDdaVinci->Open(strFpgFileName, 1, nPage);

	return TRUE;
}

void CClock2026Dlg::GetDateForClock(CTimeString& timeString, CString& strDate)
{
	strDate = m_pStatus->ClockConf()->Format();

	strDate = strDate.MakeUpper();

	strDate.Replace(_T("%Y"), timeString.m_strYear);
	strDate.Replace(_T("%M"), timeString.m_strMonth);
	strDate.Replace(_T("%D"), timeString.m_strDay);
	strDate.Replace(_T("%A"), timeString.m_strDayOfWeek);
}

void CClock2026Dlg::GetHourForClock(CTimeString& timeString, CString& strHour)
{
	CString strHour10;

	if (m_pStatus->ClockConf()->HideFirstDigit0OfHour())
	{
		if (GETINT(timeString.m_strHour10) == 0)
		{
			timeString.m_strHour10 = SPACE_STRING;
		}
	}

	strHour.Format(_T("%s%s"), timeString.m_strHour10, timeString.m_strHour1);
}

void CClock2026Dlg::GetOrigPosClockObjects(void)
{
	if (!OpenFpg(_T("상단시계"), CLOCK_PAGE_NUMBER))
	{
		return;
	}

	CCGXObject* pObjects[FROM_E(CLOCK_OBJECTS::LAST_NUM)];

	for (int i = 0; i < FROM_E(CLOCK_OBJECTS::LAST_NUM); i++)
	{
		pObjects[i] = m_pHDdaVinci->GetCGXObject(clockObjects[i], CLOCK_PAGE_NUMBER);
	}

	pObjects[FROM_E(CLOCK_OBJECTS::DATE)]->GetPosition(
		&(m_pStatus->ClockConf()->OrigPos().m_ptDate.X), 
		&(m_pStatus->ClockConf()->OrigPos().m_ptDate.Y));
		
	pObjects[FROM_E(CLOCK_OBJECTS::HOUR)]->GetPositionEx(
		&(m_pStatus->ClockConf()->OrigPos().m_ptHour.X),
		&(m_pStatus->ClockConf()->OrigPos().m_ptHour.Y),
		CGX_REFERENCE_POINT_RIGHTCENTER);

	pObjects[FROM_E(CLOCK_OBJECTS::MIN_COLON)]->GetPosition(
		&(m_pStatus->ClockConf()->OrigPos().m_ptMinColon.X),
		&(m_pStatus->ClockConf()->OrigPos().m_ptMinColon.Y));

	pObjects[FROM_E(CLOCK_OBJECTS::TEN_MIN)]->GetPosition(
		&(m_pStatus->ClockConf()->OrigPos().m_ptTenMin.X),
		&(m_pStatus->ClockConf()->OrigPos().m_ptTenMin.Y));

	pObjects[FROM_E(CLOCK_OBJECTS::MIN)]->GetPosition(
		&(m_pStatus->ClockConf()->OrigPos().m_ptMin.X),
		&(m_pStatus->ClockConf()->OrigPos().m_ptMin.Y));

	pObjects[FROM_E(CLOCK_OBJECTS::SEC_COLON)]->GetPosition(
		&(m_pStatus->ClockConf()->OrigPos().m_ptSecColon.X),
		&(m_pStatus->ClockConf()->OrigPos().m_ptSecColon.Y));

	pObjects[FROM_E(CLOCK_OBJECTS::TEN_SEC)]->GetPosition(
		&(m_pStatus->ClockConf()->OrigPos().m_ptTenSec.X),
		&(m_pStatus->ClockConf()->OrigPos().m_ptTenSec.Y));

	pObjects[FROM_E(CLOCK_OBJECTS::SEC)]->GetPosition(
		&(m_pStatus->ClockConf()->OrigPos().m_ptSec.X),
		&(m_pStatus->ClockConf()->OrigPos().m_ptSec.Y));
}

void CClock2026Dlg::SetPosClockObjects(void)
{
	SClockPos origPos = m_pStatus->ClockConf()->OrigPos();
	Point ptDateOffset = m_pStatus->ClockConf()->DateOffset();
	Point ptTimeOffset = m_pStatus->ClockConf()->TimeOffset();

	m_pHDdaVinci->ObjectSetPositionByAbs(clockObjects[FROM_E(CLOCK_OBJECTS::DATE)],
		origPos.m_ptDate.X + ptDateOffset.X,
		origPos.m_ptDate.Y + ptDateOffset.Y,
	    CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbsEx(clockObjects[FROM_E(CLOCK_OBJECTS::HOUR)],
		origPos.m_ptHour.X + ptTimeOffset.X,
		origPos.m_ptHour.Y + ptTimeOffset.Y,
		CGX_REFERENCE_POINT_RIGHTCENTER,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(clockObjects[FROM_E(CLOCK_OBJECTS::MIN_COLON)],
		origPos.m_ptMinColon.X + ptTimeOffset.X,
		origPos.m_ptMinColon.Y + ptTimeOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(clockObjects[FROM_E(CLOCK_OBJECTS::TEN_MIN)],
		origPos.m_ptTenMin.X + ptTimeOffset.X,
		origPos.m_ptTenMin.Y + ptTimeOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(clockObjects[FROM_E(CLOCK_OBJECTS::MIN)],
		origPos.m_ptMin.X + ptTimeOffset.X,
		origPos.m_ptMin.Y + ptTimeOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(clockObjects[FROM_E(CLOCK_OBJECTS::SEC_COLON)],
		origPos.m_ptSecColon.X + ptTimeOffset.X,
		origPos.m_ptSecColon.Y + ptTimeOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(clockObjects[FROM_E(CLOCK_OBJECTS::TEN_SEC)],
		origPos.m_ptTenSec.X + ptTimeOffset.X,
		origPos.m_ptTenSec.Y + ptTimeOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(clockObjects[FROM_E(CLOCK_OBJECTS::SEC)],
		origPos.m_ptSec.X + ptTimeOffset.X,
		origPos.m_ptSec.Y + ptTimeOffset.Y,
		CLOCK_PAGE_NUMBER);
}

BOOL CClock2026Dlg::DisplayClock(void)
{
	if (!m_pStatus->GetDispState(DISP_UPPER_CLOCK))
	{
		if (!OpenFpg(_T("상단시계"), CLOCK_PAGE_NUMBER))
		{
			return FALSE;
		}
	}

	CTimeString& timeString = m_pStatus->m_TimeString.GetCurrentTimeString(TRUE, m_pStatus->ClockConf()->B24Hour());

	CString strDate;
	CString strHour;

	GetDateForClock(timeString, strDate);
	GetHourForClock(timeString, strHour);

	INT nMin = GETINT(timeString.m_strMin10) * 10 + GETINT(timeString.m_strMin1);
	INT nSecond = GETINT(timeString.m_strSec10) * 10 + GETINT(timeString.m_strSec1);
	INT nSecOver = GETINT(m_pStatus->ClockConf()->SecOver());
	INT nSecUnder = GETINT(m_pStatus->ClockConf()->SecUnder());

	BOOL bShowSec = FALSE;

	if (m_pStatus->ClockConf()->DispSecond() ||	m_pStatus->ClockConf()->DispEveryHourSecond())
	{
		bShowSec = m_pStatus->ClockConf()->DispSecond() ||
			      (m_pStatus->ClockConf()->DispEveryHourSecond() &&
				  (nSecond >= nSecOver && nMin == 59) ||
				  (nSecond <= nSecUnder && nMin == 0));
	}

	m_pHDdaVinci->ObjectReplace(clockObjects[FROM_E(CLOCK_OBJECTS::DATE)],strDate, 0,CLOCK_PAGE_NUMBER);
	m_pHDdaVinci->ObjectReplace(clockObjects[FROM_E(CLOCK_OBJECTS::HOUR)],strHour, 0, CLOCK_PAGE_NUMBER);
	m_pHDdaVinci->ObjectReplace(clockObjects[FROM_E(CLOCK_OBJECTS::TEN_MIN)],timeString.m_strMin10, 1, CLOCK_PAGE_NUMBER);
	m_pHDdaVinci->ObjectReplace(clockObjects[FROM_E(CLOCK_OBJECTS::MIN)],timeString.m_strMin1, 1, CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->GetCGXObject(clockObjects[FROM_E(CLOCK_OBJECTS::SEC_COLON)],CLOCK_PAGE_NUMBER)->SetShow(bShowSec);

	if (bShowSec)
	{	
		m_pHDdaVinci->ObjectReplace(clockObjects[FROM_E(CLOCK_OBJECTS::TEN_SEC)],timeString.m_strSec10, 1,CLOCK_PAGE_NUMBER);
		m_pHDdaVinci->ObjectReplace(clockObjects[FROM_E(CLOCK_OBJECTS::SEC)],timeString.m_strSec1, 1,CLOCK_PAGE_NUMBER);
	}

	m_pHDdaVinci->GetCGXLayer(clockLayers[FROM_E(CLOCK_LAYERS::SEC)], CLOCK_PAGE_NUMBER)->SetShow(bShowSec);

#ifdef _TAKE_IN_LOCK_ 
	std::lock_guard<std::mutex> lock(m_mutexHandler);
#endif

	SetPosClockObjects();

	m_pHDdaVinci->TakeInEx(CLOCK_PAGE_NUMBER);

	return TRUE;
}

LRESULT CClock2026Dlg::OnDispCheck(WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	INT nDispID = -1;
	INT nParam = 0;
	DISP_CHECK_ARG* pArg = nullptr;
	GET_POS yesNo;

	if (lParam == NO_USE)
	{
		pArg = reinterpret_cast<DISP_CHECK_ARG*>(wParam);
		nDispID = pArg->nDispID;
		nParam = pArg->nParam;
	}
	else
	{
		nDispID = static_cast<INT>(wParam);
		nParam = static_cast<INT>(lParam);
	}

	switch (nDispID)
	{
	case DISP_UPPER_CLOCK:
		yesNo = TO_E<GET_POS>(nParam);
		if (m_pStatus->GetDispState(DISP_UPPER_CLOCK)) DisplayClock();
		break;

	case DISP_BIG_CLOCK:
		yesNo = TO_E<GET_POS>(nParam);
		if (m_pStatus->GetDispState(DISP_BIG_CLOCK)) DisplayBigClock();
		break;

	case DISP_STATION_LOGO:
		if (m_pStatus->GetDispState(DISP_STATION_LOGO)) DisplayStationLogo();
		break;

	default:
		break;
	}

	return 0;
}

UINT CClock2026Dlg::DisplayClockThread(LPVOID pParam)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(pParam);

	while (pDlg->m_pStatus->GetDispState(DISP_UPPER_CLOCK))
	{
		DWORD dwCurTick = GetTickCount64();

		if (dwCurTick - pDlg->m_dwPrevTickDisplayClock > THOUSAND)
		{
			pDlg->m_dwPrevTickDisplayClock = dwCurTick;
			pDlg->DisplayClock();
		}
	}
	return 0;
}

BOOL CClock2026Dlg::TakeOutClock()
{

	if (!m_pStatus->GetDispState(DISP_UPPER_CLOCK)) return FALSE;

	m_pStatus->SetDispStateForPage(DISP_UPPER_CLOCK, FALSE, CLOCK_PAGE_NUMBER);
	
	m_pHDdaVinci->TakeOut(CLOCK_PAGE_NUMBER);

#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(CLOCK_PAGE_NUMBER);
#endif

	PrintLog(_T("상단시계를 아웃했습니다."));

	EnableClockWindow();

	return TRUE;
}

void CClock2026Dlg::OnBnClickedCheckDispClock()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_CLOCK);

	if (bDisp)
	{
		CancelPrepareNewsScroll();
		
		bDisp = DisplayClock();
		m_pStatus->SetDispStateForPage(DISP_UPPER_CLOCK, bDisp, CLOCK_PAGE_NUMBER);

		if (bDisp)
		{
			m_dwPrevTickDisplayClock = 0;
			SetTimer(ID_TIMER_DISPLAY_CLOCK, THOUSAND, nullptr);
		//	PrintLog(_T("상단시계를 송출했습니다."),_T("알림"),TRUE);
			PrintLog(_T("상단시계를 송출했습니다."));
		}
		else
		{
			PrintLog(_T("상단시계 송출 실패했습니다."));
		}

		EnableClockWindow();

	}
	else
	{
		TakeOutClock();
	}

	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedCheckClockMoveEnable()
{
	m_pStatus->ClockConf()->MoveEnable(IsDlgButtonChecked(IDC_CHECK_CLOCK_MOVE_ENABLE));
	EnableClockWindow();
	SetChangeMark(TRUE);
	
}

void CClock2026Dlg::OnBnClickedCheckClockEntireMove()
{
	BOOL bEntireMove = IsDlgButtonChecked(IDC_CHECK_CLOCK_ENTIRE_MOVE);

	m_pStatus->ClockConf()->EntireMove(bEntireMove);
	m_pStatus->ClockConf()->DateMove(bEntireMove);
	m_pStatus->ClockConf()->TimeMove(bEntireMove);
	
	EnableClockWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckClockDateMove()
{
	m_pStatus->ClockConf()->DateMove(IsDlgButtonChecked(IDC_CHECK_CLOCK_DATE_MOVE));

	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckClockTimeMove()
{
	m_pStatus->ClockConf()->TimeMove(IsDlgButtonChecked(IDC_CHECK_CLOCK_TIME_MOVE));

	SetChangeMark(TRUE);

}

void CClock2026Dlg::OnEnKillfocusEditClockMoveSize()
{
	CString strBuffer;

	GetDlgItemText(IDC_EDIT_CLOCK_MOVE_SIZE, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->ClockConf()->MovingSize(GETINT(strBuffer));
		SetChangeMark(TRUE);
	}

	SetDlgItemText(IDC_EDIT_CLOCK_MOVE_SIZE, GETSTR(m_pStatus->ClockConf()->MovingSize()));
}

void CClock2026Dlg::OnEnKillfocusEditClockSecOver()
{
	CString strBuffer;

	GetDlgItemText(IDC_EDIT_CLOCK_SEC_OVER, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->ClockConf()->SecOver(strBuffer);
		SetChangeMark(TRUE);
	}

	SetDlgItemText(IDC_EDIT_CLOCK_SEC_OVER, m_pStatus->ClockConf()->SecOver());
}

void CClock2026Dlg::OnEnKillfocusEditClockSecUnder()
{
	CString strBuffer;

	GetDlgItemText(IDC_EDIT_CLOCK_SEC_UNDER, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->ClockConf()->SecUnder(strBuffer);
		SetChangeMark(TRUE);
	}

	SetDlgItemText(IDC_EDIT_CLOCK_SEC_UNDER, m_pStatus->ClockConf()->SecUnder());
}

void CClock2026Dlg::ClockMoveUp()
{
	BOOL bEntireMove = m_pStatus->ClockConf()->EntireMove();

	if (m_pStatus->ClockConf()->DateMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->DateOffset();
		ptOffset.Y -= m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->DateOffset(ptOffset);
	}

	if (m_pStatus->ClockConf()->TimeMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->TimeOffset();
		ptOffset.Y -= m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->TimeOffset(ptOffset);
	}
}

void CClock2026Dlg::ClockMoveLeft()
{
	BOOL bEntireMove = m_pStatus->ClockConf()->EntireMove();

	if (m_pStatus->ClockConf()->DateMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->DateOffset();
		ptOffset.X -= m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->DateOffset(ptOffset);
	}

	if (m_pStatus->ClockConf()->TimeMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->TimeOffset();
		ptOffset.X -= m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->TimeOffset(ptOffset);
	}
}

void CClock2026Dlg::ClockMoveDown()
{
	BOOL bEntireMove = m_pStatus->ClockConf()->EntireMove();

	if (m_pStatus->ClockConf()->DateMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->DateOffset();
		ptOffset.Y += m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->DateOffset(ptOffset);
	}

	if (m_pStatus->ClockConf()->TimeMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->TimeOffset();
		ptOffset.Y += m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->TimeOffset(ptOffset);
	}
}

void CClock2026Dlg::ClockMoveRight()
{
	BOOL bEntireMove = m_pStatus->ClockConf()->EntireMove();

	if (m_pStatus->ClockConf()->DateMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->DateOffset();
		ptOffset.X += m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->DateOffset(ptOffset);
	}

	if (m_pStatus->ClockConf()->TimeMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->ClockConf()->TimeOffset();
		ptOffset.X += m_pStatus->ClockConf()->MovingSize();
		m_pStatus->ClockConf()->TimeOffset(ptOffset);
	}
}

void CClock2026Dlg::ClockMove(DIRECTION dir)
{
	switch (dir)
	{
	case DIRECTION::UP:
		ClockMoveUp();
		break;
	case DIRECTION::LEFT:
		ClockMoveLeft();
		break;
	case DIRECTION::DOWN:
		ClockMoveDown();
		break;
	case DIRECTION::RIGHT:
		ClockMoveRight();
		break;
	default:
		break;
	}

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedButtonClockUp()
{
	ClockMove(DIRECTION::UP);
}

void CClock2026Dlg::OnBnClickedButtonClockLeft()
{
	ClockMove(DIRECTION::LEFT);
}

void CClock2026Dlg::OnBnClickedButtonClockDown()
{
	ClockMove(DIRECTION::DOWN);
}

void CClock2026Dlg::OnBnClickedButtonClockRight()
{
	ClockMove(DIRECTION::RIGHT);
}

void CClock2026Dlg::ClockResetPos()
{
	Point ptOffset(0, 0);

	m_pStatus->ClockConf()->DateOffset(ptOffset);
	m_pStatus->ClockConf()->TimeOffset(ptOffset);

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));
}

void CClock2026Dlg::OnBnClickedButtonClockResetPos()
{
	ClockResetPos();
}

void CClock2026Dlg::DeleteDateFormat()
{
	CString strFormat;
	CString strMsg;

	if (m_pComboDateFormat->GetCount() < 2) return;

	m_pComboDateFormat->GetWindowText(strFormat);

	int n = FindStringFromComboBox(strFormat, m_pComboDateFormat);

	if (n == -1) return;

	strMsg.Format(_T("\"%s\" 항목을 지우시겠습니까 ?"), strFormat);

	INT nResult = MessageBox(strMsg, _T("경고"), MB_ICONEXCLAMATION | MB_YESNO);

	if (nResult == IDYES)
	{
		m_pComboDateFormat->DeleteString(n);
		if (m_pComboDateFormat->GetCount() > 2)
		{
			m_pComboDateFormat->SetCurSel(n);
			m_pComboDateFormat->GetLBText(n, strFormat);

			m_pStatus->ClockConf()->FormatIndex(n);
		}
		else
		{
			m_pComboDateFormat->SetCurSel(0);
			m_pComboDateFormat->GetLBText(0, strFormat);

			m_pStatus->ClockConf()->FormatIndex(n);
		}

		if (strFormat.IsEmpty())
		{
			strFormat = DEFAULT_DATE_FORMAT;
			m_pComboDateFormat->InsertString(0, strFormat);

			m_pStatus->ClockConf()->FormatIndex(0);
		}

		m_pStatus->ClockConf()->Format(strFormat);

		UpdateComboData(TRUE);

		SetChangeMark(TRUE);
		UpdateData(FALSE);
	}
}

void CClock2026Dlg::OnBnClickedButtonDeleteDateFormat()
{
	DeleteDateFormat();
}

void CClock2026Dlg::InsertDateFormat()
{
	CString strFormat;

	m_pComboDateFormat->GetDlgItemText(1001, strFormat);
	strFormat.Trim(SPACE_STRING);
	if (strFormat != BLANK_STRING)
	{
		int nIndex = FindStringFromComboBox(strFormat, m_pComboDateFormat);

		if (nIndex == CB_ERR)
		{
			m_pComboDateFormat->InsertString(0, strFormat);
			m_pStatus->ClockConf()->FormatIndex(0);
			m_pStatus->ClockConf()->Format(strFormat);
			m_pComboDateFormat->SetCurSel(0);
			UpdateComboData(TRUE);
		}
		else
		{
			m_pStatus->ClockConf()->FormatIndex(nIndex);
			m_pStatus->ClockConf()->Format(strFormat);
			m_pComboDateFormat->SetCurSel(nIndex);
		}
	}
}

void CClock2026Dlg::OnCbnKillfocusComboDateFormat()
{
	InsertDateFormat();
}

void CClock2026Dlg::OnCbnSelchangeComboDateFormat()
{
	CString strFormat;
	int nIndex = m_pComboDateFormat->GetCurSel();
	m_pComboDateFormat->GetLBText(nIndex, strFormat);

	m_pStatus->ClockConf()->FormatIndex(nIndex);
	m_pStatus->ClockConf()->Format(strFormat);

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedCheckClock24Hour()
{
	m_pStatus->ClockConf()->B24Hour(IsDlgButtonChecked(IDC_CHECK_CLOCK_24_HOUR));

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedCheckClockDispSecond()
{
	m_pStatus->ClockConf()->DispSecond(IsDlgButtonChecked(IDC_CHECK_CLOCK_DISP_SECOND));

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedCheckClockDispEveryHourSec()
{
	m_pStatus->ClockConf()->DispEveryHourSecond(IsDlgButtonChecked(IDC_CHECK_CLOCK_DISP_EVERY_HOUR_SEC));

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedCheckClockHideFirstDigit0OfHour()
{
	m_pStatus->ClockConf()->HideFirstDigit0OfHour(IsDlgButtonChecked(IDC_CHECK_CLOCK_HIDE_FIRST_DIGIT_0_OF_HOUR));

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::ClockSnapSave(INT nSnapNumber)
{
	CString strTitle = m_pStatus->ClockSnap(nSnapNumber).Title();
	CInputDlg dlg(strTitle);

	if (dlg.DoModal() == IDOK)
	{
		m_pStatus->SaveClockSnap(nSnapNumber);

		strTitle = dlg.GetData();
		m_pStatus->ClockSnap(nSnapNumber).Title(strTitle);
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_CLOCK_SNAP_LOAD_1+ nSnapNumber), strTitle);
	}
}

void CClock2026Dlg::OnBnClickedButtonClockSnapSave1()
{
	ClockSnapSave(0);
}

void CClock2026Dlg::OnBnClickedButtonClockSnapSave2()
{
	ClockSnapSave(1);
}

void CClock2026Dlg::OnBnClickedButtonClockSnapSave3()
{
	ClockSnapSave(2);
}

void CClock2026Dlg::ClockSnapLoad(INT nSnapNumber)
{
	m_pStatus->LoadClockSnap(nSnapNumber);

	SendMessage(WM_DISP_CHECK, DISP_UPPER_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);

	EnableClockWindow();

	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonClockSnapLoad1()
{
	ClockSnapLoad(0);
}

void CClock2026Dlg::OnBnClickedButtonClockSnapLoad2()
{
	ClockSnapLoad(1);
}

void CClock2026Dlg::OnBnClickedButtonClockSnapLoad3()
{
	ClockSnapLoad(2);

}

void CClock2026Dlg::OnBnClickedCheckBigClockMoveEnable()
{
	m_pStatus->BigClockConf()->MoveEnable(IsDlgButtonChecked(IDC_CHECK_BIG_CLOCK_MOVE_ENABLE));

	EnableBigClockWindow();

	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnEnKillfocusEditBigClockMoveSize()
{
	CString strBuffer;

	GetDlgItemText(IDC_EDIT_BIG_CLOCK_MOVE_SIZE, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->BigClockConf()->MovingSize(GETINT(strBuffer));
		SetChangeMark(TRUE);
	}

	SetDlgItemText(IDC_EDIT_BIG_CLOCK_MOVE_SIZE, GETSTR(m_pStatus->BigClockConf()->MovingSize()));
}

void CClock2026Dlg::BigClockMoveUp()
{
	if (m_pStatus->BigClockConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->BigClockConf()->Offset();
		ptOffset.Y -= m_pStatus->BigClockConf()->MovingSize();
		m_pStatus->BigClockConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::BigClockMoveLeft()
{
	if (m_pStatus->BigClockConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->BigClockConf()->Offset();
		ptOffset.X -= m_pStatus->BigClockConf()->MovingSize();
		m_pStatus->BigClockConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::BigClockMoveDown()
{
	if (m_pStatus->BigClockConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->BigClockConf()->Offset();
		ptOffset.Y += m_pStatus->BigClockConf()->MovingSize();
		m_pStatus->BigClockConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::BigClockMoveRight()
{
	if (m_pStatus->BigClockConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->BigClockConf()->Offset();
		ptOffset.X += m_pStatus->BigClockConf()->MovingSize();
		m_pStatus->BigClockConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::BigClockMove(DIRECTION dir)
{
	switch (dir)
	{
	case DIRECTION::UP:
		BigClockMoveUp();
		break;
	case DIRECTION::LEFT:
		BigClockMoveLeft();
		break;
	case DIRECTION::DOWN:
		BigClockMoveDown();
		break;
	case DIRECTION::RIGHT:
		BigClockMoveRight();
		break;
	default:
		break;
	}

	SendMessage(WM_DISP_CHECK, DISP_BIG_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
}

void CClock2026Dlg::BigClockResetPos()
{
	Point ptOffset(0, 0);

	m_pStatus->BigClockConf()->Offset(ptOffset);

	SendMessage(WM_DISP_CHECK, DISP_BIG_CLOCK, FROM_E<GET_POS>(GET_POS::NO));
}

void CClock2026Dlg::GetOrigPosBigClockObjects(void)
{
	if (!OpenFpg(_T("시보시계"), CLOCK_PAGE_NUMBER))
	{
		return;
	}

	CCGXObject* pObjects[FROM_E(BIG_CLOCK_OBJECTS::LAST_NUM)];

	for (int i = 0; i < FROM_E(BIG_CLOCK_OBJECTS::LAST_NUM); i++)
	{
		pObjects[i] = m_pHDdaVinci->GetCGXObject(bigClockObjects[i], CLOCK_PAGE_NUMBER);
	}

	pObjects[FROM_E(BIG_CLOCK_OBJECTS::HOUR)]->GetPositionEx(
		&(m_pStatus->BigClockConf()->OrigPos().m_ptHour.X),
		&(m_pStatus->BigClockConf()->OrigPos().m_ptHour.Y),
		CGX_REFERENCE_POINT_RIGHTCENTER);

	pObjects[FROM_E(BIG_CLOCK_OBJECTS::MIN_COLON)]->GetPosition(
		&(m_pStatus->BigClockConf()->OrigPos().m_ptMinColon.X),
		&(m_pStatus->BigClockConf()->OrigPos().m_ptMinColon.Y));

	pObjects[FROM_E(BIG_CLOCK_OBJECTS::TEN_MIN)]->GetPosition(
		&(m_pStatus->BigClockConf()->OrigPos().m_ptTenMin.X),
		&(m_pStatus->BigClockConf()->OrigPos().m_ptTenMin.Y));

	pObjects[FROM_E(BIG_CLOCK_OBJECTS::MIN)]->GetPosition(
		&(m_pStatus->BigClockConf()->OrigPos().m_ptMin.X),
		&(m_pStatus->BigClockConf()->OrigPos().m_ptMin.Y));

	pObjects[FROM_E(BIG_CLOCK_OBJECTS::SEC_COLON)]->GetPosition(
		&(m_pStatus->BigClockConf()->OrigPos().m_ptSecColon.X),
		&(m_pStatus->BigClockConf()->OrigPos().m_ptSecColon.Y));

	pObjects[FROM_E(BIG_CLOCK_OBJECTS::TEN_SEC)]->GetPosition(
		&(m_pStatus->BigClockConf()->OrigPos().m_ptTenSec.X),
		&(m_pStatus->BigClockConf()->OrigPos().m_ptTenSec.Y));

	pObjects[FROM_E(BIG_CLOCK_OBJECTS::SEC)]->GetPosition(
		&(m_pStatus->BigClockConf()->OrigPos().m_ptSec.X),
		&(m_pStatus->BigClockConf()->OrigPos().m_ptSec.Y));
}

void CClock2026Dlg::SetPosBigClockObjects(void)
{

	SClockPos origPos = m_pStatus->BigClockConf()->OrigPos();
	Point ptOffset = m_pStatus->BigClockConf()->Offset();

	m_pHDdaVinci->ObjectSetPositionByAbsEx(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::HOUR)],
		origPos.m_ptHour.X + ptOffset.X,
		origPos.m_ptHour.Y + ptOffset.Y,
		CGX_REFERENCE_POINT_RIGHTCENTER,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::MIN_COLON)],
		origPos.m_ptMinColon.X + ptOffset.X,
		origPos.m_ptMinColon.Y + ptOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::TEN_MIN)],
		origPos.m_ptTenMin.X + ptOffset.X,
		origPos.m_ptTenMin.Y + ptOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::MIN)],
		origPos.m_ptMin.X + ptOffset.X,
		origPos.m_ptMin.Y + ptOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::SEC_COLON)],
		origPos.m_ptSecColon.X + ptOffset.X,
		origPos.m_ptSecColon.Y + ptOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::TEN_SEC)],
		origPos.m_ptTenSec.X + ptOffset.X,
		origPos.m_ptTenSec.Y + ptOffset.Y,
		CLOCK_PAGE_NUMBER);

	m_pHDdaVinci->ObjectSetPositionByAbs(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::SEC)],
		origPos.m_ptSec.X + ptOffset.X,
		origPos.m_ptSec.Y + ptOffset.Y,
		CLOCK_PAGE_NUMBER);
}

BOOL CClock2026Dlg::DisplayBigClock(void)
{
	if (!m_pStatus->GetDispState(DISP_BIG_CLOCK))
	{
		if (!OpenFpg(_T("시보시계"), CLOCK_PAGE_NUMBER))
		{
			return FALSE;
		}
	}

	CTimeString& timeString = m_pStatus->m_TimeString.GetCurrentTimeString(TRUE, m_pStatus->ClockConf()->B24Hour());

	CString strHour;
	GetHourForClock(timeString, strHour);

	INT nMin = GETINT(timeString.m_strMin10) * 10 + GETINT(timeString.m_strMin1);
	INT nSecond = GETINT(timeString.m_strSec10) * 10 + GETINT(timeString.m_strSec1);

	m_pHDdaVinci->ObjectReplace(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::HOUR)],strHour, 0,CLOCK_PAGE_NUMBER);
	m_pHDdaVinci->ObjectReplace(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::TEN_MIN)], timeString.m_strMin10, 1, CLOCK_PAGE_NUMBER);
	m_pHDdaVinci->ObjectReplace(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::MIN)], timeString.m_strMin1, 1, CLOCK_PAGE_NUMBER);
	m_pHDdaVinci->ObjectReplace(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::TEN_SEC)], timeString.m_strSec10, 1, CLOCK_PAGE_NUMBER);
	m_pHDdaVinci->ObjectReplace(bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::SEC)], timeString.m_strSec1, 1, CLOCK_PAGE_NUMBER);
	
#ifdef _TAKE_IN_LOCK_ 
	std::lock_guard<std::mutex> lock(m_mutexHandler);
#endif

	SetPosBigClockObjects();

	m_pHDdaVinci->TakeInEx(CLOCK_PAGE_NUMBER);

	return TRUE;
}


UINT CClock2026Dlg::DisplayBigClockThread(LPVOID pParam)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(pParam);

	while (pDlg->m_pStatus->GetDispState(DISP_BIG_CLOCK))
	{
		DWORD dwCurTick = GetTickCount64();

		if (dwCurTick - pDlg->m_dwPrevTickDisplayBigClock > THOUSAND)
		{
			pDlg->DisplayBigClock();
			pDlg->m_dwPrevTickDisplayBigClock = dwCurTick;
		}
	}
	return 0;
}

BOOL CClock2026Dlg::TakeOutBigClock(void)
{
	if (!m_pStatus->GetDispState(DISP_BIG_CLOCK)) return FALSE;

	m_pStatus->SetDispStateForPage(DISP_BIG_CLOCK, FALSE, CLOCK_PAGE_NUMBER);
	
	m_pHDdaVinci->TakeOut(CLOCK_PAGE_NUMBER);

#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(CLOCK_PAGE_NUMBER);
#endif

	PrintLog(_T("시보시계를 아웃했습니다."));

	EnableBigClockWindow();

	return TRUE;
}

void CClock2026Dlg::OnBnClickedCheckDispBigClock()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_BIG_CLOCK);

	if (bDisp)
	{
		CancelPrepareNewsScroll();

		bDisp = DisplayBigClock();

		m_pStatus->SetDispStateForPage(DISP_BIG_CLOCK, bDisp, CLOCK_PAGE_NUMBER);

		if (bDisp)
		{	
			m_dwPrevTickDisplayBigClock = 0;
			SetTimer(ID_TIMER_DISPLAY_BIG_CLOCK, THOUSAND, nullptr);
		
			PrintLog(_T("시보시계를 송출했습니다."));
		}
		else
		{
			PrintLog(_T("시보시계 송출 실패했습니다."));
		}

		EnableBigClockWindow();
	}
	else
	{
		TakeOutBigClock();
	}

	UpdateData(FALSE);
	
}

void CClock2026Dlg::OnBnClickedButtonBigClockUp()
{
	BigClockMove(DIRECTION::UP);
}

void CClock2026Dlg::OnBnClickedButtonBigClockRight()
{
	BigClockMove(DIRECTION::RIGHT);
}

void CClock2026Dlg::OnBnClickedButtonBigClockDown()
{
	BigClockMove(DIRECTION::DOWN);
}

void CClock2026Dlg::OnBnClickedButtonBigClockLeft()
{
	BigClockMove(DIRECTION::LEFT);
}

void CClock2026Dlg::OnBnClickedButtonBigClockResetPos()
{
	BigClockResetPos();
}

void CClock2026Dlg::BigClockSnapSave(INT nSnapNumber)
{
	CString strTitle = m_pStatus->BigClockSnap(nSnapNumber).Title();
	CInputDlg dlg(strTitle);

	if (dlg.DoModal() == IDOK)
	{
		m_pStatus->SaveBigClockSnap(nSnapNumber);

		strTitle = dlg.GetData();
		m_pStatus->BigClockSnap(nSnapNumber).Title(strTitle);
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_1 + nSnapNumber), strTitle);
	}
}

void CClock2026Dlg::OnBnClickedButtonBigClockSnapSave1()
{
	BigClockSnapSave(0);
}

void CClock2026Dlg::OnBnClickedButtonBigClockSnapSave2()
{
	BigClockSnapSave(1);
}

void CClock2026Dlg::OnBnClickedButtonBigClockSnapSave3()
{
	BigClockSnapSave(2);
	
}

void CClock2026Dlg::BigClockSnapLoad(INT nSnapNumber)
{
	m_pStatus->LoadBigClockSnap(nSnapNumber);

	SendMessage(WM_DISP_CHECK, DISP_BIG_CLOCK, FROM_E<GET_POS>(GET_POS::NO));

	EnableBigClockWindow();

	SetChangeMark(TRUE);

	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonBigClockSnapLoad1()
{
	BigClockSnapLoad(0);
}

void CClock2026Dlg::OnBnClickedButtonBigClockSnapLoad2()
{
	BigClockSnapLoad(1);
}

void CClock2026Dlg::OnBnClickedButtonBigClockSnapLoad3()
{
	BigClockSnapLoad(2);
}

void CClock2026Dlg::OnBnClickedCheckLogoEntireMove()
{
	BOOL bEntireMove = IsDlgButtonChecked(IDC_CHECK_LOGO_ENTIRE_MOVE);

	m_pStatus->LogoConf()->EntireMove(bEntireMove);
	EnableLogoWindow();
	SetChangeMark(TRUE);

}

void CClock2026Dlg::OnBnClickedCheckLogoMove()
{
	m_pStatus->LogoConf()->LogoMove(IsDlgButtonChecked(IDC_CHECK_LOGO_MOVE));
	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoLiveMove()
{
	m_pStatus->LogoConf()->LiveMove(IsDlgButtonChecked(IDC_CHECK_LOGO_LIVE_MOVE));
	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoUhdMove()
{
	m_pStatus->LogoConf()->UHDMove(IsDlgButtonChecked(IDC_CHECK_LOGO_UHD_MOVE));
	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoDongsiMove()
{
	m_pStatus->LogoConf()->DongSiMove(IsDlgButtonChecked(IDC_CHECK_LOGO_DONGSI_MOVE));
	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoEntireShow()
{
	BOOL bEntireShow = IsDlgButtonChecked(IDC_CHECK_LOGO_ENTIRE_SHOW);

	m_pStatus->LogoConf()->EntireShow(bEntireShow);

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));
	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoShow()
{
	m_pStatus->LogoConf()->LogoShow(IsDlgButtonChecked(IDC_CHECK_LOGO_SHOW));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoLiveShow()
{
	m_pStatus->LogoConf()->LiveShow(IsDlgButtonChecked(IDC_CHECK_LOGO_LIVE_SHOW));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoUhdShow()
{
	m_pStatus->LogoConf()->UHDShow(IsDlgButtonChecked(IDC_CHECK_LOGO_UHD_SHOW));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedCheckLogoDongsiShow()
{
	m_pStatus->LogoConf()->DongSiShow(IsDlgButtonChecked(IDC_CHECK_LOGO_DONGSI_SHOW));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedRadioLogoKbs1()
{
	m_pStatus->LogoConf()->KBSLogo(FROM_E<STATION_LOGO>(STATION_LOGO::KBS_1));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);

}

void CClock2026Dlg::OnBnClickedRadioLogoKbs2()
{
	m_pStatus->LogoConf()->KBSLogo(FROM_E<STATION_LOGO>(STATION_LOGO::KBS_2));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedRadioLogoKbsLocal()
{
	m_pStatus->LogoConf()->KBSLogo(FROM_E<STATION_LOGO>(STATION_LOGO::KBS_LOCAL));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedRadioLogoKbsAnimation()
{
	m_pStatus->LogoConf()->KBSLogo(FROM_E<STATION_LOGO>(STATION_LOGO::KBS_ANI));

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	EnableLogoWindow();
	SetChangeMark(TRUE);

}

void CClock2026Dlg::SetPosLogoObjects(void)
{
	BOOL bEntireShow = m_pStatus->LogoConf()->EntireShow();

	Point ptLogoOffset = m_pStatus->LogoConf()->LogoOffset();
	Point ptLiveOffset = m_pStatus->LogoConf()->LiveOffset();
	Point ptUHDOffset = m_pStatus->LogoConf()->UHDOffset();
	Point ptDongSiOffset = m_pStatus->LogoConf()->DongSiOffset();

	if (m_pStatus->LogoConf()->LogoShow() || bEntireShow)
	{
		m_pHDdaVinci->ObjectSetPositionByOffsetEx(
			logoObjects[FROM_E(LOGO_OBJECTS::LOGO)],
			ptLogoOffset.X,
			ptLogoOffset.Y,
			LOGO_PAGE_NUMBER);
	}

	if (m_pStatus->LogoConf()->LiveShow() || bEntireShow)
	{
		m_pHDdaVinci->ObjectSetPositionByOffsetEx(
			logoObjects[FROM_E(LOGO_OBJECTS::LIVE)],
			ptLiveOffset.X,
			ptLiveOffset.Y,
			LOGO_PAGE_NUMBER);
	}

	if (m_pStatus->LogoConf()->UHDShow() || bEntireShow)
	{
		m_pHDdaVinci->ObjectSetPositionByOffsetEx(
			logoObjects[FROM_E(LOGO_OBJECTS::UHD)],
			ptUHDOffset.X,
			ptUHDOffset.Y,
			LOGO_PAGE_NUMBER);
	}

	if (m_pStatus->LogoConf()->DongSiShow() || bEntireShow)
	{
		m_pHDdaVinci->ObjectSetPositionByOffsetEx(
			logoObjects[FROM_E(LOGO_OBJECTS::DONGSI)],
			ptDongSiOffset.X,
			ptDongSiOffset.Y,
			LOGO_PAGE_NUMBER);
	}
}

BOOL CClock2026Dlg::DisplayStationLogo(void)
{

	switch (TO_E<STATION_LOGO>(m_pStatus->LogoConf()->KBSLogo()))
	{
	case STATION_LOGO::KBS_1:
		if (!OpenFpg(_T("KBS1로고"), LOGO_PAGE_NUMBER))
		{
			return FALSE;
		}
		break;

	case STATION_LOGO::KBS_2:
		if (!OpenFpg(_T("KBS2로고"), LOGO_PAGE_NUMBER))
		{
			return FALSE;
		}
		break;

	case STATION_LOGO::KBS_LOCAL:
		if (!OpenFpg(_T("KBS지역로고"), LOGO_PAGE_NUMBER))
		{
			return FALSE;
		}
		break;

	case STATION_LOGO::KBS_ANI:
		if (m_pHDdaVinci->IsTransmitting(LOGO_PAGE_NUMBER)) return FALSE;
		if (!OpenFpg(_T("KBS로고애니"), LOGO_PAGE_NUMBER))
		{
			return FALSE;
		}
		break;


	default:
		if (!OpenFpg(_T("KBS지역로고"), LOGO_PAGE_NUMBER))
		{
			return FALSE;
		}
		break;
	}

	SetPosLogoObjects();

	BOOL bEntireShow = m_pStatus->LogoConf()->EntireShow();

	m_pHDdaVinci->GetCGXObject(logoObjects[FROM_E(LOGO_OBJECTS::LOGO)], LOGO_PAGE_NUMBER)->SetShow(m_pStatus->LogoConf()->LogoShow() || bEntireShow);
	m_pHDdaVinci->GetCGXObject(logoObjects[FROM_E(LOGO_OBJECTS::LIVE)], LOGO_PAGE_NUMBER)->SetShow(m_pStatus->LogoConf()->LiveShow() || bEntireShow);
	m_pHDdaVinci->GetCGXObject(logoObjects[FROM_E(LOGO_OBJECTS::UHD)], LOGO_PAGE_NUMBER)->SetShow(m_pStatus->LogoConf()->UHDShow() || bEntireShow);
	m_pHDdaVinci->GetCGXObject(logoObjects[FROM_E(LOGO_OBJECTS::DONGSI)], LOGO_PAGE_NUMBER)->SetShow(m_pStatus->LogoConf()->DongSiShow() || bEntireShow);

#ifdef _TAKE_IN_LOCK_ 
	std::lock_guard<std::mutex> lock(m_mutexHandler);
#endif
	m_pHDdaVinci->TakeInEx(LOGO_PAGE_NUMBER);

	return TRUE;
}

UINT CClock2026Dlg::DisplayLogoThread(LPVOID pParam)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(pParam);

	while (pDlg->m_pStatus->GetDispState(DISP_STATION_LOGO))
	{
		pDlg->DisplayStationLogo();
	}

	return 0;
}

BOOL CClock2026Dlg::TakeOutLogo()
{
	if (!m_pStatus->GetDispState(DISP_STATION_LOGO)) return FALSE;

	m_pStatus->SetDispStateForPage(DISP_STATION_LOGO, FALSE, LOGO_PAGE_NUMBER);

	m_pHDdaVinci->TakeOut(LOGO_PAGE_NUMBER);

	KillTimer(ID_TIMER_DISPLAY_STATION_LOGO);

#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(LOGO_PAGE_NUMBER);
#endif
	PrintLog(_T("KBS 로고를 아웃했습니다."));

	EnableLogoWindow();

	return TRUE;
}

void CClock2026Dlg::OnBnClickedCheckDispStationLogo()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_STATION_LOGO);

	if (bDisp)
	{
		CancelPrepareNewsScroll();
		bDisp = DisplayStationLogo();
		m_pStatus->SetDispStateForPage(DISP_STATION_LOGO, bDisp, LOGO_PAGE_NUMBER);

		if (bDisp)
		{
			if (m_pStatus->LogoConf()->IsAnimationLogo())
			{
				SetTimer(ID_TIMER_DISPLAY_STATION_LOGO, 10, nullptr);
			}

			PrintLog(_T("KBS 로고를 송출했습니다."));

		}
		else
		{
			PrintLog(_T("KBS 로고 송출 실패했습니다."));
		}

		EnableLogoWindow();
	}
	else
	{
		TakeOutLogo();
	}

	UpdateData(FALSE);
}

void CClock2026Dlg::OnEnKillfocusEditLogoMoveSize()
{
	CString strBuffer;

	GetDlgItemText(IDC_EDIT_LOGO_MOVE_SIZE, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->LogoConf()->MovingSize(GETINT(strBuffer));
		SetChangeMark(TRUE);
	}

	SetDlgItemText(IDC_EDIT_LOGO_MOVE_SIZE, GETSTR(m_pStatus->LogoConf()->MovingSize()));
}

void CClock2026Dlg::LogoResetPos()
{
	Point ptOffset(0, 0);

	m_pStatus->LogoConf()->LogoOffset(ptOffset);
	m_pStatus->LogoConf()->LiveOffset(ptOffset);
	m_pStatus->LogoConf()->UHDOffset(ptOffset);
	m_pStatus->LogoConf()->DongSiOffset(ptOffset);

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedButtonLogoResetPos()
{
	LogoResetPos();
}

void CClock2026Dlg::LogoMoveUp()
{
	BOOL bEntireMove = m_pStatus->LogoConf()->EntireMove();

	if (m_pStatus->LogoConf()->LogoMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LogoOffset();
		ptOffset.Y -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LogoOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->LiveMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LiveOffset();
		ptOffset.Y -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LiveOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->UHDMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->UHDOffset();
		ptOffset.Y -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->UHDOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->DongSiMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->DongSiOffset();
		ptOffset.Y -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->DongSiOffset(ptOffset);
	}
}

void CClock2026Dlg::LogoMoveLeft()
{
	BOOL bEntireMove = m_pStatus->LogoConf()->EntireMove();

	if (m_pStatus->LogoConf()->LogoMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LogoOffset();
		ptOffset.X -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LogoOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->LiveMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LiveOffset();
		ptOffset.X -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LiveOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->UHDMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->UHDOffset();
		ptOffset.X -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->UHDOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->DongSiMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->DongSiOffset();
		ptOffset.X -= m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->DongSiOffset(ptOffset);
	}
}

void CClock2026Dlg::LogoMoveDown()
{
	BOOL bEntireMove = m_pStatus->LogoConf()->EntireMove();

	if (m_pStatus->LogoConf()->LogoMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LogoOffset();
		ptOffset.Y += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LogoOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->LiveMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LiveOffset();
		ptOffset.Y += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LiveOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->UHDMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->UHDOffset();
		ptOffset.Y += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->UHDOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->DongSiMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->DongSiOffset();
		ptOffset.Y += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->DongSiOffset(ptOffset);
	}
}

void CClock2026Dlg::LogoMoveRight()
{
	BOOL bEntireMove = m_pStatus->LogoConf()->EntireMove();

	if (m_pStatus->LogoConf()->LogoMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LogoOffset();
		ptOffset.X += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LogoOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->LiveMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->LiveOffset();
		ptOffset.X += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->LiveOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->UHDMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->UHDOffset();
		ptOffset.X += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->UHDOffset(ptOffset);
	}

	if (m_pStatus->LogoConf()->DongSiMove() || bEntireMove)
	{
		Point ptOffset = m_pStatus->LogoConf()->DongSiOffset();
		ptOffset.X += m_pStatus->LogoConf()->MovingSize();
		m_pStatus->LogoConf()->DongSiOffset(ptOffset);
	}
}

void CClock2026Dlg::LogoMove(DIRECTION dir)
{
	switch (dir)
	{
	case DIRECTION::UP:
		LogoMoveUp();
		break;
	case DIRECTION::LEFT:
		LogoMoveLeft();
		break;
	case DIRECTION::DOWN:
		LogoMoveDown();
		break;
	case DIRECTION::RIGHT:
		LogoMoveRight();
		break;
	default:
		break;
	}

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
}

void CClock2026Dlg::OnBnClickedButtonLogoUp()
{
	LogoMove(DIRECTION::UP);
}

void CClock2026Dlg::OnBnClickedButtonLogoRight()
{
	LogoMove(DIRECTION::RIGHT);
}

void CClock2026Dlg::OnBnClickedButtonLogoDown()
{
	LogoMove(DIRECTION::DOWN);
}

void CClock2026Dlg::OnBnClickedButtonLogoLeft()
{
	LogoMove(DIRECTION::LEFT);
}

void CClock2026Dlg::LogoSnapSave(INT nSnapNumber)
{
	CString strTitle = m_pStatus->LogoSnap(nSnapNumber).Title();
	CInputDlg dlg(strTitle);

	if (dlg.DoModal() == IDOK)
	{
		m_pStatus->SaveLogoSnap(nSnapNumber);

		strTitle = dlg.GetData();
		m_pStatus->LogoSnap(0).Title(strTitle);
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LOGO_SNAP_LOAD_1+ nSnapNumber), strTitle);
	}
}

void CClock2026Dlg::OnBnClickedButtonLogoSnapSave1()
{
	LogoSnapSave(0);
}

void CClock2026Dlg::OnBnClickedButtonLogoSnapSave2()
{
	LogoSnapSave(1);

}

void CClock2026Dlg::OnBnClickedButtonLogoSnapSave3()
{
	LogoSnapSave(2);

}

void CClock2026Dlg::LogoSnapLoad(INT nSnapNumber)
{
	m_pStatus->LoadLogoSnap(nSnapNumber);

	SendMessage(WM_DISP_CHECK, DISP_STATION_LOGO, FROM_E<GET_POS>(GET_POS::NO));

	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonLogoSnapLoad1()
{
	LogoSnapLoad(0);
}

void CClock2026Dlg::OnBnClickedButtonLogoSnapLoad2()
{
	LogoSnapLoad(1);
}

void CClock2026Dlg::OnBnClickedButtonLogoSnapLoad3()
{
	LogoSnapLoad(2);
}

void CClock2026Dlg::OnCbnSelchangeComboNewsCategory()
{
	CString strCategory;

	GetDlgItem(IDC_COMBO_NEWS_CATEGORY)->GetDlgItemText(1001, strCategory);

	if (!strCategory.IsEmpty())
	{
		m_pStatus->NewsConf()->Category(strCategory);
	}
}

BOOL CClock2026Dlg::AddNewsCategory(CString strCategory)
{
	if (!strCategory.Trim().IsEmpty())
	{

		INT nIndex = m_pComboNewsCategory->FindString(-1, strCategory);

		if (nIndex == CB_ERR)
		{
			m_pComboNewsCategory->InsertString(0, strCategory);
			m_pComboNewsCategory->SetCurSel(0);
			m_pStatus->NewsConf()->AddCategory(strCategory);
			m_SpreadNews.RefreshComboString();
			UpdateComboData(TRUE);
		}
		else
		{
			m_pComboNewsCategory->SetCurSel(nIndex);
		}

		m_pStatus->NewsConf()->Category(strCategory);
	}

	UpdateData(FALSE);

	return TRUE;
}


void CClock2026Dlg::OnCbnKillfocusComboNewsCategory()
{
	CString strCategory;

	m_pComboNewsCategory->GetDlgItemText(1001, strCategory);

	AddNewsCategory(strCategory);
}

BOOL CClock2026Dlg::DeleteNewsCategoryFromCombo(void)
{
	CString strCategory;

	if (m_pComboNewsCategory->GetCount() < 2) return FALSE;

	m_pComboNewsCategory->GetWindowText(strCategory);

	int nIndex = m_pComboNewsCategory->FindString(-1, strCategory);

	if (nIndex == CB_ERR) return FALSE;

	INT nResult = MessageBox(strCategory + _T(" 항목을 지우시겠습니까 ?"), _T("경고"), MB_ICONEXCLAMATION | MB_YESNO);

	if (nResult == IDYES)
	{
		m_pComboNewsCategory->DeleteString(nIndex);
		if (m_pComboNewsCategory->GetCount() > 2)
		{
			m_pComboNewsCategory->SetCurSel(nIndex);
		}
		else
		{
			m_pComboNewsCategory->SetCurSel(0);
		}

		m_pComboNewsCategory->GetLBText(m_pComboNewsCategory->GetCurSel(), strCategory);

		UpdateComboData(TRUE);
		SetChangeMark(TRUE);
	}

	return TRUE;
}
void CClock2026Dlg::OnBnClickedButtonDeleteNewsCategoryFromCombo()
{
	DeleteNewsCategoryFromCombo();
}

void CClock2026Dlg::OnCbnSelchangeComboNewsTextFont()
{
	CString strFontName;
	int nIndx = m_ComboNewsTextFont.GetCurSel();

	m_ComboNewsTextFont.GetLBText(nIndx, strFontName);
	
	m_pStatus->NewsConf()->DefaultFontName(strFontName);

	m_RENewsData.SetSelectionFont(strFontName);
}


void CClock2026Dlg::OnCbnKillfocusComboNewsTextFont()
{
	CString strFontName;

	m_ComboNewsTextFont.GetDlgItem(1001)->GetWindowText(strFontName);

	int nIndex = m_ComboNewsTextFont.FindString(strFontName.GetBuffer());

	if (nIndex != CB_ERR)
	{
		m_ComboNewsTextFont.SetCurSel(nIndex);
		m_ComboNewsTextFont.GetLBText(nIndex, strFontName);

		m_pStatus->NewsConf()->DefaultFontName(strFontName);

		m_RENewsData.SetSelectionFont(strFontName);
	}
	
}

void CClock2026Dlg::OnCbnSelchangeComboNewsTextSize()
{
	CString strSize;
	int nIndex = m_pComboNewsTextSize->GetCurSel();
	m_pComboNewsTextSize->GetLBText(nIndex, strSize);

	m_pStatus->NewsConf()->TextSize(strSize);

	m_RENewsData.SetSelectionSize(GETINT(strSize));
}

void CClock2026Dlg::OnCbnKillfocusComboNewsTextSize()
{	
	CString strSize;

	m_pComboNewsTextSize->GetDlgItemText(1001, strSize);

	if (!strSize.Trim().IsEmpty() && IsNumberString(strSize))
	{

		INT nIndex = FindStringFromComboBox(strSize, m_pComboNewsTextSize);

		if (nIndex == CB_ERR)
		{
			m_pComboNewsTextSize->InsertString(0, strSize);
			m_pComboNewsCategory->SetCurSel(0);
			UpdateComboData(TRUE);
		}
		else
		{
			m_pComboNewsTextSize->SetCurSel(nIndex);
		}

		m_pStatus->NewsConf()->TextSize(strSize);
	}

	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonDeleteNewsTextSizeFromCombo()
{
	CString strSize;
	
	if (m_pComboNewsTextSize->GetCount() < 2) return;

	m_pComboNewsTextSize->GetWindowText(strSize);

	int nIndex = m_pComboNewsTextSize->FindString(-1, strSize);

	if (nIndex == CB_ERR) return;

	INT nResult = MessageBox(strSize + _T("%s 크기를 지우시겠습니까 ?"), _T("경고"), MB_ICONEXCLAMATION | MB_YESNO);

	if (nResult == IDYES)
	{
		m_pComboNewsTextSize->DeleteString(nIndex);

		if (m_pComboNewsTextSize->GetCount() > 2)
		{
			m_pComboNewsTextSize->SetCurSel(nIndex);
		}
		else
		{
			m_pComboNewsTextSize->SetCurSel(0);
		}

		m_pComboNewsTextSize->GetLBText(m_pComboNewsTextSize->GetCurSel(), strSize);

		m_pStatus->NewsConf()->TextSize(strSize);

		UpdateComboData(TRUE);

		SetChangeMark(TRUE);
	}
}

void CClock2026Dlg::OnEnKillfocusEditNewsSymbolSize()
{
	CString strSize;

	GetDlgItemText(IDC_EDIT_NEWS_SYMBOL_SIZE, strSize);

	if (!strSize.Trim().IsEmpty() && !IsNumberString(strSize))
	{
		m_pStatus->NewsConf()->SymbolSize(strSize);
	}

	UpdateData(FALSE);

}

void CClock2026Dlg::OnKillfocusEditNewsHanjaSize()
{
	
	CString strSize;

	GetDlgItemText(IDC_EDIT_NEWS_HANJA_SIZE, strSize);

	if (!strSize.Trim().IsEmpty() && !IsNumberString(strSize))
	{
		m_pStatus->NewsConf()->HanjaSize(strSize);
	}

	UpdateData(FALSE);
}

void CClock2026Dlg::ChangeNewsTextColor(void)
{
	COLORREF clrNewsTextColor = m_btnNewsTextColor.GetColor();

	m_pStatus->NewsConf()->TextColor(RGBToULONG(clrNewsTextColor));

	m_RENewsData.SetSelectionColor(clrNewsTextColor);
}

void CClock2026Dlg::OnBnClickedButtonNewsTextColor()
{
	ChangeNewsTextColor();
}

void CClock2026Dlg::OnHotitemchangeButtonNewsTextColor(NMHDR* pNMHDR, LRESULT* pResult)
{
	// This feature requires Internet Explorer 6 or greater.
	// The symbol _WIN32_IE must be >= 0x0600.
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	
	ChangeNewsTextColor();

	*pResult = 0;

}

void CClock2026Dlg::ChangeReBackColor()
{
	COLORREF clrREBack = m_btnREBackColor.GetColor();

	m_RENewsData.SetBackgroundColor(FALSE, clrREBack);
}

void CClock2026Dlg::OnBnClickedButtonReBackColor()
{
	ChangeReBackColor();
}

void CClock2026Dlg::OnBnHotItemChangeButtonReBackColor(NMHDR* pNMHDR, LRESULT* pResult)
{
	// This feature requires Internet Explorer 6 or greater.
	// The symbol _WIN32_IE must be >= 0x0600.
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	
	ChangeReBackColor();

	*pResult = 0;
}

void CClock2026Dlg::UpdateREUI(BOOL bUpdate)
{
	CHARFORMAT2 CharFormat;
	CString strFaceName;
	CString strSize;
	CString strCategory;

	CNewsData& curNewsData = m_pStatus->NewsConf()->CurNewsData();

	if (bUpdate)
	{
		// UI로부터 값을 읽어 들여서 데이터를 업데이트한다.

		CCharInfoList newsString = m_RENewsData.GetCharInfoList();

		curNewsData.SetCharInfoList(newsString);

		if (!newsString.IsEmpty())
		{
			AddNewsData(curNewsData);
			ClearRichEdit();
		}
	}
	else
	{
		// 데이터 부터 값을 읽어 들여서 UI를 업데이트한다.

		if (curNewsData.IsValidKey())
		{
			m_RENewsData.SetCharInfoList(curNewsData.GetCharInfoList());
			strCategory = curNewsData.GetValue(NEWS_FIELD_INDEX::CATEGORY);
			AddNewsCategory(strCategory);
		}
		else
		{
			ClearRichEdit();
		}
	}

	m_RENewsData.GetSelectionCharFormat(CharFormat);

	CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE;
	CharFormat.dwMask &= ~CFM_PROTECTED;
	CharFormat.dwEffects &= ~CFE_AUTOCOLOR;
	CharFormat.dwEffects &= ~CFE_AUTOBACKCOLOR;
	CharFormat.dwEffects &= ~CFE_PROTECTED;

	m_btnNewsTextColor.SetColor(CharFormat.crTextColor);
	m_ComboNewsTextFont.GetDlgItem(1001)->SetWindowText(CharFormat.szFaceName);
	m_pComboNewsTextSize->GetDlgItem(1001)->SetWindowText(GETSTR(CharFormat.yHeight / DEFAULT_FONT_SIZE));

	m_pStatus->NewsConf()->DefaultFontName(CharFormat.szFaceName);
	m_pStatus->NewsConf()->TextSize(GETSTR(CharFormat.yHeight / DEFAULT_FONT_SIZE));
	m_pStatus->NewsConf()->TextColor(RGBToULONG(CharFormat.crTextColor));

	m_RENewsData.SetSelectionCharFormat(CharFormat);
}

void CClock2026Dlg::OpenLogFolder(void)
{
	ShellExecute(nullptr, _T("explore"), nullptr, nullptr, GetLogsDir().GetBuffer(), SW_SHOW);
}

void CClock2026Dlg::OnBnClickedButtonOpenLogFolder()
{
	OpenLogFolder();
}


void CClock2026Dlg::OpenLogFile(void)
{
	CTime time = CTime::GetCurrentTime();
	CString strFileName = GetLogsDir() + _T("LOG_") + time.Format(_T("%Y_%m_%d")) + _T(".Log");

	if (CFileUtils::ExistFile(strFileName))
	{
		ShellExecute(nullptr, _T("open"), strFileName, nullptr, nullptr, SW_SHOW);
	}
	else
	{
		PrintLog(strFileName + _T(" 로그 파일을 찾을 수가 없습니다!!!"), _T("오류"),TRUE);
	}
}

void CClock2026Dlg::OnBnClickedButtonOpenLogFile()
{
	OpenLogFile();
}

void CClock2026Dlg::OnBnClickedButtonClearLogMessage()
{
	m_ListBoxLog.ResetContent();
}

void CClock2026Dlg::ApplyRichEditFontAndColorFromFpg(CString strFileName)
{

	if (CFileUtils::ExistFile(strFileName))
	{
		auto pScene = std::make_unique<CCGXScene>();
		CCGXObject* pTextObject = nullptr;
		CCGXObject* pBand = nullptr;

		pScene->Open(strFileName, 1);

		pTextObject = pScene->GetCGXObject(newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]);
		pBand = pScene->GetCGXObject(newsObjects[FROM_E(NEWS_OBJECTS::BAND)]);

		if (pTextObject != nullptr)
		{
			m_pStatus->NewsConf()->DefaultFontName(pTextObject->GetCharFaceName());
			m_pStatus->NewsConf()->TextColor(pTextObject->GetColor());

			int nX = pTextObject->GetHeight();
			int nY = pTextObject->GetHeight();
			m_pHDdaVinci->SetSystemObjectRect(CG_CMD_OBJECT_IMAGE, CRect(0, 0, nX, nY));

			int nIndex = m_ComboNewsTextFont.FindString(m_pStatus->NewsConf()->DefaultFontName());
			if (nIndex != CB_ERR)
			{
				SetHangulMode();
				m_ComboNewsTextFont.SetCurSel(nIndex);
				m_RENewsData.SetDefaultFont(pTextObject->GetCharFaceName());
			}
			else
			{
				PrintLog(m_pStatus->NewsConf()->DefaultFontName() + _T(" Font가 없습니다."));
			}

			m_RENewsData.SetDefaultFontAndColor(pTextObject->GetCharFaceName(), pTextObject->GetColor());

		}


		//INT nSize = GETINT(m_pStatus->NewsConf()->TextSize()) * DEFAULT_FONT_SIZE;

		//	m_RENewsData.SetSelectionSize(GETINT(m_pStatus->NewsConf()->TextSize()) * DEFAULT_FONT_SIZE);
		m_RENewsData.SetDefaultSize(GETINT(m_pStatus->NewsConf()->TextSize()) * DEFAULT_FONT_SIZE);

		if (pBand != nullptr)
		{
			ULONG clrBand = pBand->GetColor();
			m_btnREBackColor.SetColor(ULONGToRGB(clrBand));
			m_RENewsData.SetBackgroundColor(FALSE, ULONGToRGB(clrBand));
			m_RENewsData.SetTextBackgroundColor(clrBand);
		}

		CancelPrepareNewsScroll();
		UpdateREUI(FALSE);
		UpdateData(FALSE);
	}
}

void CClock2026Dlg::LoadRichEditFontAndColor(void)
{
	if (m_pStatus->NewsConf()->ApplyScrollColor())
	{
		ApplyRichEditFontAndColorFromFpg(GetScrollDir() + m_pStatus->NewsConf()->ScrollTemplate());
	}
	else
	{
		ApplyRichEditFontAndColorFromFpg(GetMirDir() + _T("News.fpg"));
	}
}

void CClock2026Dlg::OnBnClickedCheckApplyScrollColor()
{
	m_pStatus->NewsConf()->ApplyScrollColor(IsDlgButtonChecked(IDC_CHECK_APPLY_SCROLL_COLOR));

	LoadRichEditFontAndColor();

	UpdateData(FALSE);
}

int CClock2026Dlg::GetHanEngMode(void)
{
	int nReturnCode = HANGUL_MODE;

	HIMC hIMC = ::ImmGetContext(((CWnd*)this)->m_hWnd);
	DWORD dwConversion, dwSentence;

	if (hIMC && ::ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence))
	{
		if ((dwConversion & IME_CMODE_NATIVE) != IME_CMODE_NATIVE)		// 영문 상태 
		{
			nReturnCode = ENGLISH_MODE;
		}
	}
	return nReturnCode;
}

void CClock2026Dlg::SetHangulMode(void)
{
	// IME 한글로 셋팅
	HIMC hIMC = ::ImmGetContext(((CWnd*)this)->m_hWnd);
	DWORD dwConversion, dwSentence;

	if (hIMC && ::ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence))
	{
		if ((dwConversion & IME_CMODE_NATIVE) != IME_CMODE_NATIVE)		// 영문 상태 
		{
			dwConversion = (dwConversion & ~IME_CMODE_LANGUAGE) | IME_CMODE_NATIVE;
			::ImmSetConversionStatus(hIMC, dwConversion, dwSentence);     // ==> 한글
		}
		::ImmReleaseContext(((CWnd*)this)->m_hWnd, hIMC);
	}
}

void CClock2026Dlg::SetEnglishMode(void)
{

	// IME 영문으로 셋팅
	HIMC hIMC = ::ImmGetContext(((CWnd*)this)->m_hWnd);
	DWORD dwConversion, dwSentence;

	if (hIMC && ::ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence))
	{
		if ((dwConversion & IME_CMODE_NATIVE) == IME_CMODE_NATIVE)		// 한글 상태
		{
			dwConversion = (dwConversion & ~IME_CMODE_LANGUAGE) | IME_CMODE_ALPHANUMERIC;
			::ImmSetConversionStatus(hIMC, dwConversion, dwSentence); // ==> 영문
		}
		::ImmReleaseContext(((CWnd*)this)->m_hWnd, hIMC);	// LKM 2001.04.25
	}
}

void CClock2026Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_pHDdaVinci->ClearAll();

	m_pWndProgress->DestroyWindow();

	SAFE_DELETE(m_pWndProgress);

	theApp.SaveSetting();
}

void CClock2026Dlg::CancelPrepareNewsScroll(void)
{
	m_bPrepareNewsScroll = FALSE;
	m_btnNewsScrollPrepare.SetColor(::GetSysColor(COLOR_BTNFACE));
}

void CClock2026Dlg::AddNewsData(CNewsData& newsData)
{
	CNewsDataList* pList = m_pStatus->GetNewsList();

	if (pList == nullptr) return;

	int nIndex = pList->GetIndexByKey(newsData.GetKey());
	int nSize = pList->Size();
	CString strCategory = m_pStatus->NewsConf()->Category();

	newsData.SetValue(NEWS_FIELD_INDEX::USE, TRUE_STRING);  // 고친것은 사용할 의도가 있다고 봐야됨
	newsData.SetValue(NEWS_FIELD_INDEX::CATEGORY, strCategory);
	AddNewsCategory(strCategory);

	CancelPrepareNewsScroll();

	if (nIndex >= 0)
	{
		pList->UpdateData(newsData);
	}
	else
	{
		if (nSize < MAX_NEWS_NUM)
		{
			pList->AddData(newsData);
			nIndex = pList->Size() - 1;
		}
		else
		{
			PrintLog(_T("뉴스 아이템이 찼습니다!"), _T("경고"), TRUE);
			return;
		}
	}

	m_SpreadNews.UpdateRow(1, nIndex + 1, FALSE);
	m_SpreadNews.DrawSelection(nIndex + 1, -1);

}

void CClock2026Dlg::ClearRichEdit()
{
	m_RENewsData.ClearImageVector();

	CNewsData& curNewsData = m_pStatus->NewsConf()->CurNewsData();
	curNewsData.Clear();
	curNewsData.MakeKeyString();
	m_RENewsData.SetCharInfoList(curNewsData.GetCharInfoList());
}

void CClock2026Dlg::SendNewsDataFromRichEdit()
{
	CNewsData curNewsData = m_pStatus->NewsConf()->CurNewsData();

	CCharInfoList newsString = m_RENewsData.GetCharInfoList();

	curNewsData.SetCharInfoList(newsString);

	CString strData = curNewsData.GetContentString();

	if (strData.Trim() != BLANK_STRING)
	{
		AddNewsData(curNewsData);
	}

	ClearRichEdit();
}

LRESULT CClock2026Dlg::OnSpreadEditModeOff(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)wParam;

	switch (nID)
	{
	case IDC_SPREAD_NEWS:
		m_SpreadNews.EditModeOff(wParam, lParam);
		CancelPrepareNewsScroll();
		break;

	case IDC_SPREAD_WEATHER:
		m_SpreadWeather.EditModeOff(wParam, lParam);
		break;

	case IDC_SPREAD_ALIAS :
		m_SpreadAlias.EditModeOff(wParam, lParam);
		break;

	case IDC_SPREAD_NOTICE:
		m_SpreadNotice.EditModeOff(wParam, lParam);
		break;

	default:
		break;
	}
	return 1;
}

void CClock2026Dlg::TrackMenuOpen(UINT nID, SS_RBUTTON* ss_rbutton)
{
	SS_CELLCOORD RButtonCoord;

	CMenu Menu;
	
	RButtonCoord.Col = ss_rbutton->Col;
	RButtonCoord.Row = ss_rbutton->Row;

	switch (nID)
	{
	case IDC_SPREAD_NEWS:
		Menu.LoadMenu(IDR_MENU_SPREAD_NEWS);
		reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->SetRButtonCoord(RButtonCoord);
		break;

	case IDC_SPREAD_NOTICE:
		Menu.LoadMenu(IDR_MENU_SPREAD_NOTICE);
		reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->SetRButtonCoord(RButtonCoord);
		break;

	default:
		break;
	}

	CMenu* pSubMenu = Menu.GetSubMenu(0);

	CPoint ptMenu(ss_rbutton->xPos, ss_rbutton->yPos);

	GetDlgItem(nID)->ClientToScreen(&ptMenu);

	pSubMenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, ptMenu.x, ptMenu.y, this);
}


LRESULT CClock2026Dlg::OnSpreadRButtonClicked(WPARAM wParam, LPARAM lParam)
{

	UINT nID = (UINT)(wParam);
	SS_RBUTTON* ss_rbutton = (SS_RBUTTON*)(lParam);

	m_pTrackMenuOwner = GetDlgItem(nID);

	if (m_pTrackMenuOwner == nullptr) return 0;

	if (ss_rbutton->RButtonType == RBUTTONTYPE_DOWN)
	{
		switch (nID)
		{
		case IDC_SPREAD_NEWS:
			TrackMenuOpen(nID, ss_rbutton);
			break;

		case IDC_SPREAD_NOTICE:
			TrackMenuOpen(nID, ss_rbutton);
			break;

		default:
			break;
		}
	}
	return 1;
}

LRESULT CClock2026Dlg::SpreadLButtonClicked(UINT nID,INT nCol,INT nRow)
{
	SS_CELLCOORD ss_coord;

	ss_coord.Col = nCol;
	ss_coord.Row = nRow;

  return SendMessage(SSM_CLICK, nID, reinterpret_cast<LPARAM>(&ss_coord));
}

LRESULT CClock2026Dlg::OnSpreadDBClicked(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);


	switch (nID)
	{
	case IDC_SPREAD_WEATHER:
	//	SendMessage(SSM_DBLCLK, IDC_SPREAD_WEATHER, reinterpret_cast<LPARAM>(&lParam));
		m_SpreadWeather.SetEditMode(TRUE);
		break;

	case IDC_SPREAD_ALIAS:
	//	SendMessage(SSM_DBLCLK, IDC_SPREAD_ALIAS, reinterpret_cast<LPARAM>(&lParam));
		m_SpreadAlias.SetEditMode(TRUE);
		break;

	case IDC_SPREAD_NOTICE:
		m_SpreadNotice.DBClicked(m_CurNoticeData, wParam, lParam);
		SelectNotice(m_CurNoticeData);
		break;

	default:
		break;
	}
	UpdateData(FALSE);
	return 1;
}


LRESULT CClock2026Dlg::OnSpreadLButtonClicked(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
	SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);

	CNewsData& curNewsData = m_pStatus->NewsConf()->CurNewsData();
	CWeatherData weatherData;

	switch (nID)
	{
	case IDC_SPREAD_NEWS:
		m_SpreadNews.LButtonClicked(curNewsData, wParam, lParam);
		if (ss_coord->Col == NEWS_FIELD_INDEX::USE + 1)
		{
			CancelPrepareNewsScroll();
		}
		UpdateREUI(FALSE);
		break;

	case IDC_SPREAD_WEATHER:
		m_SpreadWeather.LButtonClicked(weatherData, wParam, lParam);
		break;

	case IDC_SPREAD_ALIAS:
		m_SpreadAlias.LButtonClicked(wParam, lParam);
		break;

	case IDC_SPREAD_NOTICE:
		m_SpreadNotice.LButtonClicked(m_CurNoticeData, wParam, lParam);
		SelectNotice(m_CurNoticeData);
		break;

	default:
		break;
	}
	UpdateData(FALSE);
	return 1;
}

LRESULT CClock2026Dlg::OnSpreadComboSelChange(WPARAM wParam, LPARAM lParam)
{
	UINT nID = (UINT)(wParam);
    SS_CELLCOORD* ss_coord = (SS_CELLCOORD*)(lParam);

	UINT nReturn = 0;

	switch (nID)
	{
	case IDC_SPREAD_NEWS:
		CancelPrepareNewsScroll();
		break;


//	case IDC_SPREAD_NOTICE:
//		nReturn = m_SpreadNotice.ComboSelChange(wParam, lParam);
//		m_CurNoticeData = m_pStatus->GetNoticeList()->GetAt(ss_coord->Row - 1);
//		if (m_CurNoticeData.IsValid())
//		{
//			ChangeNotice(m_CurNoticeData);
//		}
////		m_SpreadNotice.UpdateRow(ss_coord->Row,FALSE);
//		break;

	default:
		break;
	}
	UpdateData(FALSE);
	return 1;
}


void CClock2026Dlg::OnSpreadSortAsc()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnSortAsc();
			break;
		default:
			break;
		}

	}
}

void CClock2026Dlg::OnSpreadSortDesc()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnSortDesc();
			break;


		default:
			break;
		}

	}
}

void CClock2026Dlg::OnSpreadInsertRow()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnInsertRow();
			break;

		case IDC_SPREAD_NOTICE:
			reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->OnInsertRow();
			break;


		default:
			break;
		}

	}

}

void CClock2026Dlg::OnSpreadDeleteRow()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnDeleteRow();
			break;

		case IDC_SPREAD_NOTICE:
			reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->OnDeleteRow();
			break;


		default:
			break;
		}

	}
}


void CClock2026Dlg::OnSpreadAllUse()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnAllUse();
			break;

		case IDC_SPREAD_NOTICE:
			reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->OnAllUse();
			break;


		default:
			break;
		}

	}
}

void CClock2026Dlg::OnSpreadAllDeselectUse()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnAllDeselectUse();
			break;

		case IDC_SPREAD_NOTICE:
			reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->OnAllDeselectUse();
			break;


		default:
			break;
		}

	}
}

void CClock2026Dlg::OnSpreadInvertUse()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnInvertUse();
			break;

		case IDC_SPREAD_NOTICE:
			reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->OnInvertUse();
			break;


		default:
			break;
		}

	}
}

void CClock2026Dlg::OnSpreadClearSheet()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnClearSheet();
			break;


		case IDC_SPREAD_NOTICE:
			reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->OnClearSheet();
			break;

		default:
			break;
		}
	}
}

void CClock2026Dlg::OnSpreadRestoreSheet()
{
	if (m_pTrackMenuOwner != nullptr)
	{
		switch (m_pTrackMenuOwner->GetDlgCtrlID())
		{
		case IDC_SPREAD_NEWS:
			reinterpret_cast<CNewsSpread*>(m_pTrackMenuOwner)->OnRestoreSheet();
			break;

		case IDC_SPREAD_NOTICE:
			reinterpret_cast<CNoticeSpread*>(m_pTrackMenuOwner)->OnRestoreSheet();
			break;

		default:
			break;
		}

	}
}


void CClock2026Dlg::OnSelchangeRicheditNewsData(NMHDR* pNMHDR, LRESULT* pResult)
{
	SELCHANGE* pSelChange = reinterpret_cast<SELCHANGE*>(pNMHDR);
	// TODO:  The control will not send this notification unless you override the
	// CDialogEx::OnInitDialog() function to send the EM_SETEVENTMASK message
	// to the control with the ENM_SELCHANGE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	CHARFORMAT2 CharFormat;

	m_RENewsData.GetSelectionCharFormat(CharFormat);

	CharFormat.dwMask |= CFM_FACE | CFM_COLOR | CFM_SIZE;
	CharFormat.dwMask &= ~CFM_PROTECTED;
	CharFormat.dwEffects &= ~CFE_AUTOCOLOR;
	CharFormat.dwEffects &= ~CFE_AUTOBACKCOLOR;
	CharFormat.dwEffects &= ~CFE_PROTECTED;

	m_btnNewsTextColor.SetColor(CharFormat.crTextColor);
	m_ComboNewsTextFont.GetDlgItem(1001)->SetWindowText(CharFormat.szFaceName);
	m_pComboNewsTextSize->GetDlgItem(1001)->SetWindowText(GETSTR(CharFormat.yHeight / DEFAULT_FONT_SIZE));

	m_pStatus->NewsConf()->DefaultFontName(CharFormat.szFaceName);
	m_pStatus->NewsConf()->TextSize(GETSTR(CharFormat.yHeight / DEFAULT_FONT_SIZE));
	m_pStatus->NewsConf()->TextColor(RGBToULONG(CharFormat.crTextColor));

	*pResult = 0;
}

BOOL CClock2026Dlg::ImportImage()
{
	TCHAR szCurrentDirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szCurrentDirectory);

	CImageImportDlg dlg(TRUE, FALSE, PNG_EXT, BLANK_STRING, OFN_OVERWRITEPROMPT);
	dlg.m_ofn.lpstrFilter = IMAGE_FILTER_OLD_TYPE;

	CPXFilePath pathImage(BLANK_STRING);
	CString strFolder = pathImage.GetFolder();
	dlg.m_ofn.lpstrInitialDir = strFolder;
	_tcscpy(dlg.m_ofn.lpstrFile, pathImage.GetFileName());

	if (dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();
		CString strTextSize;
		m_pComboNewsTextSize->GetWindowText(strTextSize);
		INT nTextSize = GETINT(strTextSize);
		m_RENewsData.InsertImgToRichEditCtrl(strFileName, nTextSize);

		long nStart = -1;
		long nEnd = -1;

		m_RENewsData.GetSel(nStart, nEnd);
		m_RENewsData.SetSel(nStart - 1, nStart);
		m_RENewsData.SetSelectionSize(nTextSize);   // 이미지 크기를 저장시킨다.
		m_RENewsData.GetCharInfoList();
		m_RENewsData.SetSel(nStart, nStart);
		m_RENewsData.SetFocus();

		CancelPrepareNewsScroll();

		SetChangeMark(TRUE);

		PrintLog(strFileName + _T(" 이미지 파일을 읽어 들였습니다."));
	}
	::SetCurrentDirectory(szCurrentDirectory);

	return TRUE;
}

void CClock2026Dlg::OnBnClickedButtonInsertImage()
{
	ImportImage();
}

void CClock2026Dlg::OnBnClickedButtonImageShortcutConfig()
{
	CImageConfigDlg dlg;

	dlg.DoModal();

	UpdateImageButtons();
}

void CClock2026Dlg::InsertImageShortCut(INT nImageNumber)
{
	if (CFileUtils::ExistFile(m_pStatus->NewsConf()->ImageShortCut(nImageNumber)))
	{

		CString strTextSize;
		m_pComboNewsTextSize->GetWindowText(strTextSize);
		INT nTextSize = GETINT(strTextSize);
		m_RENewsData.InsertImgToRichEditCtrl(m_pStatus->NewsConf()->ImageShortCut(nImageNumber), nTextSize);

		long nStart = -1;
		long nEnd = -1;

		m_RENewsData.GetSel(nStart, nEnd);
		m_RENewsData.SetSel(nStart - 1, nStart);
		m_RENewsData.SetSelectionSize(nTextSize);   // 이미지 크기를 저장시킨다.
		m_RENewsData.GetCharInfoList();
		m_RENewsData.SetSel(nStart, nStart);
		m_RENewsData.SetFocus();
		CancelPrepareNewsScroll();

		SetChangeMark(TRUE);

		PrintLog(m_pStatus->NewsConf()->ImageShortCut(nImageNumber) + _T(" 이미지 파일을 읽어 들였습니다."));

	}
	else
	{
		PrintLog(m_pStatus->NewsConf()->ImageShortCut(nImageNumber) + _T(" 이미지 파일이 없습니다!!"));
	}

}

void CClock2026Dlg::OnBnClickedButtonImageShortcut1()
{
	InsertImageShortCut(IMAGE_SHORTCUT_1);
}

void CClock2026Dlg::OnBnClickedButtonImageShortcut2()
{
	InsertImageShortCut(IMAGE_SHORTCUT_2);
}

void CClock2026Dlg::OnBnClickedButtonImageShortcut3()
{
	InsertImageShortCut(IMAGE_SHORTCUT_3);
}

void CClock2026Dlg::OnBnClickedButtonImageShortcut4()
{
	InsertImageShortCut(IMAGE_SHORTCUT_4);
}

void CClock2026Dlg::OnBnClickedButtonImageShortcut5()
{
	InsertImageShortCut(IMAGE_SHORTCUT_5);
}

void CClock2026Dlg::OnBnClickedButtonImageShortcut6()
{
	InsertImageShortCut(IMAGE_SHORTCUT_6);
}

void CClock2026Dlg::OnBnClickedButtonNewsClear()
{
	if (IDYES == MessageBox(_T("뉴스 데이터를 모두 지우시겠습니까?"), _T("경고"), MB_YESNO | MB_ICONEXCLAMATION))
	{
		m_SpreadNews.OnClearSheet();
		PrintLog(_T("모든 뉴스를 지웠습니다."));

		this->SendMessage(WM_NEWS_CHANGE, 0, 0);
		SetChangeMark(TRUE);
	}
}

void CClock2026Dlg::SpecialChar()
{
	TCHAR tszWindowsDirectory[MAX_PATH];
	::GetWindowsDirectory(tszWindowsDirectory, MAX_PATH);
	TCHAR tszFileName[MAX_PATH];

	::_stprintf(tszFileName, _T("%s/system32/CharMap.exe"), tszWindowsDirectory);

	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;
	::CreateProcess(tszFileName, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
}

void CClock2026Dlg::OnBnClickedButtonNewsSpecialChar()
{
	SpecialChar();
}

void CClock2026Dlg::OnBnClickedButtonSpellCheck()
{
	CSpellCheck dlg(this);
	dlg.m_strContents = m_RENewsData.GetUnicodeString();
	dlg.DoModal();
}

void CClock2026Dlg::ReloadNewsScrollTemplate(void)
{
	m_pComboNewsScrollTemplate->ResetContent();

	CFileFind FileFinder;

	// start working for files
	BOOL bWorking = FileFinder.FindFile(GetScrollDir() + _T("*.fpg"));

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();

		if (!FileFinder.IsDirectory())
		{
			CString str = CFileUtils::ExtractFileName(FileFinder.GetFilePath());

			m_pComboNewsScrollTemplate->AddString(str);
		}

	}
	FileFinder.Close();

	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonReloadNewsScrollTemplate()
{
	ReloadNewsScrollTemplate();
}

void CClock2026Dlg::SelectChangeNewsScrollTemplate(void)
{
	CString strFpgFileName;
	
	int n = m_pComboNewsScrollTemplate->GetCurSel();
	m_pComboNewsScrollTemplate->GetLBText(n, strFpgFileName);

	CancelPrepareNewsScroll();

	DrawPreview(m_PreviewNewsScroll, GetScrollDir() + strFpgFileName);

	m_pStatus->NewsConf()->ScrollTemplate(strFpgFileName);

	LoadRichEditFontAndColor();

	UpdateData(FALSE);
}

void CClock2026Dlg::OnCbnSelchangeComboNewsScrollTemplate()
{
	SelectChangeNewsScrollTemplate();
}

void CClock2026Dlg::OnKillfocusEditNewsScrollIteration()
{
	CString strBuffer;
	GetDlgItemText(IDC_EDIT_NEWS_SCROLL_ITERATION, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->NewsConf()->ScrollIteration(min(max(GETINT(strBuffer), MIN_NEWS_SCROLL_ITERATION), MAX_NEWS_SCROLL_ITERATION));
		SetChangeMark(TRUE);
	}

	UpdateData(FALSE);
}


void CClock2026Dlg::OnKillfocusEditNewsScrollItemGap()
{
	CancelPrepareNewsScroll();

	CString strBuffer;
	GetDlgItemText(IDC_EDIT_NEWS_SCROLL_ITEM_GAP, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->NewsConf()->ScrollItemGap(min(max(GETINT(strBuffer), MIN_NEWS_SCROLL_ITEM_GAP), MAX_NEWS_SCROLL_ITEM_GAP));
		SetChangeMark(TRUE);
	}

	
	UpdateData(FALSE);
}


void CClock2026Dlg::OnKillfocusEditNewsScrollPrefixGap()
{
	CancelPrepareNewsScroll();

	CString strBuffer;
	GetDlgItemText(IDC_EDIT_NEWS_SCROLL_PREFIX_GAP, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->NewsConf()->ScrollPrefixGap(min(max(GETINT(strBuffer), MIN_NEWS_SCROLL_PREFIX_GAP), MAX_NEWS_SCROLL_PREFIX_GAP));
		SetChangeMark(TRUE);
	}

	UpdateData(FALSE);
}

void CClock2026Dlg::OnKillfocusEditNewsScrollSpeed()
{
	CancelPrepareNewsScroll();

	CString strBuffer;
	GetDlgItemText(IDC_EDIT_NEWS_SCROLL_SPEED, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->NewsConf()->ScrollSpeed(min(max(GETINT(strBuffer), MIN_NEWS_SCROLL_SPEED), MAX_NEWS_SCROLL_SPEED));
		SetChangeMark(TRUE);
	}

	UpdateData(FALSE);
}

void CClock2026Dlg::ReplaceNewsData(CNewsData& NewsData, CString strLayerAlias, CString strAlias, INT nPage)
{
	CCGXLayer* pLayer = m_pHDdaVinci->GetCGXLayer(strLayerAlias, nPage);
	CCGXObject* pCloneContents = nullptr;
	CCGXObject* pCloneImage = nullptr;

	if (pLayer == nullptr)
	{
		PrintLog(strLayerAlias + _T("존재하지 않습니다."), _T("오류"), TRUE);
		return;
	}

	INT nPrevXPos = m_NewsContentsCoord.nOrigX;

	INT nIndex = NewsData.GetRowNum();

	if (NewsData.IsOnlyText())
	{
		m_pHDdaVinci->AddObject(pLayer, m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)], CG_CMD_OBJECT_TEXT, strAlias + _T("_"), nPage);

		pCloneContents = m_pHDdaVinci->GetCGXObject(strAlias + _T("_"), nPage);

		pCloneContents->SetPositionByAbsEx(nPrevXPos, m_NewsContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);

		if (NewsData.HasErrorTextFormat())
		{
			NewsData.FixedTextFormatFromCharInfoList();
			NewsData.MakeCharInfoListFromTextFormat();

			NewsData.RefreshData();

			AddNewsData(NewsData);
		}
		else
		{
			NewsData.ExtractTextFormat();
		}

		CString strContents = NewsData.GetContentString();

		strContents = strContents.Trim(_T(" \n\r"));  // 메모장에서 엔터키와 함께 복사된 경우 죽는 경우가 있어 이를 방지하기 위함

		if (strContents.GetLength() != 0)
		{
			m_pHDdaVinci->ObjectSetTextFontColorEx(strAlias + _T("_"),
				strContents,
				NewsData.Fonts(),
				NewsData.Colors(),
				NewsData.Sizes(),
				_T("KBSSymbol1"),
				100,
				100,
				nPage,
				1);
		}
		else
		{
			m_pHDdaVinci->GetCGXObject(strAlias + _T("_"), nPage)->SetShow(false);
		}
	}
	else 
	{
		std::vector<CNewsData> vNewsData;
		NewsData.ParseItem(vNewsData);

		INT nSize = vNewsData.size();

		for (int j = 0; j < nSize; j++)
		{
			CNewsData& newsDataBuffer = vNewsData.at(j);

			if (!newsDataBuffer.IsImagePart())  // 그림파일이 아닌 문자면
			{

				m_pHDdaVinci->AddObject(pLayer, m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)], CG_CMD_OBJECT_TEXT, strAlias + _T("_") + GETSTR(j), nPage);

				pCloneContents = m_pHDdaVinci->GetCGXObject(strAlias + _T("_") + GETSTR(j), nPage);

				pCloneContents->SetPositionByAbsEx(nPrevXPos, m_NewsContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);

				newsDataBuffer.ExtractTextFormat();

				CString strContents = newsDataBuffer.GetContentString();

				strContents = strContents.Trim(_T("\n\r"));  // 메모장에서 엔터키와 함께 복사된 경우 죽는 경우가 있어 이를 방지하기 위함

				CString strTrim = strContents;

				strTrim.TrimRight();

				INT nSpaceCount = (strContents.GetLength() - strTrim.GetLength());

				if (strTrim.GetLength() != 0)
				{
					m_pHDdaVinci->ObjectSetTextFontColorEx(strAlias + _T("_") + GETSTR(j),
						strContents,
						newsDataBuffer.Fonts(),
						newsDataBuffer.Colors(),
						newsDataBuffer.Sizes(),
						_T("KBSSymbol1"),
						100,
						100,
						nPage,
						1);

					pCloneContents->GetPositionEx(&nPrevXPos, &m_NewsContentsCoord.nY, CGX_REFERENCE_POINT_RIGHTCENTER);
				}
				else
				{
					m_pHDdaVinci->GetCGXObject(strAlias + _T("_") + GETSTR(j), nPage)->SetShow(false);
				}

				nPrevXPos = nPrevXPos + m_nNewsSpaceWidth * nSpaceCount; // 좌우측에 있는 스페이스 문자를 고려해줘야한다.

			}
			else                                                                       // 그림에 대한 처리
			{
				if (pLayer != nullptr)          // 레이어와 레퍼런스 이미지가 있어야 이미지 추가가 가능하다.
				{
					m_pHDdaVinci->AddObject(pLayer, m_pNewsImageSystemObject, CG_CMD_OBJECT_IMAGE, IMAGE_FILE_TAG + GETSTR(nIndex) + CString(_T("_")) + GETSTR(j), nPage); // Alias가 중복되면 송출이상이 생김

					pCloneImage = m_pHDdaVinci->GetCGXObject(IMAGE_FILE_TAG + GETSTR(nIndex) + CString(_T("_")) + GETSTR(j), nPage);

					CString strImageFileName = newsDataBuffer.ImageFileName();

					if (CFileUtils::ExistFile(strImageFileName))   // 이미지 파일이 존재하여야 한다.
					{
						CRect rt = m_RENewsData.GetImageRect(strImageFileName);

						pCloneImage->Replace(strImageFileName, 1);

						INT nImageHeight = static_cast<INT>(rt.Height() * (static_cast<float>(newsDataBuffer.Sizes().at(0)) / 100.0f));
						INT nImageWidth = 0;

						INT nImageHeightArray[1] = { nImageHeight };

						nImageWidth = static_cast<INT>(rt.Width() * (static_cast<FLOAT>(nImageHeight) / static_cast<FLOAT>(rt.Height())));  // 밴드높이와 같을 때의 이미지 폭을 구함

						INT nImageWidthArray[1] = { nImageWidth };

						m_pHDdaVinci->SetProperty(pCloneImage, CGX_PROPERTY_WIDTH, nImageWidthArray, 1);
						m_pHDdaVinci->SetProperty(pCloneImage, CGX_PROPERTY_HEIGHT, nImageHeightArray, 1);

						pCloneImage->SetPositionByAbsEx(nPrevXPos, m_NewsContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);  // Y좌표는 맨처음 Prefix의 좌표를 사용한다.

						pCloneImage->GetPositionEx(&nPrevXPos, &m_NewsContentsCoord.nY, CGX_REFERENCE_POINT_RIGHTCENTER);
					}
					else
					{
						pCloneImage->SetShow(false);
						PrintLog(strImageFileName + _T(" 파일이 없습니다.!!!"));
					}
				}
				else
				{
					PrintLog(_T("레이어가 없어 이미지 추가가 불가능합니다. "));
				}
			}
		}
	}
}

BOOL CClock2026Dlg::DisplayNews(BOOL bFirst)
{
	//시간 줄이는 방법에 대해 연구

	auto ShowLayer = [&](INT nIndex, BOOL bShow)
	{
		if (m_pNewsLayers[nIndex] != nullptr)
		{
			m_pNewsLayers[nIndex]->SetShow(bShow);
		}
		else
		{
			PrintLog(newsLayers[nIndex] + _T("가 존재하지 않습니다. FPG를 점검하세요!!!!"));
		}
	};

	auto ShowObject = [&](INT nIndex, BOOL bShow)
	{
		if (m_pNewsObjects[nIndex] != nullptr)
		{
			m_pNewsObjects[nIndex]->SetShow(bShow);
		}
		else
		{
			PrintLog(newsObjects[nIndex] + _T("가 존재하지 않습니다. FPG를 점검하세요!!!!"));
		}
	};

	auto ReplaceObject = [&](INT nIndex, CString strData)
	{
		if (m_pNewsObjects[nIndex] != nullptr)
		{
			m_pNewsObjects[nIndex]->SetShow(true);
			m_pNewsObjects[nIndex]->Replace(strData, FALSE);
		}
		else
		{
			PrintLog(newsObjects[nIndex] + _T("가 존재하지 않습니다. FPG를 점검하세요!!!!"));
		}
	};

	if (bFirst)
	{
		if (!OpenFpg(_T("News"), NEWS_PAGE_NUMBER, GetMirDir()))
		{
			return FALSE;
		}

		// NEWS_LAYERS 처리
		std::vector<NEWS_LAYERS> layerKeys = {
			NEWS_LAYERS::CONTENTS,
			NEWS_LAYERS::CONTENTS1,
			NEWS_LAYERS::FIXED_CATEGORY,
			NEWS_LAYERS::MOVING_CATEGORY,
			NEWS_LAYERS::CATEGORY_EFFECT
		};

		for (const auto& key : layerKeys)
		{
			int idx = FROM_E(key);
			m_pNewsLayers[idx] = m_pHDdaVinci->GetCGXLayer(newsLayers[idx], NEWS_PAGE_NUMBER);
			if (m_pNewsLayers[idx] == nullptr)
			{
				PrintLog(newsLayers[idx] + _T("가 존재하지 않습니다."), _T("오류"), TRUE);
				return FALSE;
			}
		}

		// NEWS_OBJECTS 처리
		std::vector<NEWS_OBJECTS> objectKeys = {
			NEWS_OBJECTS::CONTENTS,
			NEWS_OBJECTS::CONTENTS1,
			NEWS_OBJECTS::FIXED_CATEGORY,
			NEWS_OBJECTS::FIXED_CATEGORY_BAND,
			NEWS_OBJECTS::MOVING_CATEGORY,
			NEWS_OBJECTS::MOVING_CATEGORY_BAND,
			NEWS_OBJECTS::CATEGORY_EFFECT,
			NEWS_OBJECTS::BAND
		};

		for (const auto& key : objectKeys)
		{
			int idx = FROM_E(key);
			m_pNewsObjects[idx] = m_pHDdaVinci->GetCGXObject(newsObjects[idx], NEWS_PAGE_NUMBER);
			if (m_pNewsObjects[idx] == nullptr)
			{
				PrintLog(newsObjects[idx] + _T("가 존재하지 않습니다."), _T("오류"), TRUE);
				return FALSE;
			}
		}

		m_pHDdaVinci->AddObject(m_pNewsLayers[FROM_E(NEWS_LAYERS::CONTENTS)],
			m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)],
			CG_CMD_OBJECT_TEXT,
			_T("Clone_") + newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)],
			NEWS_PAGE_NUMBER);

		m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS)] = m_pHDdaVinci->GetCGXObject(_T("Clone_") + newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)], NEWS_PAGE_NUMBER);

		m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]->SetShow(FALSE);

		m_pHDdaVinci->AddObject(m_pNewsLayers[FROM_E(NEWS_LAYERS::CONTENTS1)],
			m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)],
			CG_CMD_OBJECT_TEXT,
			_T("Clone_") + newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)],
			NEWS_PAGE_NUMBER);

		m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)] = m_pHDdaVinci->GetCGXObject(_T("Clone_") + newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)], NEWS_PAGE_NUMBER);

		m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)]->SetShow(FALSE);

		m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]->GetPositionEx(&m_NewsContentsCoord.nOrigX, &m_NewsContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);
		m_nNewsPrevXPos = m_NewsContentsCoord.nOrigX;
		m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]->Replace(_T("가"), 0);
		m_nNewsSpaceWidth = m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]->GetWidth() / 2;

		m_pHDdaVinci->SetSystemObjectRect(CG_CMD_OBJECT_IMAGE, CRect(0, 0, m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]->GetHeight(), m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]->GetHeight()));

		m_pNewsImageSystemObject = new CCGXObject(m_pHDdaVinci->GetSystemObject(CG_CMD_OBJECT_IMAGE));

		m_pNewsAlias = new CStringList();

		m_pHDdaVinci->m_pxScene[NEWS_PAGE_NUMBER]->GetAlias(m_pNewsAlias);

		m_pHDdaVinci->m_pxScene[NEWS_PAGE_NUMBER]->RemoveObject(m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]);
		m_pHDdaVinci->m_pxScene[NEWS_PAGE_NUMBER]->RemoveObject(m_pNewsCloneObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)]);

		m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)]->SetShow(FALSE);
		m_pNewsObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)]->SetShow(FALSE);

	}
	else
	{
		m_pHDdaVinci->m_pxScene[NEWS_PAGE_NUMBER]->RemoveObjectNotInList(m_pNewsAlias);
	}

	INT nNewsSize = m_pStatus->GetNewsList()->Size();

	if (bFirst)
	{
		m_nCurNews = (m_pStatus->GetNewsList()->GetCurIndex() >= nNewsSize) ? 0 : m_pStatus->GetNewsList()->GetCurIndex();

		m_curNewsData = m_pStatus->GetNewsList()->GetAt(m_nCurNews);

		while (m_curNewsData.GetCharInfoList().Size() == 0 || !m_curNewsData.IsAvailable())
		{
			if (++m_nCurNews >= nNewsSize) m_nCurNews = 0;
			m_curNewsData = m_pStatus->GetNewsList()->GetAt(m_nCurNews);
		}

		ShowLayer(FROM_E(NEWS_LAYERS::FIXED_CATEGORY), _HIDE_);

		ShowLayer(FROM_E(NEWS_LAYERS::MOVING_CATEGORY), _SHOW_);

		ShowLayer(FROM_E(NEWS_LAYERS::CATEGORY_EFFECT), _SHOW_);

		ShowLayer(FROM_E(NEWS_LAYERS::CONTENTS), _HIDE_);

		ShowLayer(FROM_E(NEWS_LAYERS::CONTENTS1), _SHOW_);  

		ReplaceObject(FROM_E(NEWS_OBJECTS::MOVING_CATEGORY), m_curNewsData.GetValue(NEWS_FIELD_INDEX::CATEGORY));

		ReplaceNewsData(m_curNewsData, newsLayers[FROM_E(NEWS_LAYERS::CONTENTS1)], newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)], NEWS_PAGE_NUMBER);
	}
	else
	{
		if (++m_nCurNews >= nNewsSize) m_nCurNews = 0;

		m_curNewsData = m_pStatus->GetNewsList()->GetAt(m_nCurNews);

		while (m_curNewsData.GetCharInfoList().Size() == 0 || !m_curNewsData.IsAvailable())
		{
			if (++m_nCurNews >= nNewsSize) m_nCurNews = 0;
			m_curNewsData = m_pStatus->GetNewsList()->GetAt(m_nCurNews);
		}
	
		if (m_prevNewsData.GetValue(NEWS_FIELD_INDEX::CATEGORY).Trim() == m_curNewsData.GetValue(NEWS_FIELD_INDEX::CATEGORY).Trim())
		{
			ReplaceObject(FROM_E(NEWS_OBJECTS::FIXED_CATEGORY), m_curNewsData.GetValue(NEWS_FIELD_INDEX::CATEGORY));
			ReplaceObject(FROM_E(NEWS_OBJECTS::MOVING_CATEGORY), BLANK_STRING);

			ShowLayer(FROM_E(NEWS_LAYERS::FIXED_CATEGORY), _SHOW_);
			ShowLayer(FROM_E(NEWS_LAYERS::MOVING_CATEGORY), _HIDE_);
			ShowLayer(FROM_E(NEWS_LAYERS::CATEGORY_EFFECT), _HIDE_);

			ShowObject(FROM_E(NEWS_OBJECTS::FIXED_CATEGORY), _SHOW_);
			ShowObject(FROM_E(NEWS_OBJECTS::FIXED_CATEGORY_BAND), _SHOW_);
			ShowObject(FROM_E(NEWS_OBJECTS::MOVING_CATEGORY), _HIDE_);
			ShowObject(FROM_E(NEWS_OBJECTS::MOVING_CATEGORY_BAND), _HIDE_);
		}
		else
		{
			ReplaceObject(FROM_E(NEWS_OBJECTS::FIXED_CATEGORY), BLANK_STRING);
			ReplaceObject(FROM_E(NEWS_OBJECTS::MOVING_CATEGORY), m_curNewsData.GetValue(NEWS_FIELD_INDEX::CATEGORY));

			ShowLayer(FROM_E(NEWS_LAYERS::FIXED_CATEGORY), _HIDE_);
			ShowLayer(FROM_E(NEWS_LAYERS::MOVING_CATEGORY), _SHOW_);
			ShowLayer(FROM_E(NEWS_LAYERS::CATEGORY_EFFECT), _SHOW_);

			ShowObject(FROM_E(NEWS_OBJECTS::FIXED_CATEGORY), _HIDE_);
			ShowObject(FROM_E(NEWS_OBJECTS::FIXED_CATEGORY_BAND), _HIDE_);
			ShowObject(FROM_E(NEWS_OBJECTS::MOVING_CATEGORY), _SHOW_);
			ShowObject(FROM_E(NEWS_OBJECTS::MOVING_CATEGORY_BAND), _SHOW_);
		}

		ShowLayer(FROM_E(NEWS_LAYERS::CONTENTS), _SHOW_);
		ShowLayer(FROM_E(NEWS_LAYERS::CONTENTS1), _SHOW_);

		ReplaceNewsData(m_prevNewsData, newsLayers[FROM_E(NEWS_LAYERS::CONTENTS)], newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)], NEWS_PAGE_NUMBER);
		ReplaceNewsData(m_curNewsData, newsLayers[FROM_E(NEWS_LAYERS::CONTENTS1)], newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS1)], NEWS_PAGE_NUMBER);
		
	}

	m_SpreadNews.DrawSelection(m_nCurNews + 1);

	m_prevNewsData = m_curNewsData;
	m_nPrevNews = m_nCurNews;


#ifdef _TAKE_IN_LOCK_ 
	std::lock_guard<std::mutex> lock(m_mutexHandler);
#endif

	m_pHDdaVinci->TakeInEx(NEWS_PAGE_NUMBER);

	return TRUE;

}


BOOL CClock2026Dlg::TakeOutNews()
{
	if (!m_pStatus->GetDispState(DISP_NEWS)) return FALSE;

	m_pStatus->SetDispStateForPage(DISP_NEWS, FALSE, NEWS_PAGE_NUMBER);

	KillTimer(ID_TIMER_NEWS_DATA);

	m_pHDdaVinci->TakeOut(NEWS_PAGE_NUMBER);
#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(NEWS_PAGE_NUMBER);
#endif
	SAFE_DELETE(m_pNewsImageSystemObject);

	SAFE_DELETE(m_pNewsAlias);

	PrintLog(_T("하단 뉴스를 아웃했습니다."));

	EnableWindow(IDC_CHECK_DISP_NEWS, FALSE);

	return TRUE;
}


UINT CClock2026Dlg::DisplayNewsThread(LPVOID pParam)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(pParam);

	while (pDlg->m_pStatus->GetDispState(DISP_NEWS))
	{
		DWORD dwCurTick = GetTickCount64();

		if (dwCurTick - pDlg->m_dwPrevTickDisplayNews > static_cast<ULONGLONG>(pDlg->m_pStatus->NewsConf()->DisplayDuration()) * THOUSAND)
		{
			pDlg->m_dwPrevTickDisplayNews = dwCurTick;
			pDlg->DisplayNews(FALSE);
		}
	}	
	return 0;
}

void CClock2026Dlg::OnBnClickedCheckDispNews()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_NEWS);

	if (bDisp)
	{
		CancelPrepareNewsScroll();

		bDisp = DisplayNews(TRUE);
		m_pStatus->SetDispStateForPage(DISP_NEWS, bDisp, NEWS_PAGE_NUMBER);

		if (bDisp)
		{
			m_dwPrevTickDisplayNews = 0;

			m_pStatus->GetNewsList()->OptimizeData();

			m_SpreadNews.UpdateDataSheet(0,FALSE, FALSE);

			SetTimer(ID_TIMER_NEWS_DATA, abs(m_pStatus->NewsConf()->DisplayDuration()) * THOUSAND, nullptr);
		
			PrintLog(_T("하단 뉴스를 송출합니다"));
		}
		else
		{
			PrintLog(_T("하단 뉴스 송출에 실패했습니다!!!"));
		}

		EnableWindow(IDC_CHECK_DISP_NEWS, bDisp);
	}
	else
	{
		TakeOutNews();
	}

	UpdateData(FALSE);
}


void CClock2026Dlg::OnEnKillfocusEditNewsTime()
{
	CString strBuffer;
	GetDlgItemText(IDC_EDIT_NEWS_DISPLAY_DURATION, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->NewsConf()->DisplayDuration(min(max(GETINT(strBuffer), MIN_NEWS_DISPLAY_DURATION), MAX_NEWS_DISPLAY_DURATION));
	}

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}


int CClock2026Dlg::Handler(LPVOID me, CString strLayer, int effectNo, int frameIdx, int frameLength, int totalidx, int reversetotalidx, float* in, int* out, LPVOID pAgent)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(me);

	if (pDlg->m_pStatus->GetDispState(DISP_NEWS_SCROLL))
	{
		if (pDlg->m_pHDdaVinci->GetScene(NEWS_PAGE_NUMBER))
		{
			if (pDlg->m_pHDdaVinci->GetAgent(NEWS_PAGE_NUMBER) == pAgent)
			{
				if (newsLayers[FROM_E(NEWS_LAYERS::SCROLL)] == strLayer)
				{
					if (frameIdx == frameLength - 2 ) // 레이어의 끝이면 한 프레임 앞으로 돌린다
					{
						pDlg->m_nNewScrollCounter++;

						if (pDlg->m_nNewScrollCounter > pDlg->m_pStatus->NewsConf()->ScrollIteration())
						{
							pDlg->PostMessage(WM_NEWS_SCROLL_FINISH, pDlg->m_bPrepareNewsScroll, 0);
							return 0;
						}
						else
						{
							pDlg->m_pHDdaVinci->EffectResetScroll(newsEffects[FROM_E(NEWS_EFFECTS::SCROLL)], NEWS_PAGE_NUMBER);
							pDlg->m_pHDdaVinci->GotoEx(NEWS_PAGE_NUMBER, 0);

							pDlg->GetDlgItem(IDC_STATIC_NEWS_SCROLL_COUNTER)->SetWindowText(GETSTR(pDlg->m_nNewScrollCounter));

							pDlg->PrintLog(GETSTR(pDlg->m_nNewScrollCounter)+ _T("번째 스크롤 시작합니다."));
						}

					}
				}
			}
		}

	}
	return 0;
}

int CClock2026Dlg::HandlerForAge(LPVOID me, CString strLayer, int effectNo, int frameIdx, int frameLength, int totalidx, int reversetotalidx, float* in, int* out, LPVOID pAgent)
{
	
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(me);

	if (pDlg->m_pStatus->GetDispState(DISP_AGE) || pDlg->m_pStatus->GetDispState(DISP_AGE_NOTICE))
	{
		if (pDlg->m_pHDdaVinci->GetScene(AGE_PAGE_NUMBER))
		{
			if (pDlg->m_pHDdaVinci->GetAgent(AGE_PAGE_NUMBER) == pAgent)
			{
					if (AgeLayers[FROM_E(AGE_LAYERS::AGE)] == strLayer)
					{
						if (pDlg->m_pStatus->GetDispState(DISP_AGE_NOTICE))
						{
							pDlg->m_pAgeObject->SetPositionByAbs(pDlg->m_nAgePosX, pDlg->m_nAgePosY);
						}
						pDlg->m_pAgeObject->SetShow(pDlg->m_bAgeShow);
						pDlg->m_pHDdaVinci->GetCGXLayer(AgeLayers[FROM_E(AGE_LAYERS::AGE)], AGE_PAGE_NUMBER)->Prepare();

						if (frameIdx == totalidx) // 레이어의 끝이면 한 프레임 앞으로 돌린다
						{
#ifdef _TAKE_IN_LOCK_
							std::lock_guard<std::mutex> lock(pDlg->m_mutexHandler);
#endif
							pDlg->m_pHDdaVinci->GotoEx(AGE_PAGE_NUMBER, 0);
						}
					}
			}
		}
	}

	return 0;
}

int CClock2026Dlg::HandlerForInputVideo(LPVOID me, CString strLayer, int effectNo, int frameIdx, int frameLength, int totalidx, int reversetotalidx, float* in, int* out, LPVOID pAgent)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(me);

	if (pDlg->m_pHDdaVinci->GetScene(LIVE_VIDEO_PAGE_NUMBER) && pDlg->m_pStatus->GetDispState(DISP_LIVE_VIDEO))
	{
		if (pDlg->m_pHDdaVinci->GetAgent(LIVE_VIDEO_PAGE_NUMBER) == pAgent)
		{
			if (LiveVideoLayers[FROM_E(LIVE_VIDEO_LAYERS::LIVE_INPUT_VIDEO)] == strLayer)
			{
				if (pDlg->m_pLiveVideo->m_CapturedVideoFIFO.Size() > 1)
				{
					pDlg->m_pInputLiveVideoObject->SetShow(TRUE);
					pDlg->m_pInputLiveVideoMaskObject->SetShow(FALSE);

					pDlg->SetPosLiveVideoObjects();

					PULONG pInputVideoFrame = (PULONG)(pDlg->m_pLiveVideo->m_CapturedVideoFIFO.Read_PopHead());

					if (pInputVideoFrame != nullptr)
					{
						pDlg->m_pInputLiveVideoObject->ReplaceBuffer(pInputVideoFrame, pDlg->m_pLiveVideo->m_targetWidth, pDlg->m_pLiveVideo->m_targetHeight);

						delete pInputVideoFrame;
					}

					pDlg->m_pInputLiveVideoLayer->Prepare();


					if (frameIdx == totalidx)
					{
#ifdef _TAKE_IN_LOCK_
						std::lock_guard<std::mutex> lock(pDlg->m_mutexHandler);
#endif
						pDlg->m_pHDdaVinci->GotoEx(LIVE_VIDEO_PAGE_NUMBER, 0);   // 무조건 첫번째 프레임만 돌리면 됨
					}
				}
			}
		}
	}

	return 0;
}


BOOL CClock2026Dlg::PrepareNewsScroll()
{
	if (m_bPrepareNewsScroll) return TRUE;

	if (m_pStatus->IsOnAir())
	{
		PrintLog(_T("스크롤은 다른페이지 송출 중에는 만들수가 없습니다!!!"),_T("경고"),TRUE);
		return FALSE;
	}

	CString strScrollTemplate = GetScrollDir() + m_pStatus->NewsConf()->ScrollTemplate();

	if (!CFileUtils::ExistFile(strScrollTemplate))
	{
		PrintLog(strScrollTemplate + _T(" 파일이 없습니다. 스크롤 준비를 중단합니다!!!"), _T("오류"), TRUE);
		return FALSE;
	}

	m_pHDdaVinci->Open(strScrollTemplate, 1, NEWS_PAGE_NUMBER);

	CCGXLayer*  pCrawlLayer = m_pHDdaVinci->GetCGXLayer(newsLayers[FROM_E(NEWS_LAYERS::SCROLL)],NEWS_PAGE_NUMBER);
	CCGXObject* pPrefix = m_pHDdaVinci->GetCGXObject(newsObjects[FROM_E(NEWS_OBJECTS::PREFIX)], NEWS_PAGE_NUMBER);
	CCGXObject* pContents = m_pHDdaVinci->GetCGXObject(newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)], NEWS_PAGE_NUMBER);
	CCGXObject* pBand = m_pHDdaVinci->GetCGXObject(newsObjects[FROM_E(NEWS_OBJECTS::BAND)], NEWS_PAGE_NUMBER);
	CCGXEffect* pScrollEffect = m_pHDdaVinci->GetCGXEffect(newsEffects[FROM_E(NEWS_EFFECTS::SCROLL)], NEWS_PAGE_NUMBER);
	CCGXObject* pImage = nullptr;

	CCGXObject* pClonePrefix = nullptr;
	CCGXObject* pCloneContents = nullptr;
	CCGXObject* pCloneImage = nullptr;

	INT nPrevXPos = 0;
	INT nPrevYPos = 0; // 데이터 버리는 곳으로 사용

	CString strProgress;

	CString strPrefix;

	ALIAS_COORD PrefixCoord;
	ALIAS_COORD ContentsCoord;
	ALIAS_COORD BandCoord;

	INT nBandWidth[1];

	INT nSpaceWidth = 0;// 스페이스 문자의 폭을 구해야한다. 왜냐하면 스페이스의 위치나 폭은 SDK에서 0으로 반환하기 때문이다.

	INT nItemGap = m_pStatus->NewsConf()->ScrollItemGap();

	INT nPrefixGap = m_pStatus->NewsConf()->ScrollPrefixGap();


	INT nFirstItemXPos = 0;

	INT nTotalNewsCount = m_pStatus->GetNewsList()->Size();

	CNewsData newsData;

	m_pWndProgress->SetWindowText(_T("뉴스 스크롤 생성 중..."));
	m_pWndProgress->SetRange(0, nTotalNewsCount);
	m_pWndProgress->ShowWindow(SW_SHOW);


	if (pPrefix != nullptr)
	{
		pPrefix->GetPositionEx(&PrefixCoord.nOrigX, &PrefixCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);
		strPrefix = pPrefix->GetContents();
		PrefixCoord.nDiffY = (PrefixCoord.nOrigY - BandCoord.nOrigY);
	}

	if (pContents != nullptr)
	{
		pContents->GetPositionEx(&ContentsCoord.nOrigX, &ContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);
		pContents->Replace(_T("가"), 0);
		nSpaceWidth = pContents->GetWidth() / 2;

		pContents->SetShow(false);

		ContentsCoord.nDiffY = (ContentsCoord.nOrigY - PrefixCoord.nOrigY);

		m_pHDdaVinci->SetSystemObjectRect(CG_CMD_OBJECT_IMAGE, CRect(0, 0, pContents->GetHeight(), pContents->GetHeight()));

		pImage = new CCGXObject(m_pHDdaVinci->GetSystemObject(CG_CMD_OBJECT_IMAGE));   // 레퍼런스 이미지 생성
	}


	BOOL bFirstItem = TRUE;

	for (int i = 0; i < nTotalNewsCount; i++)
	{
		newsData = m_pStatus->GetNewsList()->GetAt(i);

		if (newsData.GetContentString() != BLANK_STRING && newsData.IsAvailable())
		{
			CString strPrefixAlias = newsObjects[FROM_E(NEWS_OBJECTS::PREFIX)] + GETSTR(i);

			m_pHDdaVinci->AddObject(pCrawlLayer, pPrefix, CG_CMD_OBJECT_TEXT,strPrefixAlias, NEWS_PAGE_NUMBER);
			pClonePrefix = m_pHDdaVinci->GetCGXObject(strPrefixAlias, NEWS_PAGE_NUMBER);

			if (bFirstItem)
			{
				pClonePrefix->SetPositionByAbsEx(m_pStatus->NewsConf()->ScrollPrefixGap() * 20, PrefixCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER); // 경험상 설정함
				pClonePrefix->GetPositionEx(&PrefixCoord.nOrigX, &PrefixCoord.nOrigY, CGX_REFERENCE_POINT_RIGHTCENTER);
				pClonePrefix->GetPositionEx(&nFirstItemXPos, &PrefixCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);

				bFirstItem = FALSE;
			}
			else
			{
				PrefixCoord.nX = nPrevXPos + nItemGap;
				pClonePrefix->SetPositionByAbsEx(PrefixCoord.nX, PrefixCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);
			}

			pClonePrefix->GetPositionEx(&nPrevXPos, &nPrevYPos, CGX_REFERENCE_POINT_RIGHTCENTER);


			std::vector<CNewsData> vNewsDataList;
			
			newsData.ParseItem(vNewsDataList);

			for (int j = 0; j < vNewsDataList.size(); j++)
			{
				CNewsData newsDataBuffer = vNewsDataList.at(j);
				newsDataBuffer.ExtractTextFormat();

				if (newsDataBuffer.Fonts().at(0) != IMAGE_FILE_TAG)  // 그림파일이 아닌 문자면
				{
					CString strContentsAlias = newsObjects[FROM_E(NEWS_OBJECTS::CONTENTS)] + GETSTR(i) + _T("_") + GETSTR(j);

					m_pHDdaVinci->AddObject(pCrawlLayer, pContents, CG_CMD_OBJECT_TEXT,strContentsAlias , NEWS_PAGE_NUMBER);

					pCloneContents = m_pHDdaVinci->GetCGXObject(strContentsAlias, NEWS_PAGE_NUMBER);

					if (j == 0)
					{
						pCloneContents->SetPositionByAbsEx(nPrevXPos + nPrefixGap, ContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);
					}
					else
					{
						pCloneContents->SetPositionByAbsEx(nPrevXPos, ContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);
					}

					CString strContents = newsDataBuffer.GetContentString();

					strContents = strContents.TrimRight(_T("\r\n"));

					CString strTrim = strContents;

					strTrim.TrimRight();

					INT nSpaceCount = (strContents.GetLength() - strTrim.GetLength());

					if (strTrim.GetLength() != 0)  // 전체가 스페이스 문자로 이루어지지 않아야 한다. 만약 전체가 스페이스 문자라면 nPrevXPos가 0값이 나와서 스크롤이 어그러진다.
					{

						m_pHDdaVinci->ObjectSetTextFontColorEx(strContentsAlias,
							strContents,
							newsDataBuffer.Fonts(),
							newsDataBuffer.Colors(),
							newsDataBuffer.Sizes(),
							_T("KBSSymbol1"), 
							100, 
							100,
							NEWS_PAGE_NUMBER,
							0);

						pCloneContents->GetPositionEx(&nPrevXPos, &ContentsCoord.nY, CGX_REFERENCE_POINT_RIGHTCENTER);
					}
					else
					{
						pCloneContents->SetShow(false);
					}

					nPrevXPos = nPrevXPos + nSpaceWidth * nSpaceCount; // 좌우측에 있는 스페이스 문자를 고려해줘야한다.

				}
				else                                                                       // 그림에 대한 처리
				{
					if (pCrawlLayer != nullptr && pImage != nullptr)          // 레이어와 테일이미지가 있어야 이미지 추가가 가능하다.
					{
						CString strImageAlias = IMAGE_FILE_TAG + GETSTR(i) + _T("_") + GETSTR(j);

						m_pHDdaVinci->AddObject(pCrawlLayer, pImage, CG_CMD_OBJECT_IMAGE,strImageAlias , NEWS_PAGE_NUMBER);

						pCloneImage = m_pHDdaVinci->GetCGXObject(strImageAlias, NEWS_PAGE_NUMBER);

						CString strImageFileName = newsDataBuffer.Contents().at(0);

						if (CFileUtils::ExistFile(strImageFileName))   // 이미지 파일이 존재하여야 한다.
						{
							CRect rt = m_RENewsData.GetImageRect(strImageFileName);

							pCloneImage->Replace(strImageFileName, 0);

							INT nImageHeight = static_cast<INT>(rt.Height() * (static_cast<FLOAT>(newsDataBuffer.Sizes().at(0)) / 100.0f));
								INT nImageWidth = 0;

								INT nImageHeightArray[1] = { nImageHeight };

								nImageWidth = static_cast<INT>(rt.Width() * (static_cast<FLOAT>(nImageHeight) / static_cast<FLOAT>(rt.Height())));  // 밴드높이와 같을 때의 이미지 폭을 구함

							INT nImageWidthArray[1] = { nImageWidth };

							m_pHDdaVinci->SetProperty(pCloneImage, CGX_PROPERTY_WIDTH, nImageWidthArray, 1);
							m_pHDdaVinci->SetProperty(pCloneImage, CGX_PROPERTY_HEIGHT, nImageHeightArray, 1);


							if (j == 0)
							{
								pCloneImage->SetPositionByAbsEx(nPrevXPos + nPrefixGap, ContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);  // Y좌표는 맨처음 Prefix의 좌표를 사용한다.
							}
							else
							{
								pCloneImage->SetPositionByAbsEx(nPrevXPos, ContentsCoord.nOrigY, CGX_REFERENCE_POINT_LEFTCENTER);  // Y좌표는 맨처음 Prefix의 좌표를 사용한다.

							}
							pCloneImage->GetPositionEx(&nPrevXPos, &ContentsCoord.nY, CGX_REFERENCE_POINT_RIGHTCENTER);
						}
						else
						{
							pCloneImage->SetShow(false);
							PrintLog(strImageFileName + _T(" 파일이 없습니다.!!!"));
						}
					}
					else
					{
						PrintLog(_T("스크롤 레이어가 없어 이미지 추가가 불가능합니다. "));
					}
				}
			}
		}
		m_pWndProgress->SetPos(i);

		strProgress.Format(_T("뉴스 스크롤 생성 중... [%d/%d(%d%%)]"), i, nTotalNewsCount, i * 100 / nTotalNewsCount);
		m_pWndProgress->SetWindowText(strProgress);
		m_pWndProgress->SetText(GETSTR(i) + _T("번째 아이템 생성 중..."));
	}

	m_pWndProgress->SetWindowText(_T("스크롤  마무리 작업 중..."));
	m_pWndProgress->SetText(_T("SetPageHandler... "));

	if (pCrawlLayer != nullptr) pCrawlLayer->Prepare();


	pPrefix->SetShow(FALSE);
	pContents->SetShow(FALSE);


	if (!pScrollEffect)
	{
		PrintLog( _T("스크롤 이펙트가 없습니다! 스크롤 준비를 중단합니다!!!"), _T("오류"), TRUE);
		return FALSE;
	}
	
	pScrollEffect->SetScrollSpeed(m_pStatus->NewsConf()->ScrollSpeed());
	
	m_pHDdaVinci->SetNoTransition(false, NEWS_PAGE_NUMBER);

	m_pHDdaVinci->SetPageHandler(this, CClock2026Dlg::Handler);

	m_pWndProgress->SetText(_T("Preparing... "));
	m_pHDdaVinci->Prepare(NEWS_PAGE_NUMBER);

	m_pWndProgress->SetText(_T("Setting Scene... "));
	m_pHDdaVinci->SetScene(NEWS_PAGE_NUMBER, m_pHDdaVinci->GetScene(NEWS_PAGE_NUMBER), 0);

	m_pWndProgress->ShowWindow(SW_HIDE);

	SAFE_DELETE(pImage);

	m_bPrepareNewsScroll = TRUE;
	PrintLog(_T("Prepare를 완료하였습니다."));
	return TRUE;
}

LRESULT CClock2026Dlg::OnNewsScrollFinish(WPARAM wParam, LPARAM lParam)
{
	m_pStatus->SetDispStateForPage(DISP_NEWS_SCROLL, FALSE, NEWS_PAGE_NUMBER);

	m_pHDdaVinci->EffectResetScroll(newsEffects[FROM_E(NEWS_EFFECTS::SCROLL)], NEWS_PAGE_NUMBER);

	m_pHDdaVinci->SetPageHandler(this, nullptr);

	m_pHDdaVinci->TakeOut(NEWS_PAGE_NUMBER);

#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(NEWS_PAGE_NUMBER);
#endif

	CancelPrepareNewsScroll();

	m_nNewScrollCounter = 1;
	
	PrintLog(_T("스크롤을 아웃합니다"));

	EnableWindow(IDC_CHECK_DISP_NEWS_SCROLL, FALSE);

	UpdateData(FALSE);

	return 0;
}

LRESULT CClock2026Dlg::OnNoticeFinish(WPARAM wParam, LPARAM lParam)
{
	TakeOutNotice();

	return 0;
}

void CClock2026Dlg::OnBnClickedCheckPrepareNewsScroll()
{
	m_bPrepareNewsScroll = PrepareNewsScroll();

	UpdateData(FALSE);
}


BOOL CClock2026Dlg::TakeInNewsScroll(void)
{
	if (m_pStatus->IsOnAir())
	{
		PrintLog(_T("스크롤은 다른페이지 송출 중에는 송출 할 수가 없습니다!!!"), _T("경고"), TRUE);
		m_pStatus->SetDispStateForPage(DISP_NEWS_SCROLL, FALSE, NEWS_PAGE_NUMBER);
		return FALSE;
	}

	if (!m_pStatus->GetNewsList()->HasAvailableData())
	{
		PrintLog(_T("송출할 데이터가 없습니다!"), _T("경고"), TRUE);
		m_pStatus->SetDispStateForPage(DISP_NEWS_SCROLL, FALSE, NEWS_PAGE_NUMBER);
		return FALSE;
	}

	if (!PrepareNewsScroll())
	{
		PrintLog(_T("스크롤 준비에 실패 했습니다!!!"), _T("오류"), TRUE);
		m_pStatus->SetDispStateForPage(DISP_NEWS_SCROLL, FALSE, NEWS_PAGE_NUMBER);
		return FALSE;
	}
	m_pHDdaVinci->TakeIn(NEWS_PAGE_NUMBER);
	m_pStatus->SetDispStateForPage(DISP_NEWS_SCROLL, TRUE, NEWS_PAGE_NUMBER);
	PrintLog(_T("스크롤을 송출합니다"), _T("알림"), TRUE);

	return TRUE;
}

BOOL CClock2026Dlg::TakeOutNewsScroll(BOOL bPrepare)
{
	if (!m_pStatus->GetDispState(DISP_NEWS_SCROLL)) return FALSE;

	SendMessage(WM_NEWS_SCROLL_FINISH, bPrepare, 0);

	return TRUE;
}

void CClock2026Dlg::OnBnClickedCheckDispNewsScroll()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_NEWS_SCROLL);

	if (bDisp)
	{
		bDisp = TakeInNewsScroll();

		EnableWindow(IDC_CHECK_DISP_NEWS_SCROLL, bDisp);
	}
	else
	{
		TakeOutNewsScroll(m_bPrepareNewsScroll);
	}

	UpdateData(FALSE);
}

void CClock2026Dlg::LoadWeatherCity()
{
	auto cities = m_pStatus->WeatherConf()->MapCode_SelectCity();
	m_pStatus->GetWeatherList()->UpdateSelectCity(cities);
	
	m_SpreadWeather.UpdateDataSpread(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonWeatherCitySelect()
{
	CWeatherSelectDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		m_pStatus->GetWeatherList()->ResetReadTime();
		LoadWeatherCity();
		SetChangeMark(TRUE);
	}
}

void CClock2026Dlg::UpdateWeatherTime(void)
{
	CString strBuffer;

	strBuffer = m_pStatus->GetWeatherList()->GetTimeReadWeatherAndAir().Format(_T("%Y년 %m월 %d일 %H시 %M분 데이터"));

	SetDlgItemText(IDC_STATIC_WEATHER_TIME, strBuffer);
}

HBRUSH CClock2026Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_WEATHER_TIME:
		pDC->SetTextColor(RGB(0xFF, 0x00, 0x00));
		break;
	}

	return hbr;
}

void CClock2026Dlg::GetWeatherAndAirData(void)
{
	GetDlgItem(IDC_BUTTON_GET_KBS_WEATHER)->EnableWindow(FALSE);

	if (!m_pStatus->WeatherConf()->ExistSelectCity())
	{
		PrintLog(_T("선택한 지역이 없습니다. 지역설정 후 다시 시도하세요!!!."));	
	}
	else
	{
		PrintLog(_T("KBS 재난정보시스템 날씨를 읽기 시작합니다."));

		CTime CurTime = CTime::GetCurrentTime();

		if (GETTIMESPAN(m_pStatus->GetWeatherList()->GetTimeReadWeatherAndAir(), CurTime).GetTotalSeconds() < 10) // 데이터 읽은지 10분 미만이면 그냥 통과 마진 5초
		{
			PrintLog_Message(_T("기상 데이터 읽은 후 10초 지나야 가능합니다!"), TRUE);
			GetDlgItem(IDC_BUTTON_GET_KBS_WEATHER)->EnableWindow(TRUE);
			return;
		}

		BOOL bExceptAir = m_pStatus->WeatherConf()->ExceptAir();
		if (m_pStatus->GetWeatherList()->RefreshWeatherAndAirData(true,bExceptAir))
		{
			if (bExceptAir)
			{
				PrintLog(_T("날씨 정보를 읽었습니다."));
			}
			else
			{
				PrintLog(_T("날씨와 미세먼지 정보를 읽었습니다."));
			}

			m_pStatus->GetWeatherList()->SetTimeReadWeatherAndAir(CTime::GetCurrentTime());
			UpdateWeatherTime();
			SetChangeMark(TRUE);

			m_SpreadWeather.UpdateDataSpread(FALSE);
		}
	}

	GetDlgItem(IDC_BUTTON_GET_KBS_WEATHER)->EnableWindow(TRUE);
}

BOOL CClock2026Dlg::RunWeatherThread(void)
{
	CWinThread* pThread = AfxBeginThread(AutoReadingWeatherThread, this);

	//if (WAIT_TIMEOUT == WaitForSingleObject(pThread->m_hThread, WAIT_TIME_FOR_WEATHER_READING))
	//{
	//	PrintLog(_T("날씨 서버 Time Out"));
	//	return FALSE;
	//}

	return TRUE;
}

void CClock2026Dlg::OnBnClickedButtonGetKbsWeather()
{
	RunWeatherThread();
}

void CClock2026Dlg::OnBnClickedButtonClearWeatherData()
{
	m_pStatus->GetWeatherList()->ClearWeatherAndAirData();
	m_SpreadWeather.UpdateDataSpread(FALSE);
	PrintLog(_T("날씨와 미세먼지 정보를 지웠습니다!"));
}


void CClock2026Dlg::OnBnClickedButtonClearAllWeatherData()
{
	m_pStatus->GetWeatherList()->DeleteAllData();
	m_SpreadWeather.UpdateDataSpread(FALSE);
	PrintLog(_T("날씨와 미세먼지 정보를 모두 지웠습니다!"));

}


UINT CClock2026Dlg::AutoReadingWeatherThread(LPVOID lpvoid)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(lpvoid);

	if (!IsInternetConnected())
	{
		pDlg->PrintLog(_T("네트워크에 문제가 있어 날씨 데이터를 읽을 수가 없습니다."),_T("오류"),TRUE);
		pDlg->m_pStatus->GetWeatherList()->ClearWeatherAndAirData();
		pDlg->m_SpreadWeather.UpdateDataSpread(FALSE);
		return 0;
	}

	std::lock_guard<std::mutex> lock(pDlg->m_mutexReadWeather);

	pDlg->GetWeatherAndAirData();

	pDlg->SetActiveWindow();
	pDlg->SetForegroundWindow();

	return 0;
}

void CClock2026Dlg::ConfigTimerForAutoReadingWeather(BOOL bAutoReading)
{

	if (bAutoReading)
	{
		INT nVal = m_pStatus->WeatherConf()->AutoReadingPeriod();

		SetTimer(ID_TIMER_AUTO_WEATHER_READING, nVal * 60 * 1000, nullptr);
	}
	else
	{
		KillTimer(ID_TIMER_AUTO_WEATHER_READING);
	}

	m_pStatus->WeatherConf()->AutoReading(bAutoReading);	
}

void CClock2026Dlg::OnBnClickedCheckAutoWeatherReading()
{
	BOOL bAutoReading = IsDlgButtonChecked(IDC_CHECK_AUTO_WEATHER_READING);

	ConfigTimerForAutoReadingWeather(bAutoReading);

	UpdateData(FALSE);
}


void CClock2026Dlg::OnEnKillfocusEditAutoReadingWeatherPeriodMin()
{
	INT nOld = m_pStatus->WeatherConf()->AutoReadingPeriod();

	CString strVal;
	GetDlgItemText(IDC_EDIT_AUTO_READING_WEATHER_PERIOD_MIN, strVal);

	INT nVal = GETINT(strVal);

	if (nVal >= 10 && nVal <= 60)
	{
		m_pStatus->WeatherConf()->AutoReadingPeriod(nVal);
	}
	else
	{
		m_pStatus->WeatherConf()->AutoReadingPeriod(nOld);
	}

	ConfigTimerForAutoReadingWeather(m_pStatus->WeatherConf()->AutoReading());

	UpdateData(FALSE);
}


BOOL CClock2026Dlg::DisplayWeather(BOOL bFirst)
{
	BOOL bResult = TRUE;

	if (bFirst)
	{
		if (!OpenFpg(_T("날씨_먼지"), WEATHER_PAGE_NUMBER, GetMirDir()))
		{
			return FALSE;
		}
	}

	if (bFirst)
	{
		m_nWeatherSize = m_pStatus->GetWeatherList()->Size();

		m_nCurWeather = (m_pStatus->GetWeatherList()->GetCurIndex() >= m_nWeatherSize) ? 0 : m_pStatus->GetWeatherList()->GetCurIndex();

		if (!m_pStatus->GetWeatherList()->HaveAvailableData())
		{
			PrintLog(_T("송출할 날씨 데이터가 존재하지 않습니다."));
			bResult = FALSE;
		}

		if (bResult == FALSE)
		{
			return bResult;
		}

		for (int i = FROM_E(WEATHER_LAYERS::WEATHER1); i < FROM_E(WEATHER_LAYERS::LAST_NUM); i++)
		{
			m_pWeatherLayer[i] = m_pHDdaVinci->GetCGXLayer(weatherLayers[i], WEATHER_PAGE_NUMBER);
		}

		for (int i = FROM_E(WEATHER_OBJECTS::WEATHER_CITYNAME_1); i < FROM_E(WEATHER_OBJECTS::LAST_NUM); i++)
		{
			m_pWeatherObject[i] = m_pHDdaVinci->GetCGXObject(weatherObjects[i], WEATHER_PAGE_NUMBER);
		}

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_SYMBOL_1)]->GetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_1)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			0, 0, 0,
			&m_nWeathterSymbolOffsetX[0],
			&m_nWeathterSymbolOffsetY[0],
			&m_nWeathterSymbolOffsetZ[0]);

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_SYMBOL)]->GetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_CONCENTRATION)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			0, 0, 0,
			&m_nWeathterSymbolOffsetX[1],
			&m_nWeathterSymbolOffsetY[1],
			&m_nWeathterSymbolOffsetZ[1]);

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_SYMBOL_2)]->GetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_2)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			0, 0, 0,
			&m_nWeathterSymbolOffsetX[2],
			&m_nWeathterSymbolOffsetY[2],
			&m_nWeathterSymbolOffsetZ[2]);

	}
	else
	{
		if (++m_nCurWeather >= m_nWeatherSize) m_nCurWeather = 0;

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_SYMBOL_1)]->GetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_1)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			0, 0, 0,
			&m_nWeathterSymbolOffsetX[0],
			&m_nWeathterSymbolOffsetY[0],
			&m_nWeathterSymbolOffsetZ[0]);

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_SYMBOL)]->GetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_CONCENTRATION)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			0, 0, 0,
			&m_nWeathterSymbolOffsetX[1],
			&m_nWeathterSymbolOffsetY[1],
			&m_nWeathterSymbolOffsetZ[1]);

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_SYMBOL_2)]->GetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_2)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			0, 0, 0,
			&m_nWeathterSymbolOffsetX[2],
			&m_nWeathterSymbolOffsetY[2],
			&m_nWeathterSymbolOffsetZ[2]);
	}


	CWeatherData weatherData = m_pStatus->GetWeatherList()->GetAt(m_nCurWeather);

	while (!weatherData.IsAvailable())
	{
		if (++m_nCurWeather >= m_nWeatherSize) m_nCurWeather = 0;

		weatherData = m_pStatus->GetWeatherList()->GetAt(m_nCurWeather);
	}

	BOOL bDustEmpty = weatherData.IsEmptyDust();

	m_pWeatherLayer[FROM_E(WEATHER_LAYERS::WEATHER1)]->SetShow(!bDustEmpty);
	m_pWeatherLayer[FROM_E(WEATHER_LAYERS::WEATHER2)]->SetShow(bDustEmpty);
	m_pWeatherLayer[FROM_E(WEATHER_LAYERS::DUST)]->SetShow(!bDustEmpty);

	if (!bDustEmpty)
	{
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_CITYNAME_1)]->Replace(weatherData.GetValue(WEATHER_FIELD_INDEX::CITY_NAME), FALSE);
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_ICON_1)]->Replace(weatherData.GetIconFileName(), FALSE);
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_1)]->Replace(weatherData.GetValue(WEATHER_FIELD_INDEX::WEATHER_TEMP), FALSE);

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_CITYNAME)]->Replace(weatherData.GetValue(WEATHER_FIELD_INDEX::CITY_NAME), FALSE);
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_GRADE)]->Replace(weatherData.GetDustGradeFileName(), FALSE);
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_CONCENTRATION)]->Replace(weatherData.GetValue(WEATHER_FIELD_INDEX::PM25AVG), FALSE);

		INT nSolidColor[2] = { 0, static_cast<INT>(weatherData.GetDustColor()) };

		m_pHDdaVinci->SetProperty(m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_CONCENTRATION)], CGX_PROPERTY_COLOR_SOLID, nSolidColor, 2);

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_SYMBOL_1)]->SetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_1)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			-m_nWeathterSymbolOffsetX[0],
			-m_nWeathterSymbolOffsetY[0],
			-m_nWeathterSymbolOffsetZ[0]);


		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_SYMBOL)]->SetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::DUST_CONCENTRATION)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			-m_nWeathterSymbolOffsetX[1],
			-m_nWeathterSymbolOffsetY[1],
			-m_nWeathterSymbolOffsetZ[1]);
	}
	else
	{
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_CITYNAME_2)]->Replace(weatherData.GetValue(WEATHER_FIELD_INDEX::CITY_NAME), FALSE);
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_ICON_2)]->Replace(weatherData.GetIconFileName(), FALSE);
		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_2)]->Replace(weatherData.GetValue(WEATHER_FIELD_INDEX::WEATHER_TEMP), FALSE);

		m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_SYMBOL_2)]->SetPositionByBuddy(
			m_pWeatherObject[FROM_E(WEATHER_OBJECTS::WEATHER_TEMP_2)],
			CGX_REFERENCE_POINT_LEFTBOTTOM,
			CGX_REFERENCE_POINT_RIGHTBOTTOM,
			-m_nWeathterSymbolOffsetX[2],
			-m_nWeathterSymbolOffsetY[2],
			-m_nWeathterSymbolOffsetZ[2]);
	}

	m_SpreadWeather.DrawSelection(m_nCurWeather + 1);

#ifdef _TAKE_IN_LOCK_
	std::lock_guard<std::mutex> lock(m_mutexHandler);
#endif

	m_pHDdaVinci->TakeInEx(WEATHER_PAGE_NUMBER);

	return bResult;
}

BOOL CClock2026Dlg::TakeOutWeather(void)
{
	if (!m_pStatus->GetDispState(DISP_WEATHER)) return FALSE;

	KillTimer(ID_TIMER_WEATHER_DATA);

	m_pStatus->SetDispStateForPage(DISP_WEATHER, FALSE, WEATHER_PAGE_NUMBER);

	m_pHDdaVinci->TakeOut(WEATHER_PAGE_NUMBER);
#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(WEATHER_PAGE_NUMBER);
#endif
	PrintLog(_T("날씨를 송출을 아웃했습니다"));

	EnableWindow(IDC_CHECK_DISP_WEATHER, FALSE);

	return TRUE;
}

UINT CClock2026Dlg::DisplayWeatherThread(LPVOID pParam)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(pParam);

	while (pDlg->m_pStatus->GetDispState(DISP_WEATHER))
	{
		DWORD dwCurTick = GetTickCount64();

		if (dwCurTick - pDlg->m_dwPrevTickDisplayWeather > pDlg->m_pStatus->WeatherConf()->DisplayDuration() * THOUSAND)
		{
			pDlg->m_dwPrevTickDisplayWeather = dwCurTick;

			pDlg->DisplayWeather(FALSE);
		}
	}
	return 0;
}

void CClock2026Dlg::OnBnClickedCheckDispWeather()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_WEATHER);

	if (bDisp)
	{	
		CancelPrepareNewsScroll();

		bDisp = DisplayWeather(TRUE);
		m_pStatus->SetDispStateForPage(DISP_WEATHER, bDisp, WEATHER_PAGE_NUMBER);

		if (bDisp)
		{
			m_dwPrevTickDisplayWeather = 0;
			SetTimer(ID_TIMER_WEATHER_DATA, m_pStatus->WeatherConf()->DisplayDuration() * THOUSAND, nullptr);
	
			PrintLog(_T("날씨를 송출합니다"));
		}
		else
		{
			PrintLog(_T("날씨 송출에 실패했습니다!!!"));
		}

		EnableWindow(IDC_CHECK_DISP_WEATHER, bDisp);
	}
	else
	{
		TakeOutWeather();	
	}

	UpdateData(FALSE);

}

void CClock2026Dlg::OnBnClickedButtonWeatherMoveUp()
{
	CWeatherDataList* pList = m_pStatus->GetWeatherList();

	if (pList == nullptr) return;

	int nIndex = pList->GetCurIndex();

	pList->MoveUp(nIndex);

	m_SpreadWeather.UpdateCurrentSheet(FALSE);
	m_SpreadWeather.DrawSelectionForCurIndex();
}

void CClock2026Dlg::OnBnClickedButtonWeatherMoveDown()
{
	CWeatherDataList* pList = m_pStatus->GetWeatherList();

	if (pList == nullptr) return;

	int nIndex = pList->GetCurIndex();

	pList->MoveDown(nIndex);

	m_SpreadWeather.UpdateCurrentSheet(FALSE);
	m_SpreadWeather.DrawSelectionForCurIndex();
}


void CClock2026Dlg::OnBnClickedButtonNewsMoveUp()
{
	CNewsDataList* pList = m_pStatus->GetNewsList();

	if (pList == nullptr) return;

	int nIndex = pList->GetCurIndex();

	pList->MoveUp(nIndex);

	m_SpreadNews.UpdateCurrentSheet(FALSE);	
	m_SpreadNews.DrawSelectionForCurIndex();
}

void CClock2026Dlg::OnBnClickedButtonNewsMoveDown()
{
	CNewsDataList* pList = m_pStatus->GetNewsList();

	if (pList == nullptr) return;

	int nIndex = pList->GetCurIndex();

	pList->MoveDown(nIndex);

	m_SpreadNews.UpdateCurrentSheet(FALSE);
	m_SpreadNews.DrawSelectionForCurIndex();
}

void CClock2026Dlg::ReloadNoticeTemplate(void)
{
	m_SpreadNotice.RefreshTemplateFiles();
}

void CClock2026Dlg::OnBnClickedButtonReloadNoticeTemplate()
{
	ReloadNoticeTemplate();
}

BOOL CClock2026Dlg::SelectNotice(CNoticeData& noticeData)
{
	CString strFpgFile = noticeData.GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

	if(CFileUtils::ExistFile(strFpgFile))
	{
		DrawPreview(m_PreviewNotice, strFpgFile);
	}
	
	m_SpreadAlias.SetNoticeData(noticeData);

	m_SpreadAlias.UpdateDataSheet(FALSE, TRUE);

	UpdateData(FALSE);

	return TRUE;
}

BOOL CClock2026Dlg::ClearNotice(CNoticeData& noticeData)
{
	noticeData.Clear();

	m_PreviewNotice.SetFpgFile(BLANK_STRING);

	m_SpreadAlias.SetNoticeData(noticeData);

	m_SpreadAlias.UpdateDataSheet(FALSE, TRUE);

	return TRUE;
}

void CClock2026Dlg::ApplyNotice(void)
{
	m_CurNoticeData = m_SpreadAlias.GetNoticeData();

	if (m_pStatus->GetNoticeList()->IsExist(m_CurNoticeData))
	{
		m_CurNoticeData.MakeAnnotation();

		m_pStatus->GetNoticeList()->UpdateData(m_CurNoticeData);

		m_SpreadNotice.UpdateCurrentSheet(FALSE);

		ClearNotice(m_CurNoticeData);
	}
}

LRESULT CClock2026Dlg::OnApplyNotice(WPARAM wParam /* = 0 */, LPARAM lParam /* = 0 */)
{
	ApplyNotice();
	return 0;
}

void CClock2026Dlg::OnBnClickedButtonApplyNotice()
{
	ApplyNotice();
}

void CClock2026Dlg::OnBnClickedRadioAgeAll()
{
	m_pStatus->NoticeConf()->AgeMode(AGE_ALL);
	PrintLog(_T("전체 연령 고지 선택"));
	TimedMessageBox(this->GetSafeHwnd(),_T("알림"), _T("전체 연령 고지는 연령 표시가 나오지 않습니다."), 2000, MB_ICONWARNING | MB_OK);
	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedRadioAge7()
{
	m_pStatus->NoticeConf()->AgeMode(AGE_7);
	PrintLog(_T("연령 고지 7세 선택"));
	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedRadioAge12()
{
	m_pStatus->NoticeConf()->AgeMode(AGE_12);
	PrintLog(_T("연령 고지 12세 선택"));
	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedRadioAge15()
{
	m_pStatus->NoticeConf()->AgeMode(AGE_15);
	PrintLog(_T("연령 고지 15세 선택"));
	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedRadioAge19()
{
	m_pStatus->NoticeConf()->AgeMode(AGE_19);
	PrintLog(_T("연령 고지 19세 선택"));
	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::EnableClockWindow()
{
	BOOL bEntireMove = m_pStatus->ClockConf()->EntireMove();
	BOOL bMoveEnable = m_pStatus->ClockConf()->MoveEnable();

	GetDlgItem(IDC_CHECK_CLOCK_ENTIRE_MOVE)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_CHECK_CLOCK_DATE_MOVE)->EnableWindow(!bEntireMove && bMoveEnable);
	GetDlgItem(IDC_CHECK_CLOCK_TIME_MOVE)->EnableWindow(!bEntireMove && bMoveEnable);

	GetDlgItem(IDC_BUTTON_CLOCK_UP)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_BUTTON_CLOCK_DOWN)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_BUTTON_CLOCK_LEFT)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_BUTTON_CLOCK_RIGHT)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_BUTTON_CLOCK_RESET_POS)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_BUTTON_CLOCK_SNAP_LOAD_1)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_BUTTON_CLOCK_SNAP_LOAD_2)->EnableWindow(bMoveEnable);
	GetDlgItem(IDC_BUTTON_CLOCK_SNAP_LOAD_3)->EnableWindow(bMoveEnable);

	GetDlgItem(IDC_EDIT_CLOCK_MOVE_SIZE)->EnableWindow(bMoveEnable);

}

void CClock2026Dlg::EnableBigClockWindow(void)
{
	BOOL bVal = m_pStatus->BigClockConf()->MoveEnable();

	GetDlgItem(IDC_BUTTON_BIG_CLOCK_UP)->EnableWindow(bVal);
	GetDlgItem(IDC_BUTTON_BIG_CLOCK_DOWN)->EnableWindow(bVal);
	GetDlgItem(IDC_BUTTON_BIG_CLOCK_LEFT)->EnableWindow(bVal);
	GetDlgItem(IDC_BUTTON_BIG_CLOCK_RIGHT)->EnableWindow(bVal);
	GetDlgItem(IDC_BUTTON_BIG_CLOCK_RESET_POS)->EnableWindow(bVal);
	GetDlgItem(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_1)->EnableWindow(bVal);
	GetDlgItem(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_2)->EnableWindow(bVal);
	GetDlgItem(IDC_BUTTON_BIG_CLOCK_SNAP_LOAD_3)->EnableWindow(bVal);
}

void CClock2026Dlg::EnableLogoWindow(void)
{
	BOOL bKBSLogoAni = m_pStatus->LogoConf()->IsAnimationLogo();

	BOOL bVal = m_pStatus->LogoConf()->EntireMove();

	GetDlgItem(IDC_CHECK_LOGO_MOVE)->EnableWindow(!bVal && !bKBSLogoAni);
	GetDlgItem(IDC_CHECK_LOGO_LIVE_MOVE)->EnableWindow(!bVal && !bKBSLogoAni);

	GetDlgItem(IDC_CHECK_LOGO_UHD_MOVE)->EnableWindow(!bVal && !bKBSLogoAni);
	GetDlgItem(IDC_CHECK_LOGO_DONGSI_MOVE)->EnableWindow(!bVal && !bKBSLogoAni);

	bVal = m_pStatus->LogoConf()->EntireShow();

	GetDlgItem(IDC_CHECK_LOGO_SHOW)->EnableWindow(!bVal);
	GetDlgItem(IDC_CHECK_LOGO_LIVE_SHOW)->EnableWindow(!bVal);

	GetDlgItem(IDC_CHECK_LOGO_UHD_SHOW)->EnableWindow(!bVal);
	GetDlgItem(IDC_CHECK_LOGO_DONGSI_SHOW)->EnableWindow(!bVal);

	GetDlgItem(IDC_CHECK_LOGO_ENTIRE_MOVE)->EnableWindow(!bKBSLogoAni);

	GetDlgItem(IDC_BUTTON_LOGO_UP)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_LEFT)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_RIGHT)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_DOWN)->EnableWindow(!bKBSLogoAni);

	GetDlgItem(IDC_CHECK_LOGO_ENTIRE_SHOW)->EnableWindow(!bKBSLogoAni);

	GetDlgItem(IDC_BUTTON_LOGO_RESET_POS)->EnableWindow(!bKBSLogoAni);

	GetDlgItem(IDC_BUTTON_LOGO_SNAP_SAVE_1)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_SNAP_SAVE_2)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_SNAP_SAVE_3)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_SNAP_LOAD_1)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_SNAP_LOAD_2)->EnableWindow(!bKBSLogoAni);
	GetDlgItem(IDC_BUTTON_LOGO_SNAP_LOAD_3)->EnableWindow(!bKBSLogoAni);

	GetDlgItem(IDC_EDIT_LOGO_MOVE_SIZE)->EnableWindow(!bKBSLogoAni);

	if (m_pStatus->GetDispState(DISP_STATION_LOGO))
	{
		GetDlgItem(IDC_RADIO_LOGO_KBS1)->EnableWindow(!bKBSLogoAni);
		GetDlgItem(IDC_RADIO_LOGO_KBS2)->EnableWindow(!bKBSLogoAni);
		GetDlgItem(IDC_RADIO_LOGO_KBS_LOCAL)->EnableWindow(!bKBSLogoAni);
		GetDlgItem(IDC_RADIO_LOGO_KBS_ANIMATION)->EnableWindow(!bKBSLogoAni);
	}
	else
	{
		GetDlgItem(IDC_RADIO_LOGO_KBS1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LOGO_KBS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LOGO_KBS_LOCAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LOGO_KBS_ANIMATION)->EnableWindow(TRUE);
	}

}

void CClock2026Dlg::EnableManualUpWindow(void)
{
	BOOL bDisp = m_pStatus->GetDispState(DISP_MANUALUP);

	GetDlgItem(IDC_BUTTON_RELOAD_MANUALUP_TEMPLATE)->EnableWindow(!bDisp);
}

void CClock2026Dlg::EnableAgeWindow(BOOL bChecked)
{
	GetDlgItem(IDC_RADIO_AGE_ALL)->EnableWindow(!bChecked);
	GetDlgItem(IDC_RADIO_AGE_7)->EnableWindow(!bChecked);
	GetDlgItem(IDC_RADIO_AGE_12)->EnableWindow(!bChecked);
	GetDlgItem(IDC_RADIO_AGE_15)->EnableWindow(!bChecked);
	GetDlgItem(IDC_RADIO_AGE_19)->EnableWindow(!bChecked);
}

void CClock2026Dlg::EnableNoticeWindow(BOOL bChecked)
{
	GetDlgItem(IDC_SPREAD_NOTICE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_NOTICE_MOVE_UP)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_NOTICE_MOVE_DOWN)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_RELOAD_NOTICE_TEMPLATE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LOAD_FPR)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_APPLY_NOTICE)->EnableWindow(!bChecked);

	GetDlgItem(IDC_EDIT_NOTICE_REPETITION)->EnableWindow(!bChecked);
	GetDlgItem(IDC_SPREAD_ALIAS)->EnableWindow(!bChecked);
}

void CClock2026Dlg::EnableLiveVideoWindow(BOOL bChecked)
{
	GetDlgItem(IDC_COMBO_LIVE_VIDEO_TEMPLATES)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_RELOAD_LIVE_VIDEO_TEMPLATE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_CHECK_LIVE_VIDEO_MOVE_ENABLE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_RESET_POS)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_LEFT)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_RIGHT)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_UP)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_DOWN)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_1)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_2)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_3)->EnableWindow(!bChecked);
}

void CClock2026Dlg::EnableWeatherWindow(BOOL bChecked)
{
	GetDlgItem(IDC_BUTTON_GET_KBS_WEATHER)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_WEATHER_CITY_SELECT)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_CLEAR_WEATHER_DATA)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_CLEAR_ALL_WEATHER_DATA)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_WEATHER_DISPLAY_DURATION)->EnableWindow(!bChecked);
	GetDlgItem(IDC_CHECK_AUTO_WEATHER_READING)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_AUTO_READING_WEATHER_PERIOD_MIN)->EnableWindow(!bChecked);
	GetDlgItem(IDC_SPREAD_WEATHER)->EnableWindow(!bChecked);
}

void CClock2026Dlg::EnableNewsScrollWindow(BOOL bChecked)
{
	GetDlgItem(IDC_COMBO_NEWS_SCROLL_TEMPLATE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_RELOAD_NEWS_SCROLL_TEMPLATE)->EnableWindow(!bChecked);

	GetDlgItem(IDC_EDIT_NEWS_SCROLL_ITERATION)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_NEWS_SCROLL_SPEED)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_NEWS_SCROLL_PREFIX_GAP)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_NEWS_SCROLL_ITEM_GAP)->EnableWindow(!bChecked);
}

void CClock2026Dlg::EnableNewsWindow(BOOL bChecked)
{
	INT nShow = bChecked ? SW_HIDE : SW_SHOW;

	GetDlgItem(IDC_SPREAD_NEWS)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_NEWS_MOVE_UP)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_NEWS_MOVE_DOWN)->EnableWindow(!bChecked);
	GetDlgItem(IDC_RICHEDIT_NEWS_DATA)->EnableWindow(!bChecked);

	GetDlgItem(IDC_COMBO_NEWS_CATEGORY)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_DELETE_NEWS_CATEGORY_FROM_COMBO)->EnableWindow(!bChecked);
	GetDlgItem(IDC_COMBO_NEWS_TEXT_FONT)->EnableWindow(!bChecked);
	GetDlgItem(IDC_COMBO_NEWS_TEXT_SIZE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_DELETE_NEWS_TEXT_SIZE_FROM_COMBO)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_NEWS_SYMBOL_SIZE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_NEWS_HANJA_SIZE)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_NEWS_TEXT_COLOR)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_RE_BACK_COLOR)->EnableWindow(!bChecked);
	GetDlgItem(IDC_CHECK_APPLY_SCROLL_COLOR)->EnableWindow(!bChecked);

	GetDlgItem(IDC_BUTTON_INSERT_IMAGE)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_1)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_2)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_3)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_4)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_5)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_6)->ShowWindow(nShow);
	GetDlgItem(IDC_BUTTON_IMAGE_SHORTCUT_CONFIG)->ShowWindow(nShow);

	GetDlgItem(IDC_BUTTON_NEWS_CLEAR)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_NEWS_SPECIAL_CHAR)->EnableWindow(!bChecked);
	GetDlgItem(IDC_BUTTON_SPELL_CHECK)->EnableWindow(!bChecked);
	GetDlgItem(IDC_EDIT_NEWS_DISPLAY_DURATION)->EnableWindow(!bChecked);

}

void CClock2026Dlg::EnableWindow(UINT nID,BOOL bChecked)
{
	switch (nID)
	{
	case IDC_CHECK_DISP_AGE:
		GetDlgItem(IDC_CHECK_DISP_NOTICE)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_AGE_NOTICE)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_PREPARE_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NEWS));
		GetDlgItem(IDC_CHECK_DISP_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NEWS));
		GetDlgItem(IDC_CHECK_DISP_LIVE_VIDEO)->EnableWindow(!bChecked);

		EnableAgeWindow(bChecked);
		break;

	case IDC_CHECK_DISP_NOTICE:
		GetDlgItem(IDC_CHECK_DISP_AGE)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_LIVE_VIDEO) && !m_pStatus->GetDispState(DISP_NEWS_SCROLL));
		GetDlgItem(IDC_CHECK_DISP_AGE_NOTICE)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_LIVE_VIDEO) && !m_pStatus->GetDispState(DISP_NEWS_SCROLL));

		EnableNoticeWindow(bChecked);
		break;

	case IDC_CHECK_DISP_AGE_NOTICE:
		GetDlgItem(IDC_CHECK_DISP_AGE)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_NOTICE)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_PREPARE_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NEWS));
		GetDlgItem(IDC_CHECK_DISP_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NEWS));
		GetDlgItem(IDC_CHECK_DISP_LIVE_VIDEO)->EnableWindow(!bChecked);

		EnableAgeWindow(bChecked);
		EnableNoticeWindow(bChecked);
		break;

	case IDC_CHECK_DISP_LIVE_VIDEO:
		GetDlgItem(IDC_CHECK_DISP_AGE)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NOTICE) && !m_pStatus->GetDispState(DISP_AGE_NOTICE));
		GetDlgItem(IDC_CHECK_DISP_AGE_NOTICE)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_AGE) && !m_pStatus->GetDispState(DISP_NOTICE));
		GetDlgItem(IDC_CHECK_PREPARE_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NEWS));
		GetDlgItem(IDC_CHECK_DISP_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NEWS_SCROLL));

		EnableLiveVideoWindow(bChecked);
		break;

	case IDC_CHECK_DISP_NEWS_SCROLL:
		GetDlgItem(IDC_CHECK_DISP_AGE)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_NOTICE) && !m_pStatus->GetDispState(DISP_AGE_NOTICE));
		GetDlgItem(IDC_CHECK_DISP_AGE_NOTICE)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_AGE) && !m_pStatus->GetDispState(DISP_NOTICE));
		GetDlgItem(IDC_CHECK_PREPARE_NEWS_SCROLL)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_LIVE_VIDEO)->EnableWindow(!bChecked);

		GetDlgItem(IDC_CHECK_DISP_NEWS)->EnableWindow(!bChecked);

		EnableNewsScrollWindow(bChecked);
		EnableNewsWindow(bChecked);
		break;

	case IDC_CHECK_DISP_NEWS:
		GetDlgItem(IDC_CHECK_PREPARE_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_AGE) && !m_pStatus->GetDispState(DISP_AGE_NOTICE) && !m_pStatus->GetDispState(DISP_LIVE_VIDEO));
		GetDlgItem(IDC_CHECK_DISP_NEWS_SCROLL)->EnableWindow(!bChecked && !m_pStatus->GetDispState(DISP_AGE) && !m_pStatus->GetDispState(DISP_AGE_NOTICE) && !m_pStatus->GetDispState(DISP_LIVE_VIDEO));

		EnableNewsWindow(bChecked);
		break;

	case IDC_CHECK_DISP_WEATHER:
		EnableWeatherWindow(bChecked);
		break;

	case 0:
		GetDlgItem(IDC_CHECK_DISP_AGE)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_NOTICE)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_AGE_NOTICE)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_PREPARE_NEWS_SCROLL)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_NEWS_SCROLL)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_LIVE_VIDEO)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_NEWS)->EnableWindow(!bChecked);
		GetDlgItem(IDC_CHECK_DISP_WEATHER)->EnableWindow(!bChecked);

		EnableAgeWindow(bChecked);
		EnableNoticeWindow(bChecked);
		EnableLiveVideoWindow(bChecked);
		EnableWeatherWindow(bChecked);
		EnableNewsScrollWindow(bChecked);
		EnableNewsWindow(bChecked);
		CancelPrepareNewsScroll();
		break;
	}

	if(nID != 0) GetDlgItem(nID)->SetFocus();
}

BOOL CClock2026Dlg::DispAge(void)
{
	CalcAgeShow();
	return TRUE;
}

BOOL CClock2026Dlg::StartAge(BOOL bOnlyAge)
{
	CString strFpgFileName = _T("연령");

	if (!OpenFpg(strFpgFileName, AGE_PAGE_NUMBER, GetNoticeDir())) return FALSE;

	m_bUseAge = TRUE;

	m_timeAgeStart = CTime::GetCurrentTime();
	m_bPrevAgeShow = FALSE;
	m_bAgeFirstRun = TRUE;
	m_strPrevAgeTime = BLANK_STRING;

	CCGXObject* pAgeObjectFromNoticePage = nullptr;

	m_pAgeObject = m_pHDdaVinci->GetCGXObject(AgeObjects[FROM_E(AGE_OBJECTS::AGE)], AGE_PAGE_NUMBER);
 
	SetAgeImageFile(m_pAgeObject);

	if (bOnlyAge)
	{
		SetTimer(ID_TIMER_DISPLAY_AGE, 500, nullptr);
	}
	else
	{
		// 우상단과 같이 나와야 할때는 우상단 페이지에서 연령을 찾는다.
		pAgeObjectFromNoticePage = m_pHDdaVinci->GetCGXObject(AgeObjects[FROM_E(AGE_OBJECTS::AGE)], NOTICE_PAGE_NUMBER);

		if (pAgeObjectFromNoticePage != nullptr)
		{
			pAgeObjectFromNoticePage->GetPosition(&m_nAgePosX, &m_nAgePosY);
		}

		if (m_pAgeObject != nullptr)
		{
			m_pAgeObject->SetPositionByAbs(m_nAgePosX, m_nAgePosY);
		}
	}

	m_pHDdaVinci->SetPageHandler(this, CClock2026Dlg::HandlerForAge);

	m_pHDdaVinci->GetScene(AGE_PAGE_NUMBER)->Prepare(AGE_PAGE_NUMBER);
	m_pHDdaVinci->SetScene(AGE_PAGE_NUMBER, m_pHDdaVinci->GetScene(AGE_PAGE_NUMBER), 0);
	m_pHDdaVinci->TakeIn(AGE_PAGE_NUMBER);

	return TRUE;
}

void CClock2026Dlg::SetAgeImageFile(CCGXObject* pObject)
{
	if (pObject == nullptr) return;

	INT nAgeMode = m_pStatus->NoticeConf()->AgeMode();

	switch (nAgeMode)
	{
	    case AGE_ALL: m_strAgeFileName = BLANK_STRING; break;
		case AGE_7:	  m_strAgeFileName = GetImagesDir() + _T("7세.png"); break;
		case AGE_12:  m_strAgeFileName = GetImagesDir() + _T("12세.png"); break;
		case AGE_15:  m_strAgeFileName = GetImagesDir() + _T("15세.png"); break;
		case AGE_19:  m_strAgeFileName = GetImagesDir() + _T("19세.png"); break;

		default:
			m_strAgeFileName = BLANK_STRING;
		break;
	}

	pObject->Replace(m_strAgeFileName, 1);

}

void CClock2026Dlg::SetAgePos(CCGXObject* pObject)
{
	if (pObject != nullptr)
	{
		pObject->GetPosition(&m_nAgePosX,&m_nAgePosY);
	}
	else
	{
		m_nAgePosX = -19200;
		m_nAgePosY = -10800;
	}
}

void CClock2026Dlg::DrawAgeTime(CWnd* pWnd, CString strAgeTime)
{
	CClientDC dc(pWnd);
	CDC memDC;
	CBitmap bmp;

	CRect rect;
	pWnd->GetClientRect(&rect);
	
	memDC.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);
	CFont* pOldFont = memDC.SelectObject(m_pStatus->UIFonts()->GetFont(FONT_STATIC_TIMER));

	memDC.FillSolidRect(rect, _COLOR_DEFAULT_);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(_COLOR_BLACK_);

	memDC.DrawText(strAgeTime, &rect, (DT_CENTER | DT_VCENTER | DT_SINGLELINE));

//	memDC.TextOut(0, 0, _T("Hello, World!"));
		
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bmp.DeleteObject();

}

BOOL CClock2026Dlg::CalcAgeShow(void)
{
	if (!m_bUseAge)
	{
		m_bAgeShow = FALSE;
		m_bPrevAgeShow = FALSE;

		return FALSE;
	}

	CTimeSpan timeSpanAge = GETTIMESPAN(m_timeAgeStart, CTime::GetCurrentTime());

	INT nTotalSeconds = static_cast<INT>(timeSpanAge.GetTotalSeconds());

	switch (m_pStatus->NoticeConf()->AgeMode())
	{
	case AGE_ALL:
		if (m_bAgeFirstRun)
		{
			PrintLog(_T("전체 시청가능 연령고지 송출합니다. 전체 시청가능은 표시가 없습니다."));
		}
		m_bAgeShow = FALSE;
		break;

	case AGE_7:
	case AGE_12:
	case AGE_15:
		m_bAgeShow = (nTotalSeconds % 600) < 30;

		if(m_bAgeShow != m_bPrevAgeShow)
		{
			if (m_bAgeShow)
			{
				PrintLog( _T("타이머에 의한 연령고지 표시"));
			}
			else
			{
				PrintLog(_T("타이머에 의한 연령고지 휴지시간"));

				SetDlgItemText(IDC_STATIC_AGE_TIMER_STOP, CTime::GetCurrentTime().Format(_T("%H:%M:%S")));
			}
		}
		break;

	case AGE_19:
		if (m_bAgeFirstRun)
		{
			PrintLog(_T("연령고지(19세) 송출"));
		}
		m_bAgeShow = TRUE;
		break;

	default:
		break;
	}

	CString strAgeTime;

	strAgeTime.Format(_T("%02d:%02d"), (nTotalSeconds % 600) / 60, nTotalSeconds % 60);   // 10분단위 연령 시간 보기

	if (m_strPrevAgeTime != strAgeTime)
	{
		//SetDlgItemText(IDC_STATIC_AGE_TIMER, strAgeTime);
		m_strPrevAgeTime = strAgeTime;
		DrawAgeTime(GetDlgItem(IDC_STATIC_AGE_TIMER), strAgeTime); // 더블버퍼링이 이상하게 멈추는 현상이 있음
	}

	m_bAgeFirstRun = FALSE;
	m_bPrevAgeShow = m_bAgeShow;
	return m_bAgeShow;
}

BOOL CClock2026Dlg::SearchAndModifyFileName(CString& strFileName, CString strPath)
{
	CString strModifyFileName;

	if (!CFileUtils::ExistFile(strFileName))
	{
		strModifyFileName = strPath + CFileUtils::ExtractFileName(strFileName);

		if (!CFileUtils::ExistFile(strModifyFileName))
		{
			return FALSE;
		}
		else
		{
			strFileName = strModifyFileName;
		}
	}

	return TRUE;
}

void CClock2026Dlg::OnEnKillfocusEditNoticeRepetition()
{
	CString strBuffer;
	GetDlgItemText(IDC_EDIT_NOTICE_REPETITION, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->NoticeConf()->Repetition(min(max(GETINT(strBuffer), MIN_NOTICE_REPETITION), MAX_NOTICE_REPETITION));
	}
	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

BOOL CClock2026Dlg::StartDisplayNotice(BOOL bUseAge)
{
	m_bUseAge = bUseAge;

	m_nCurNoticeCount = 1;
	m_nCurNoticeIndex = m_pStatus->GetNoticeList()->GetCurIndex();

	if (!DisplayNotice(TRUE)) 
	{
		return FALSE;
	}

	SetTimer(ID_TIMER_DISPLAY_NOTICE, DURATION_AGE_TIMER, nullptr);

	return TRUE;
}

UINT CClock2026Dlg::DisplayNoticeThread(LPVOID pParam)
{
	CClock2026Dlg* pDlg = reinterpret_cast<CClock2026Dlg*>(pParam);

	while (pDlg->m_pStatus->GetDispState(DISP_AGE_NOTICE) || pDlg->m_pStatus->GetDispState(DISP_NOTICE))
	{
		pDlg->DisplayNotice(FALSE);
	}

	return 0;
}

BOOL CClock2026Dlg::DisplayNotice(BOOL bFirst)
{
	CNoticeDataList* pList = m_pStatus->GetNoticeList();

	if (m_bUseAge)
	{
		if (bFirst)
		{
			CString strFpg;
			if (!pList->HasNoAgeNotice(m_pHDdaVinci, strFpg))
			{
				PrintLog(strFpg + CString(_T("연령 알리아스가 없습니다.")), _T("오류"), TRUE);
				return FALSE;
			}
		}

		CalcAgeShow();
	}

	if (m_pHDdaVinci->IsTransmitting(NOTICE_PAGE_NUMBER)) return FALSE;

	int nDataCount = pList->Size();
	BOOL bUseExist = pList->HasAvailableData();

	if (!bUseExist || nDataCount == 0)
	{
		TakeOutNotice();
		return FALSE;
	}

	auto noticeData = pList->GetAt(m_nCurNoticeIndex);

	while (!noticeData.IsAvailable())
	{
		m_nCurNoticeIndex++;

		if (nDataCount <= m_nCurNoticeIndex)
		{
			m_nCurNoticeIndex = 0;
			if (m_pStatus->NoticeConf()->Repetition() == m_nCurNoticeCount)
			{
				SendMessage(WM_NOTICE_FINISH, 0, 0);
				TakeOutNotice();
				return TRUE;
			}			

			SetDlgItemText(IDC_STATIC_NOTICE_COUNTER, GETSTR(++m_nCurNoticeCount));
		}

		noticeData = pList->GetAt(m_nCurNoticeIndex);
	}

	if(!TakeInNotice(m_nCurNoticeIndex))
	{
		return FALSE;
	}

	if (bFirst && m_bUseAge)
	{
		StartAge(FALSE);
	}

	m_nCurNoticeIndex++;

	return TRUE;
}

//======== Notice CCS 5-type classification (ported from Notice2018, TakeInNotice only) ========
enum class ECCSRelation { BEFORE, STRADDLE_BEGIN, INSIDE, STRADDLE_END, AFTER };

static ECCSRelation ClassifyCCSRelation(
	int effBegin, int effEnd, int ccsBegin, int ccsEnd)
{
	if (effEnd   <= ccsBegin) return ECCSRelation::BEFORE;
	if (effBegin >= ccsEnd)   return ECCSRelation::AFTER;
	if (effBegin <  ccsBegin) return ECCSRelation::STRADDLE_BEGIN;
	if (effEnd   >  ccsEnd)   return ECCSRelation::STRADDLE_END;
	return ECCSRelation::INSIDE;
}

static void AdjustEffectByCCSRelation(
	CCGXEffect* pEff,
	int oldCcsBegin, int oldCcsEnd,
	int newCcsBegin, int newCcsEnd)
{
	int effBegin = pEff->GetBeginFrame();
	int effEnd   = pEff->GetEndFrame();
	int effSize  = pEff->GetFrames();
	int oldCcsSz = oldCcsEnd - oldCcsBegin;
	int newCcsSz = newCcsEnd - newCcsBegin;
	int delta    = newCcsSz - oldCcsSz;

	ECCSRelation rel = ClassifyCCSRelation(effBegin, effEnd, oldCcsBegin, oldCcsEnd);

	switch (rel)
	{
	case ECCSRelation::BEFORE:
		break;

	case ECCSRelation::STRADDLE_BEGIN:
		if (newCcsEnd < effEnd)
			pEff->Resize(newCcsEnd - effBegin);
		break;

	case ECCSRelation::INSIDE:
		if (newCcsSz < effSize)
		{
			pEff->MoveTo(newCcsBegin);
			pEff->Resize(newCcsSz);
		}
		else
		{
			int offsetFromBegin = effBegin - oldCcsBegin;
			pEff->MoveTo(newCcsBegin + offsetFromBegin);
		}
		break;

	case ECCSRelation::STRADDLE_END:
		{
			// Always keep End-relative position (== Move(delta)); preserves original
			// overlap, never grows coverage. Removed the B=CCS.Begin shrink branch
			// that pulled the effect over the CCS (covering bug). delta>0 path unchanged.
			int offsetFromEnd = effBegin - oldCcsEnd;
			pEff->MoveTo(newCcsEnd + offsetFromEnd);
		}
		break;

	case ECCSRelation::AFTER:
		pEff->Move(delta);
		break;
	}
}
BOOL CClock2026Dlg::TakeInNotice(INT nIndex)
{
	CNoticeData noticeData = m_pStatus->GetNoticeList()->GetAt(nIndex);

	CString strTemplateFileName = noticeData.GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

	if (!SearchAndModifyFileName(strTemplateFileName, GetNoticeDir()))
	{
		PrintLog(GETSTR(nIndex + 1) + _T("번 리스트 fpg 파일이 존재하지 않습니다."));
		return FALSE;
	}

	LARGE_INTEGER ccsQpcFreq = {}, ccsTOpen0 = {}, ccsTOpen1 = {}, ccsTTake0 = {}, ccsTTake1 = {};
	QueryPerformanceFrequency(&ccsQpcFreq);
	QueryPerformanceCounter(&ccsTOpen0);
	m_pHDdaVinci->Open(strTemplateFileName, 1, NOTICE_PAGE_NUMBER);
	QueryPerformanceCounter(&ccsTOpen1);

	CCGXObject* pObject = m_pHDdaVinci->GetCGXObject(_T("연령"), NOTICE_PAGE_NUMBER);

	if (m_bUseAge)
	{
		if (pObject != nullptr)
		{
			pObject->SetShow(FALSE);
			pObject->GetPosition(&m_nAgePosX, &m_nAgePosY);
		}
		else
		{
			PrintLog(_T("연령이 반드시 있어야 합니다."), _T("경고"), TRUE);
			return FALSE;
		}
	}
	else
	{
		if (pObject != nullptr)
		{
			pObject->SetShow(FALSE);
		}
	}

	CStringList* pLayerAliasList = new CStringList;

	m_pHDdaVinci->GetLayerAlias(pLayerAliasList, NOTICE_PAGE_NUMBER);

	for (POSITION pos = pLayerAliasList->GetHeadPosition(); pos != nullptr;)
	{
		CString strLayerAlias = pLayerAliasList->GetAt(pos);

		CCGXLayer* pLayer = m_pHDdaVinci->GetCGXLayer(strLayerAlias, NOTICE_PAGE_NUMBER);

		pLayer->SetShow(strLayerAlias.Find(_T("MaskLayer")) == -1);  // 마스크레이어는 안보이게

		pLayerAliasList->GetNext(pos);
	}

	delete pLayerAliasList;

	for (INT i = 0; i < MAX_ALIAS_NUM; i++)
	{
		pObject = m_pHDdaVinci->GetCGXObject(noticeData.GetValue(ALIAS_1 + i), NOTICE_PAGE_NUMBER);

		if (pObject != nullptr)
		{
			if (pObject->GetType() != CG_CMD_OBJECT_ANIMATION)
			{
				m_pHDdaVinci->ReplaceObject(noticeData.GetValue(ALIAS_1 + i), noticeData.GetValue(VALUE_1 + i), 1, NOTICE_PAGE_NUMBER);
			}
			else
			{
				// CCS variable-length handling: after Replace, shift surrounding
				// effect timelines by delta using 5-type classification (Notice2018 logic).
				CString strCCSAlias = noticeData.GetValue(ALIAS_1 + i);
				CString strCCSValue = noticeData.GetValue(VALUE_1 + i);

				CString strLayerAlias = m_pHDdaVinci->GetObjectParentLayerAlias(strCCSAlias, NOTICE_PAGE_NUMBER);
				CCGXEffect* pLayerEff = m_pHDdaVinci->GetLayerOwnEffect(strLayerAlias, NOTICE_PAGE_NUMBER);
				CCGXEffect* pObjEff   = m_pHDdaVinci->GetObjectOwnEffect(strCCSAlias, NOTICE_PAGE_NUMBER);

				int oldCcsBegin = 0, oldCcsEnd = 0;
				if (pLayerEff)
				{
					oldCcsBegin = pLayerEff->GetBeginFrame();
					oldCcsEnd   = pLayerEff->GetEndFrame();
				}
				else if (pObjEff)
				{
					oldCcsBegin = pObjEff->GetBeginFrame();
					oldCcsEnd   = pObjEff->GetEndFrame();
				}

				int oldLen = pObject->GetCCSLength();
				pObject->Replace(strCCSValue, 0);
				int newLen = pObject->GetCCSLength();
				int delta  = newLen - oldLen;

				if (delta != 0)
				{
					int newCcsBegin = oldCcsBegin;
					int newCcsEnd   = oldCcsEnd + delta;

					CPtrList allEffects;
					m_pHDdaVinci->GetEffectList(&allEffects, NOTICE_PAGE_NUMBER);
					for (POSITION epos = allEffects.GetHeadPosition(); epos != NULL;)
					{
						CCGXEffect* pEff = (CCGXEffect*)allEffects.GetNext(epos);
						if (pEff == nullptr || pEff == pLayerEff || pEff == pObjEff)
							continue;
						AdjustEffectByCCSRelation(
							pEff, oldCcsBegin, oldCcsEnd, newCcsBegin, newCcsEnd);
					}

					if (pLayerEff) pLayerEff->Resize(pLayerEff->GetFrames() + delta);
					if (pObjEff)   pObjEff->Resize(pObjEff->GetFrames() + delta);
				}
			}
		}
	}

	//	_MEASURE_START_(3)// 스프레드에 색칠하는 자체가 시간이 너무 길다. 테스트는 해보는데 안되면 포기
		m_SpreadNotice.DrawSelection(m_nCurNoticeIndex + 1);
	//	SpreadLButtonClicked(IDC_SPREAD_NOTICE, NOTICE_FIELD_INDEX::ANNOTATION + 1, m_nCurNoticeIndex);
	//	_MEASURE_END_(3, _T("Notice DrawSelection"))


#ifdef _TAKE_IN_LOCK_ 
	std::lock_guard<std::mutex> lock(m_mutexHandler);
#endif

	QueryPerformanceCounter(&ccsTTake0);
	m_pHDdaVinci->TakeInEx(NOTICE_PAGE_NUMBER);
	QueryPerformanceCounter(&ccsTTake1);

	{
		double ccsMsOpen   = (double)(ccsTOpen1.QuadPart - ccsTOpen0.QuadPart) * 1000.0 / (double)ccsQpcFreq.QuadPart;
		double ccsMsPrep   = (double)(ccsTTake0.QuadPart - ccsTOpen1.QuadPart) * 1000.0 / (double)ccsQpcFreq.QuadPart;
		double ccsMsTakein = (double)(ccsTTake1.QuadPart - ccsTTake0.QuadPart) * 1000.0 / (double)ccsQpcFreq.QuadPart;
		double ccsMsWindow = (double)(ccsTTake1.QuadPart - ccsTOpen0.QuadPart) * 1000.0 / (double)ccsQpcFreq.QuadPart;
		CString s;
		s.Format(_T("[CCS_T] open=%.2f prep=%.2f takein=%.2f window(open..onair)=%.2f ms  OVER30=%s"),
			ccsMsOpen, ccsMsPrep, ccsMsTakein, ccsMsWindow, (ccsMsWindow > 30.0 ? _T("YES") : _T("no")));
		PrintLog(s);
	}

	return TRUE;

}

BOOL CClock2026Dlg::TakeOutNotice()
{
	if (!m_pStatus->GetDispState(DISP_AGE_NOTICE) && !m_pStatus->GetDispState(DISP_NOTICE)) return FALSE;

	KillTimer(ID_TIMER_DISPLAY_NOTICE);

	if (m_pStatus->GetDispState(DISP_AGE_NOTICE))
	{
		TakeOutAge();
		m_pStatus->SetDispStateForPage(DISP_AGE_NOTICE, FALSE, NOTICE_PAGE_NUMBER);
		PrintLog(_T("연령+우상단 송출 아웃합니다."));

		EnableWindow(IDC_CHECK_DISP_AGE_NOTICE, FALSE);
	}

	if (m_pStatus->GetDispState(DISP_NOTICE))
	{
		m_pStatus->SetDispStateForPage(DISP_NOTICE, FALSE, NOTICE_PAGE_NUMBER);
		PrintLog(_T("우상단 송출 아웃합니다."));

		EnableWindow(IDC_CHECK_DISP_NOTICE, FALSE);
	}

	m_pHDdaVinci->TakeOut(NOTICE_PAGE_NUMBER);
#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(NOTICE_PAGE_NUMBER);
#endif	

	UpdateData(FALSE);

	return TRUE;
}


BOOL CClock2026Dlg::TakeOutAge(void)
{
	if (!m_pStatus->GetDispState(DISP_AGE) && !m_pStatus->GetDispState(DISP_AGE_NOTICE)) return FALSE;

	KillTimer(ID_TIMER_DISPLAY_AGE);

	if (m_pStatus->GetDispState(DISP_AGE))
	{
		m_pStatus->SetDispStateForPage(DISP_AGE, FALSE, AGE_PAGE_NUMBER);
	}

	if (m_pStatus->GetDispState(DISP_AGE_NOTICE))
	{
		m_pStatus->SetDispStateForPage(DISP_AGE_NOTICE, FALSE, AGE_PAGE_NUMBER);
	}

	m_pHDdaVinci->SetPageHandler(this, nullptr);
	
	m_pHDdaVinci->TakeOut(AGE_PAGE_NUMBER);

#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(AGE_PAGE_NUMBER);
#endif

	m_bAgeFirstRun = TRUE;
	m_bUseAge = FALSE;
	m_bPrevAgeShow = FALSE;
	m_strPrevAgeTime = BLANK_STRING;

	PrintLog(_T("연령 고지 송출 아웃합니다."));

	CString strStopTime = CTime::GetCurrentTime().Format(_T("%H:%M:%S"));
	
	SetDlgItemText(IDC_STATIC_AGE_TIMER_STOP, strStopTime);

	EnableWindow(IDC_CHECK_DISP_AGE, FALSE);

	UpdateData(FALSE);

	return TRUE;
}

void CClock2026Dlg::OnBnClickedCheckDispAge()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_AGE);

	if (bDisp)
	{
		CancelPrepareNewsScroll();
		bDisp = StartAge(TRUE);
		m_pStatus->SetDispStateForPage(DISP_AGE, bDisp, AGE_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("연령 송출을 시작합니다."), _T("알림"), TRUE);
		}
		else
		{
			PrintLog(_T("연령 송출에 실패했습니다."), _T("알림"), TRUE);
		}

		EnableWindow(IDC_CHECK_DISP_AGE, bDisp);

		UpdateData(FALSE);

	}
	else
	{
		TakeOutAge();
	}

}

void CClock2026Dlg::OnBnClickedCheckDispNotice()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_NOTICE);

	if (bDisp)
	{
		bDisp = StartDisplayNotice(FALSE);

		m_pStatus->SetDispStateForPage(DISP_NOTICE, bDisp, NOTICE_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("우상단 송출을 시작합니다."));
		}
		else
		{
			PrintLog(_T("우상단 송출에 실패했습니다."));
		}

		EnableWindow(IDC_CHECK_DISP_NOTICE, bDisp);

		UpdateData(FALSE);

	}
	else
	{
		TakeOutNotice();
	}

}

void CClock2026Dlg::OnBnClickedCheckDispAgeNotice()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_AGE_NOTICE);

	if (bDisp)
	{
		PrintLog(_T("연령 송출을 시작합니다."), _T("알림"), TRUE);

		CancelPrepareNewsScroll();
		bDisp = StartDisplayNotice(TRUE);

		m_pStatus->SetDispStateForPage(DISP_AGE_NOTICE, bDisp, NOTICE_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("연령+우상단 송출을 시작합니다."));
		}
		else
		{
			PrintLog(_T("연령+우상단 송출에 실패했습니다."));
		}

		EnableWindow(IDC_CHECK_DISP_AGE_NOTICE, bDisp);

		UpdateData(FALSE);
	}
	else
	{
		TakeOutNotice();
	}

}

void CClock2026Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	WriteStatusBar_MousePos(point);

	CDialogEx::OnMouseMove(nFlags, point);
}

void CClock2026Dlg::OnBnClickedButtonNoticeMoveUp()
{
	CNoticeDataList* pList = m_pStatus->GetNoticeList();

	if (pList == nullptr) return;

	int nIndex = pList->GetCurIndex();

	pList->MoveUp(nIndex);

	m_SpreadNotice.UpdateCurrentSheet(FALSE);
	m_SpreadNotice.DrawSelectionForCurIndex();
}

void CClock2026Dlg::OnBnClickedButtonNoticeMoveDown()
{
	CNoticeDataList* pList = m_pStatus->GetNoticeList();

	if (pList == nullptr) return;

	int nIndex = pList->GetCurIndex();

	pList->MoveDown(nIndex);

	m_SpreadNotice.UpdateCurrentSheet(FALSE);
	m_SpreadNotice.DrawSelectionForCurIndex();
}

void CClock2026Dlg::DrawTextWithDoubleBuffering(CWnd* pWnd, CString strText)
{
	CClientDC dc(pWnd);
	CDC memDC;
	CBitmap bmp;

	CRect rect;
	pWnd->GetClientRect(&rect);

	memDC.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);
//	CFont* pOldFont = memDC.SelectObject(m_pStatus->UIFonts()->GetFont(FONT_STATIC_TIMER));

	memDC.FillSolidRect(rect, _COLOR_DEFAULT_);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(_COLOR_BLACK_);

	memDC.DrawText(strText, &rect, (DT_CENTER | DT_VCENTER | DT_SINGLELINE));

	//	memDC.TextOut(0, 0, _T("Hello, World!"));

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
//	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bmp.DeleteObject();

}

void CClock2026Dlg::SelectChangeManualUpTemplate(void)
{
	CString strFpgFileName;

	int n = m_pComboManualUpTemplate->GetCurSel();
	m_pComboManualUpTemplate->GetLBText(n, strFpgFileName);

	m_pStatus->ManualUpConf()->ManualUpTemplate(strFpgFileName);

	if (m_pStatus->GetDispState(DISP_MANUALUP))
	{
		BOOL bDisp = DispManualUp();

		m_pStatus->SetDispStateForPage(DISP_MANUALUP, bDisp, MANUALUP_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("좌상단 송출했습니다."));
		}
		else
		{
			PrintLog(_T("좌상단 송출 실패했습니다."));
		}
	}

	UpdateData(FALSE);

}

void CClock2026Dlg::OnCbnSelchangeComboManualupTemplates()
{
	SelectChangeManualUpTemplate();
}

void CClock2026Dlg::ReloadManualUpTemplate(void)
{
	m_pComboManualUpTemplate->ResetContent();

	CFileFind FileFinder;

	// start working for files
	BOOL bWorking = FileFinder.FindFile(GetManualUpDir() + _T("*.fpg"));

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();

		if (!FileFinder.IsDirectory())
		{
			CString str = CFileUtils::ExtractFileName(FileFinder.GetFilePath());

			m_pComboManualUpTemplate->AddString(str);
		}

	}
	FileFinder.Close();
}

void CClock2026Dlg::OnBnClickedButtonReloadManualupTemplate()
{
	ReloadManualUpTemplate();
}

BOOL CClock2026Dlg::PrevManualUp(void)
{
	CString strFpgFileName;

	GetDlgItemText(IDC_COMBO_MANUALUP_TEMPLATES, strFpgFileName);

	INT nIndex =  m_pComboManualUpTemplate->FindString(-1,strFpgFileName);

	if (nIndex < 1) return FALSE;

	nIndex--;

	m_pComboManualUpTemplate->SetCurSel(nIndex);

	GetDlgItemText(IDC_COMBO_MANUALUP_TEMPLATES, strFpgFileName);

	m_pStatus->ManualUpConf()->ManualUpTemplate(strFpgFileName);

	if (m_pStatus->GetDispState(DISP_MANUALUP))
	{
		BOOL bDisp = DispManualUp();

		m_pStatus->SetDispStateForPage(DISP_MANUALUP, bDisp, MANUALUP_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("수동 상단 송출했습니다."));
		}
		else
		{
			PrintLog(_T("수동 상단 송출 실패했습니다."));
		}
	}


	return TRUE;
}

BOOL CClock2026Dlg::NextManualUp(void)
{
	CString strFpgFileName;

	GetDlgItemText(IDC_COMBO_MANUALUP_TEMPLATES, strFpgFileName);

	INT nIndex = m_pComboManualUpTemplate->FindString(-1, strFpgFileName);

	INT nItems = m_pComboManualUpTemplate->GetCount();

	if (nIndex >= nItems - 1) return FALSE;

	nIndex++;

	m_pComboManualUpTemplate->SetCurSel(nIndex);

	GetDlgItemText(IDC_COMBO_MANUALUP_TEMPLATES, strFpgFileName);

	m_pStatus->ManualUpConf()->ManualUpTemplate(strFpgFileName);

	if (m_pStatus->GetDispState(DISP_MANUALUP))
	{
		BOOL bDisp = DispManualUp();

		m_pStatus->SetDispStateForPage(DISP_MANUALUP, bDisp, MANUALUP_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("수동 상단 송출했습니다."));
		}
		else
		{
			PrintLog(_T("수동 상단 송출 실패했습니다."));
		};
	}

	return TRUE;
}

BOOL CClock2026Dlg::DispManualUp(void)
{
	m_pHDdaVinci->TakeOut(MANUALUP_PAGE_NUMBER);

	m_pHDdaVinci->WaitForTransmitting(MANUALUP_PAGE_NUMBER);

	CString strFpgFileName = m_pStatus->ManualUpConf()->ManualUpTemplate().Trim();

	if (!OpenFpg(strFpgFileName,MANUALUP_PAGE_NUMBER,GetManualUpDir()))
	{
		return FALSE;
	}

	m_pHDdaVinci->TakeInEx(MANUALUP_PAGE_NUMBER);


	return TRUE;
}

BOOL CClock2026Dlg::TakeOutManualUp(void)
{

	m_pStatus->SetDispStateForPage(DISP_MANUALUP, FALSE, MANUALUP_PAGE_NUMBER);
	
	m_pHDdaVinci->TakeOut(MANUALUP_PAGE_NUMBER);
#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(MANUALUP_PAGE_NUMBER);
#endif
	PrintLog(_T("좌상단 송출 아웃합니다."));

	EnableManualUpWindow();

	UpdateData(FALSE);

	return TRUE;
	
}

void CClock2026Dlg::OnBnClickedCheckDispManualUp()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_MANUALUP);

	if (bDisp)
	{
		CancelPrepareNewsScroll();
		bDisp = DispManualUp();
		m_pStatus->SetDispStateForPage(DISP_MANUALUP, bDisp, MANUALUP_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("수동상단 송출했습니다."));
		}
		else
		{
			PrintLog(_T("수동상단 송출 실패했습니다."));
		}

		EnableManualUpWindow();

		UpdateData(FALSE);
	}
	else
	{
		TakeOutManualUp();
	}
}


void CClock2026Dlg::OnBnClickedButtonManualupPrev()
{
	PrevManualUp();
}

void CClock2026Dlg::OnBnClickedButtonManualupNext()
{
	NextManualUp();
}

void CClock2026Dlg::OnBnClickedCancel()
{
	int nResult = AfxMessageBox(_T("MPT 2025 프로그램을 종료하시겠습니까 ? "), MB_YESNO);

	if (nResult == IDNO)
	{
		return;
	}

	BOOL bExist = CFileUtils::ExistFile(m_pStatus->m_strFileName);

	if (m_bFileChange || !bExist)
	{
		CString strMsg;

		strMsg.Format(_T("%s 파일을 저장하시겠습니까?"), CFileUtils::ExtractFileName(m_pStatus->m_strFileName));

		if (IDYES == MessageBox(strMsg, _T("저장하기"), MB_YESNO | MB_ICONQUESTION))
		{
			if (!bExist)
			{
				SaveAsFile(m_pStatus->m_strFileName);
			}
			else
			{
				SaveFile(m_pStatus->m_strFileName);
			}
		}
	}

	CDialogEx::OnCancel();
}

INT CClock2026Dlg::LoadFromFPG(CString strFileName, int nPage)
{
	CString strProgress;

	m_pHDdaVinci->ProjectOpen(strFileName);

	INT nPageCount = m_pHDdaVinci->ProjectGetCount();

	CProgressWnd* pWndProgress = new CProgressWnd;
	pWndProgress->Create(this, _T("FPG File Exporting..."), TRUE);
	pWndProgress->SetRange(0, nPageCount);
	pWndProgress->SetPos(0);
	pWndProgress->ShowWindow(SW_SHOW);
	pWndProgress->SetActiveWindow();
	
	std::vector<CNoticeData> vObjects;

	INT nStartIndex = m_pStatus->GetNoticeList()->Size();

	for (int i = 0; i < nPageCount; i++)
	{
		CString strFpgFileName = CFileUtils::ExtractOnlyFileName(strFileName) + _T("-") + GETSTR(i);

		m_pHDdaVinci->OpenPage(i, 1, nPage);

		m_pHDdaVinci->Export(GetNoticeDir().TrimRight(_T('/')), strFpgFileName, nPage);

		CNoticeData noticeData;

		noticeData.SetValue(NOTICE_FIELD_INDEX::USE, TRUE_STRING);

		noticeData.SetValue(NOTICE_FIELD_INDEX::TEMPLATE, GetNoticeDir() + strFpgFileName + FPG_EXT);

		noticeData.GetAliases(m_pHDdaVinci);

		vObjects.emplace_back(noticeData);

		pWndProgress->SetPos(i);
		strProgress.Format(_T("FPG File Exporting... [%d%%]"), i * 100 / nPageCount);
		pWndProgress->SetText(_T("FPG File Load : ") + strFpgFileName);
		pWndProgress->SetWindowText(strProgress);
	}

	m_pStatus->GetNoticeList()->AddRecords(vObjects);
	m_SpreadNotice.UpdateCurrentSheet(FALSE);

	pWndProgress->ShowWindow(SW_HIDE);
	pWndProgress->DestroyWindow();
	SAFE_DELETE(pWndProgress);
	SetActiveWindow();

	return nStartIndex;
}

void CClock2026Dlg::OnBnClickedButtonLoadFpr()
{
	CFileDialog dlg(TRUE, _T("fpr"), m_pStatus->m_strFPRFileName);
	dlg.m_ofn.lpstrDefExt = _T("fpr");
	dlg.m_ofn.lpstrFilter = _T("Project Files (*.fpr)\0*.fpr\0All Formats (*.*)\0*.*\0\0");

	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		if (!CFileUtils::ExistFile(dlg.GetPathName()))
		{
			PrintLog(dlg.GetFileName() + _T("파일을 읽을 수 없습니다."));
			UpdateData(FALSE);
			return;
		}

		m_pStatus->m_strFPRFileName = dlg.GetPathName();
		m_nCurNoticeIndex = LoadFromFPG(m_pStatus->m_strFPRFileName, NOTICE_PAGE_NUMBER);
		
		SS_CELLCOORD ss_coord;

		ss_coord.Col = NOTICE_FIELD_INDEX::TEMPLATE + 1;
		ss_coord.Row = m_nCurNoticeIndex;

		OnSpreadLButtonClicked(IDC_SPREAD_NOTICE, (LPARAM)(&ss_coord));
		
		SetChangeMark(TRUE);
		UpdateData(FALSE);
	}
}

void CClock2026Dlg::GetOrigPosLiveVideoObjects(void)
{
	SLiveVideoPos& origPos = m_pStatus->LiveVideoConf()->OrigPos();

	m_pHDdaVinci->ObjectGetPosition(
		LiveVideoObjects[FROM_E(LIVE_VIDEO_OBJECTS::EXPRESSION_AREA)],
		&(origPos.m_ptLiveVideo.X),
		&(origPos.m_ptLiveVideo.Y),
		LIVE_VIDEO_PAGE_NUMBER);

	m_pHDdaVinci->ObjectGetPosition(
		LiveVideoObjects[FROM_E(LIVE_VIDEO_OBJECTS::MASK)],
		&(origPos.m_ptMask.X),
		&(origPos.m_ptMask.Y),
		LIVE_VIDEO_PAGE_NUMBER);
	
	//PrintLog(GETSTR(origPos.m_ptLiveVideo.X) + _T(":") + GETSTR(origPos.m_ptLiveVideo.Y));
}

void CClock2026Dlg::SetPosLiveVideoObjects(void)
{

	SLiveVideoPos origPos = m_pStatus->LiveVideoConf()->OrigPos();
	Point ptOffset = m_pStatus->LiveVideoConf()->Offset();

     m_pInputLiveVideoObject->SetPositionByAbs(
		origPos.m_ptLiveVideo.X + ptOffset.X,
		origPos.m_ptLiveVideo.Y + ptOffset.Y);

	m_pInputLiveVideoMaskObject->SetPositionByAbs(
		origPos.m_ptMask.X + ptOffset.X,
		origPos.m_ptMask.Y + ptOffset.Y);
}

BOOL CClock2026Dlg::DispLiveVideo()
{
	if (m_pStatus->GetDispState(DISP_LIVE_VIDEO))
	{
		return FALSE;
	}

	m_pLiveVideo->StartQueuing();

	CCGXObject* pInputVideo = nullptr;

	CString strFpgFileName = m_pStatus->LiveVideoConf()->Template();

	if (!OpenFpg(strFpgFileName, LIVE_VIDEO_PAGE_NUMBER, GetLiveVideoDir()))
	{
		return FALSE;
	}
	
	m_pInputLiveVideoLayer = m_pHDdaVinci->GetCGXLayer(LiveVideoLayers[FROM_E(LIVE_VIDEO_LAYERS::LIVE_INPUT_VIDEO)], LIVE_VIDEO_PAGE_NUMBER);
	m_pInputLiveVideoObject = m_pHDdaVinci->GetCGXObject(LiveVideoObjects[FROM_E(LIVE_VIDEO_OBJECTS::EXPRESSION_AREA)], LIVE_VIDEO_PAGE_NUMBER);
	m_pInputLiveVideoMaskObject = m_pHDdaVinci->GetCGXObject(LiveVideoObjects[FROM_E(LIVE_VIDEO_OBJECTS::MASK)], LIVE_VIDEO_PAGE_NUMBER);

	if (m_pInputLiveVideoObject == nullptr)
	{
		PrintLog(strFpgFileName + _T("에 표출영역 알리아스가 없습니다"), _T("경고"), TRUE);
		return FALSE;
	}

	GetOrigPosLiveVideoObjects();

	if (m_pInputLiveVideoMaskObject != nullptr)
	{
		m_pInputLiveVideoMaskObject->SetShow(FALSE);
	}

	m_pHDdaVinci->GetScene(LIVE_VIDEO_PAGE_NUMBER)->Prepare(LIVE_VIDEO_PAGE_NUMBER);
	m_pHDdaVinci->SetScene(LIVE_VIDEO_PAGE_NUMBER, m_pHDdaVinci->GetScene(LIVE_VIDEO_PAGE_NUMBER), 0);

	m_pHDdaVinci->SetPageHandler(this, CClock2026Dlg::HandlerForInputVideo);

	Sleep(100); // 3프레임정도 건너뛰게 만들어야 번쩍임이 없음

	m_pHDdaVinci->TakeIn(LIVE_VIDEO_PAGE_NUMBER);

	return TRUE;
}

BOOL CClock2026Dlg::TakeOutLiveVideo(void)
{
	m_pStatus->SetDispStateForPage(DISP_LIVE_VIDEO, FALSE, LIVE_VIDEO_PAGE_NUMBER);

	m_pLiveVideo->StopQueuing();

	m_pHDdaVinci->SetPageHandler(this, nullptr);

	m_pHDdaVinci->TakeOut(LIVE_VIDEO_PAGE_NUMBER);
#ifdef _THREAD_TRANS_MODE_
	m_pHDdaVinci->WaitForTransmitting(LIVE_VIDEO_PAGE_NUMBER);
#endif
	PrintLog(_T("Live Video를 송출 아웃합니다."));

	EnableWindow(IDC_CHECK_DISP_LIVE_VIDEO, FALSE);

	UpdateData(FALSE);

	return TRUE;
}

void CClock2026Dlg::OnBnClickedCheckDispLiveVideo()
{
	BOOL bDisp = IsDlgButtonChecked(IDC_CHECK_DISP_LIVE_VIDEO);

	if (bDisp)
	{
		bDisp = DispLiveVideo();
		m_pStatus->SetDispStateForPage(DISP_LIVE_VIDEO, bDisp, LIVE_VIDEO_PAGE_NUMBER);

		if (bDisp)
		{
			PrintLog(_T("Live Video를 송출합니다."), _T("알림"), TRUE);
		}
		else
		{
			PrintLog(_T("Live Video 송출에 실패했습니다."));
		}

		EnableWindow(IDC_CHECK_DISP_LIVE_VIDEO, bDisp);


		UpdateData(FALSE);
	}
	else
	{
		TakeOutLiveVideo();
	}

}


void CClock2026Dlg::OnBnClickedCheckLiveVideoMoveEnable()
{
	m_pStatus->LiveVideoConf()->MoveEnable(IsDlgButtonChecked(IDC_CHECK_LIVE_VIDEO_MOVE_ENABLE));

	UpdateData(FALSE);
}

void CClock2026Dlg::LiveVideoResetPos()
{
	Point ptOffset(0, 0);

	m_pStatus->LiveVideoConf()->Offset(ptOffset);

	UpdateData(FALSE);
}


void CClock2026Dlg::LiveVideoMoveUp()
{
	if (m_pStatus->LiveVideoConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->LiveVideoConf()->Offset();
		ptOffset.Y -= m_pStatus->LiveVideoConf()->MovingSize();
		m_pStatus->LiveVideoConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::LiveVideoMoveLeft()
{
	if (m_pStatus->LiveVideoConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->LiveVideoConf()->Offset();
		ptOffset.X -= m_pStatus->LiveVideoConf()->MovingSize();
		m_pStatus->LiveVideoConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::LiveVideoMoveDown()
{
	if (m_pStatus->LiveVideoConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->LiveVideoConf()->Offset();
		ptOffset.Y += m_pStatus->LiveVideoConf()->MovingSize();
		m_pStatus->LiveVideoConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::LiveVideoMoveRight()
{
	if (m_pStatus->LiveVideoConf()->MoveEnable())
	{
		Point ptOffset = m_pStatus->LiveVideoConf()->Offset();
		ptOffset.X += m_pStatus->LiveVideoConf()->MovingSize();
		m_pStatus->LiveVideoConf()->Offset(ptOffset);
	}
}

void CClock2026Dlg::LiveVideoMove(DIRECTION dir)
{
	if (!m_pStatus->LiveVideoConf()->MoveEnable()) return;

	switch (dir)
	{
	case DIRECTION::UP:
		LiveVideoMoveUp();
		break;
	case DIRECTION::LEFT:
		LiveVideoMoveLeft();
		break;
	case DIRECTION::DOWN:
		LiveVideoMoveDown();
		break;
	case DIRECTION::RIGHT:
		LiveVideoMoveRight();
		break;
	default:
		break;
	}
//	SendMessage(WM_DISP_CHECK, DISP_LIVE_VIDEO, FROM_E<GET_POS>(GET_POS::NO));

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoResetPos()
{
	LiveVideoResetPos();
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoUp()
{
	LiveVideoMove(DIRECTION::UP);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoLeft()
{
	LiveVideoMove(DIRECTION::LEFT);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoDown()
{
	LiveVideoMove(DIRECTION::DOWN);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoRight()
{
	LiveVideoMove(DIRECTION::RIGHT);
}

void CClock2026Dlg::OnEnKillfocusEditLiveVideoMoveSize()
{
	CString strBuffer;

	GetDlgItemText(IDC_EDIT_LIVE_VIDEO_MOVE_SIZE, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->LiveVideoConf()->MovingSize(GETINT(strBuffer));
		SetChangeMark(TRUE);
	}

	SetDlgItemText(IDC_EDIT_LIVE_VIDEO_MOVE_SIZE, GETSTR(m_pStatus->LiveVideoConf()->MovingSize()));
}

void CClock2026Dlg::LiveVideoSnapSave(INT nSnapNumber)
{
	CString strTitle = m_pStatus->LiveVideoSnap(nSnapNumber).Title();
	CInputDlg dlg(strTitle);

	if (dlg.DoModal() == IDOK)
	{
		m_pStatus->SaveLiveVideoSnap(nSnapNumber);

		strTitle = dlg.GetData();
		m_pStatus->LiveVideoSnap(nSnapNumber).Title(strTitle);
		m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LIVE_VIDEO_SNAP_LOAD_1+ nSnapNumber), strTitle);
	}
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoSnapSave1()
{
	LiveVideoSnapSave(0);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoSnapSave2()
{
	LiveVideoSnapSave(1);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoSnapSave3()
{
	LiveVideoSnapSave(2);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoSnapLoad1()
{
	m_pStatus->LoadLiveVideoSnap(0);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoSnapLoad2()
{
	m_pStatus->LoadLiveVideoSnap(1);

	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonLiveVideoSnapLoad3()
{
	m_pStatus->LoadLiveVideoSnap(2);

	UpdateData(FALSE);
}

void CClock2026Dlg::SelectChangeLiveVideoTemplate(void)
{
	CString strFpgFileName;

	int n = m_pComboLiveVideoTemplate->GetCurSel();
	m_pComboLiveVideoTemplate->GetLBText(n, strFpgFileName);

	m_pStatus->LiveVideoConf()->Template(strFpgFileName);

	InitLiveVideo();

	UpdateData(FALSE);

}

void CClock2026Dlg::OnCbnSelchangeComboLiveVideoTemplates()
{
	SelectChangeLiveVideoTemplate();
}

void CClock2026Dlg::ReloadLiveVideoTemplate(void)
{
	m_pComboLiveVideoTemplate->ResetContent();

	CFileFind FileFinder;

	// start working for files
	BOOL bWorking = FileFinder.FindFile(GetLiveVideoDir() + _T("*.fpg"));

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();

		if (!FileFinder.IsDirectory())
		{
			CString str = CFileUtils::ExtractFileName(FileFinder.GetFilePath());

			m_pComboLiveVideoTemplate->AddString(str);
		}

	}
	FileFinder.Close();
}

void CClock2026Dlg::OnBnClickedButtonReloadLiveVideoTemplate()
{
	ReloadLiveVideoTemplate();
}

void CClock2026Dlg::OnEnKillfocusEditWeatherDisplayDuration()
{
	CString strBuffer;
	GetDlgItemText(IDC_EDIT_WEATHER_DISPLAY_DURATION, strBuffer);

	if (IsNumberString(strBuffer))
	{
		m_pStatus->WeatherConf()->DisplayDuration(min(max(GETINT(strBuffer), MIN_WEATHER_DISPLAY_DURATION), MAX_WEATHER_DISPLAY_DURATION));
	}

	SetChangeMark(TRUE);
	UpdateData(FALSE);
}

void CClock2026Dlg::OnBnClickedButtonDateFormat()
{
	AfxMessageBox(_T("표기법 -> %M:연도, %M:월, %D:일, %A:요일\n\r 예) 안녕하세요 %M %D입니다. -> 안녕하세요 11월 19일입니다. \n\r 예) %M월 %D일 (%A)요일 -> 11월 19일 (수)요일"));
}

BOOL CClock2026Dlg::Unlock_Activation()
{

	//#ifndef _SD_
	TCHAR tszVolumeName[_MAX_PATH];
	DWORD dwVolumeSerialNumber;
	DWORD dwMaximumComponentLength;
	DWORD dwFileSystemFlag;
	TCHAR tszFileSystemName[_MAX_PATH];
	

	CString strMirAct1;
	CString strMirAct2;

	::GetVolumeInformation(_T("C:\\"), tszVolumeName, _MAX_PATH, &dwVolumeSerialNumber, &dwMaximumComponentLength, &dwFileSystemFlag, tszFileSystemName, _MAX_PATH);

    DWORD  dwVolumeSerialNumberConverted = (~dwVolumeSerialNumber) ^ 0x00226740;//0x07190909;

	if (strMirAct1 != BLANK_STRING)
	{
		dwVolumeSerialNumber = GETINT(strMirAct1);
	}

	if (strMirAct2 != BLANK_STRING)
	{
		dwVolumeSerialNumberConverted = GETINT(strMirAct2);
	}

	DWORD dwActivationCodeFull = ((~dwVolumeSerialNumber) ^ 0x5A3) | ((~dwVolumeSerialNumberConverted) ^ (~0x5A3));

	TCHAR tszActivationCodeFull[_MAX_PATH];
	::_stprintf(tszActivationCodeFull, _T("%X"), dwActivationCodeFull);

	AfxMessageBox(tszActivationCodeFull);

	return TRUE;
}

void CClock2026Dlg::OnBnClickedButton1()
{
	Unlock_Activation();
}


void CClock2026Dlg::OnBnClickedCheckExceptAir()
{
	BOOL bExceptAir = IsDlgButtonChecked(IDC_CHECK_EXCEPT_AIR);

	m_pStatus->WeatherConf()->ExceptAir(bExceptAir);
}
