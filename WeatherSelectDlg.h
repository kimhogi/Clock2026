#pragma once
#include "ClockData.h"

// CWeatherSelectDlg 대화 상자입니다.

class CWeatherSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CWeatherSelectDlg)

public:
	CWeatherSelectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CWeatherSelectDlg();
private :
	 CStatus* m_pStatus;
	 std::map<INT, BOOL> m_mapSelectCity;

     
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_WEATHER_CITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckGyeonggidoEntire();
	afx_msg void OnBnClickedCheckGangwondoEntire();
	afx_msg void OnBnClickedCheckChungcheongbukdoEntire();
	afx_msg void OnBnClickedCheckChungcheongnamdoEntire();
	afx_msg void OnBnClickedCheckJeollabukdoEntire();
	afx_msg void OnBnClickedCheckJeollanamdoEntire();
	afx_msg void OnBnClickedCheckGyeongsangbukdoEntire();
	afx_msg void OnBnClickedCheckGyeongsangnamdoEntire();
	afx_msg void OnBnClickedCheckJejudoEntire();
	afx_msg void OnBnClickedCheckSeoul();
	afx_msg void OnBnClickedCheckBusan();
	afx_msg void OnBnClickedCheckDaejeon();
	afx_msg void OnBnClickedCheckGwangju();
	afx_msg void OnBnClickedCheckDaegu();
	afx_msg void OnBnClickedCheckUlsan();
	afx_msg void OnBnClickedCheckAllCities();
	afx_msg void OnBnClickedCheckIncheon();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnclickedCheck(UINT nID);
	afx_msg void OnBnClickedCheckEntireMajorCities();
};
