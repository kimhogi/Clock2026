// WeatherSelectDlg.cpp : 구현 파일입니다.
//

//#include <stdafx.h>
#include "pch.h"
#include "Clock2026.h"
#include "WeatherSelectDlg.h"
#include ".\weatherselectdlg.h"


// CWeatherSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWeatherSelectDlg, CDialog)
CWeatherSelectDlg::CWeatherSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWeatherSelectDlg::IDD, pParent)
{
	m_pStatus = theApp.GetStatus();
}

CWeatherSelectDlg::~CWeatherSelectDlg()
{
}

void CWeatherSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX,IDC_CHECK_GYEONGGIDO_ENTIRE, m_mapSelectCity[GYEONGGIDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_SUWON_SI, m_mapSelectCity[SUWON_SI]);
	DDX_Check(pDX,IDC_CHECK_SEONGNAM_SI, m_mapSelectCity[SEONGNAM_SI]);
	DDX_Check(pDX,IDC_CHECK_BUCHEON_SI, m_mapSelectCity[BUCHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_ANYANG_SI, m_mapSelectCity[ANYANG_SI]);
	DDX_Check(pDX,IDC_CHECK_ANSAN_SI, m_mapSelectCity[ANSAN_SI]);
	DDX_Check(pDX,IDC_CHECK_YONGIN_SI, m_mapSelectCity[YONGIN_SI]);
	DDX_Check(pDX,IDC_CHECK_PYEONGTAEK_SI, m_mapSelectCity[PYEONGTAEK_SI]);
	DDX_Check(pDX,IDC_CHECK_GWANGMYEONG_SI, m_mapSelectCity[GWANGMYEONG_SI]);
	DDX_Check(pDX,IDC_CHECK_SIHEUNG_SI, m_mapSelectCity[SIHEUNG_SI]);
	DDX_Check(pDX,IDC_CHECK_GUNPO_SI, m_mapSelectCity[GUNPO_SI]);
	DDX_Check(pDX,IDC_CHECK_HWASEONG_SI,m_mapSelectCity[HWASEONG_SI]);
	DDX_Check(pDX,IDC_CHECK_ICHEON_SI,m_mapSelectCity[ICHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_GIMPO_SI,m_mapSelectCity[GIMPO_SI]);
	DDX_Check(pDX,IDC_CHECK_GWANGJU_SI,m_mapSelectCity[GWANGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_ANSEONG_SI,m_mapSelectCity[ANSEONG_SI]);
	DDX_Check(pDX,IDC_CHECK_HANAM_SI,m_mapSelectCity[HANAM_SI]);
	DDX_Check(pDX,IDC_CHECK_UIWANG_SI,m_mapSelectCity[UIWANG_SI]);
	DDX_Check(pDX,IDC_CHECK_OSAN_SI,m_mapSelectCity[OSAN_SI]);
	DDX_Check(pDX,IDC_CHECK_GWACHEON_SI,m_mapSelectCity[GWACHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_YEOJU_GUN,m_mapSelectCity[YEOJU_GUN]);
	DDX_Check(pDX,IDC_CHECK_YANGPYEONG_GUN,m_mapSelectCity[YANGPYEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_GOYANG_SI,m_mapSelectCity[GOYANG_SI]);
	DDX_Check(pDX,IDC_CHECK_UIJEONGBU_SI,m_mapSelectCity[UIJEONGBU_SI]);
	DDX_Check(pDX,IDC_CHECK_NAMYANGJU_SI,m_mapSelectCity[NAMYANGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_PAJU_SI,m_mapSelectCity[PAJU_SI]);
	DDX_Check(pDX,IDC_CHECK_GURI_SI,m_mapSelectCity[GURI_SI]);
	DDX_Check(pDX,IDC_CHECK_POCHEON_SI,m_mapSelectCity[POCHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_YANGJU_SI,m_mapSelectCity[YANGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_DONGDUCHEON_SI,m_mapSelectCity[DONGDUCHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_GAPYEONG_GUN,m_mapSelectCity[GAPYEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_YEONCHEON_GUN,m_mapSelectCity[YEONCHEON_GUN]);

	DDX_Check(pDX,IDC_CHECK_GANGWONDO_ENTIRE ,m_mapSelectCity[GANGWONDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_CHUNCHEON_SI ,m_mapSelectCity[CHUNCHEON_SI ]);
	DDX_Check(pDX,IDC_CHECK_GANGNEUNG_SI ,m_mapSelectCity[GANGNEUNG_SI ]);
	DDX_Check(pDX,IDC_CHECK_WONJU_SI,m_mapSelectCity[WONJU_SI]);
	DDX_Check(pDX,IDC_CHECK_DONGHAE_SI,m_mapSelectCity[DONGHAE_SI]);
	DDX_Check(pDX,IDC_CHECK_SAMCHEOK_SI,m_mapSelectCity[SAMCHEOK_SI]);
	DDX_Check(pDX,IDC_CHECK_SOKCHO_SI,m_mapSelectCity[SOKCHO_SI]);
	DDX_Check(pDX,IDC_CHECK_TAEBAEK_SI,m_mapSelectCity[TAEBAEK_SI]);
	DDX_Check(pDX,IDC_CHECK_CHEORWON_GUN,m_mapSelectCity[CHEORWON_GUN]);
	DDX_Check(pDX,IDC_CHECK_HWACHEON_GUN,m_mapSelectCity[HWACHEON_GUN]);
	DDX_Check(pDX,IDC_CHECK_HOENGSEONG_GUN,m_mapSelectCity[HOENGSEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_YANGGU_GUN,m_mapSelectCity[YANGGU_GUN]);
	DDX_Check(pDX,IDC_CHECK_YEONGWOL_GUN,m_mapSelectCity[YEONGWOL_GUN]);
	DDX_Check(pDX,IDC_CHECK_INJE_GUN ,m_mapSelectCity[INJE_GUN ]);
	DDX_Check(pDX,IDC_CHECK_PYEONGCHANG_GUN,m_mapSelectCity[PYEONGCHANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_GOSEONG_GUN_GANGWON_DO,m_mapSelectCity[GOSEONG_GUN_GANGWON_DO]);
	DDX_Check(pDX,IDC_CHECK_JEONGSEON_GUN ,m_mapSelectCity[JEONGSEON_GUN ]);
	DDX_Check(pDX,IDC_CHECK_YANGYANG_GUN,m_mapSelectCity[YANGYANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_HONGCHEON_GUN,m_mapSelectCity[HONGCHEON_GUN]);

	DDX_Check(pDX,IDC_CHECK_CHUNGCHEONGBUKDO_ENTIRE,m_mapSelectCity[CHUNGCHEONGBUKDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_CHEONGJU_SI,m_mapSelectCity[CHEONGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_CHUNGJU_SI,m_mapSelectCity[CHUNGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_JECHEON_SI,m_mapSelectCity[JECHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_CHEONGWON_GUN,m_mapSelectCity[CHEONGWON_GUN]);
	DDX_Check(pDX,IDC_CHECK_BOEUN_GUN ,m_mapSelectCity[BOEUN_GUN ]);
	DDX_Check(pDX,IDC_CHECK_OKCHEON_GUN,m_mapSelectCity[OKCHEON_GUN]);
	DDX_Check(pDX,IDC_CHECK_YEONGDONG_GUN,m_mapSelectCity[YEONGDONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_JINCHEON_GUN,m_mapSelectCity[JINCHEON_GUN]);
	DDX_Check(pDX,IDC_CHECK_GOESAN_GUN,m_mapSelectCity[GOESAN_GUN]);
	DDX_Check(pDX,IDC_CHECK_EUMSEONG_GUN,m_mapSelectCity[EUMSEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_DANYANG_GUN,m_mapSelectCity[DANYANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_JEUNGPYEONG_GUN,m_mapSelectCity[JEUNGPYEONG_GUN]);

	DDX_Check(pDX,IDC_CHECK_CHUNGCHEONGNAMDO_ENTIRE,m_mapSelectCity[CHUNGCHEONGNAMDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_CHEONAN_SI,m_mapSelectCity[CHEONAN_SI]);
	DDX_Check(pDX,IDC_CHECK_GONGJU_SI,m_mapSelectCity[GONGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_BORYEONG_SI,m_mapSelectCity[BORYEONG_SI]);
	DDX_Check(pDX,IDC_CHECK_ASAN_SI,m_mapSelectCity[ASAN_SI]);
	DDX_Check(pDX,IDC_CHECK_SEOSAN_SI,m_mapSelectCity[SEOSAN_SI]);
	DDX_Check(pDX,IDC_CHECK_NONSAN_SI,m_mapSelectCity[NONSAN_SI]);
	DDX_Check(pDX,IDC_CHECK_SEJONG_SI,m_mapSelectCity[SEJONG_SI]);
	DDX_Check(pDX,IDC_CHECK_BUYEO_GUN ,m_mapSelectCity[BUYEO_GUN ]);
	DDX_Check(pDX,IDC_CHECK_SEOCHEON_GUN,m_mapSelectCity[SEOCHEON_GUN]);
	DDX_Check(pDX,IDC_CHECK_CHEONGYANG_GUN,m_mapSelectCity[CHEONGYANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_HONGSEONG_GUN,m_mapSelectCity[HONGSEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_YESAN_GUN ,m_mapSelectCity[YESAN_GUN]);
	DDX_Check(pDX,IDC_CHECK_TAEAN_GUN ,m_mapSelectCity[TAEAN_GUN]);
	DDX_Check(pDX,IDC_CHECK_DANGJIN_SI,m_mapSelectCity[DANGJIN_SI]);
	DDX_Check(pDX,IDC_CHECK_GEUMSAN_GUN,m_mapSelectCity[GEUMSAN_GUN]);
	DDX_Check(pDX,IDC_CHECK_GYERYONG_SI,m_mapSelectCity[GYERYONG_SI]);



	DDX_Check(pDX,IDC_CHECK_JEOLLABUKDO_ENTIRE,m_mapSelectCity[JEOLLABUKDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_JEONJU_SI,m_mapSelectCity[JEONJU_SI]);
	DDX_Check(pDX,IDC_CHECK_GUNSAN_SI ,m_mapSelectCity[GUNSAN_SI]);
	DDX_Check(pDX,IDC_CHECK_IKSAN_SI,m_mapSelectCity[IKSAN_SI]);
	DDX_Check(pDX,IDC_CHECK_JEONGEUP_SI,m_mapSelectCity[JEONGEUP_SI]);
	DDX_Check(pDX,IDC_CHECK_NAMWON_SI,m_mapSelectCity[NAMWON_SI]);
	DDX_Check(pDX,IDC_CHECK_GIMJE_SI,m_mapSelectCity[GIMJE_SI]);
	DDX_Check(pDX,IDC_CHECK_WANJU_GUN,m_mapSelectCity[WANJU_GUN]);
	DDX_Check(pDX,IDC_CHECK_JINAN_GUN,m_mapSelectCity[JINAN_GUN]);
	DDX_Check(pDX,IDC_CHECK_MUJU_GUN,m_mapSelectCity[MUJU_GUN]);
	DDX_Check(pDX,IDC_CHECK_JANGSU_GUN ,m_mapSelectCity[JANGSU_GUN]);
	DDX_Check(pDX,IDC_CHECK_IMSIL_GUN ,m_mapSelectCity[IMSIL_GUN]);
	DDX_Check(pDX,IDC_CHECK_SUNCHANG_GUN,m_mapSelectCity[SUNCHANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_GOCHANG_GUN,m_mapSelectCity[GOCHANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_BUAN_GUN ,m_mapSelectCity[BUAN_GUN]);

	DDX_Check(pDX,IDC_CHECK_JEOLLANAMDO_ENTIRE,m_mapSelectCity[JEOLLANAMDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_MOKPO_SI ,m_mapSelectCity[MOKPO_SI]);
	DDX_Check(pDX,IDC_CHECK_YEOSU_SI ,m_mapSelectCity[YEOSU_SI]);
	DDX_Check(pDX,IDC_CHECK_SUNCHEON_SI,m_mapSelectCity[SUNCHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_NAJU_SI,m_mapSelectCity[NAJU_SI]);
	DDX_Check(pDX,IDC_CHECK_GWANGYANG_SI,m_mapSelectCity[GWANGYANG_SI]);
	DDX_Check(pDX,IDC_CHECK_DAMYANG_GUN,m_mapSelectCity[DAMYANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_GOKSEONG_GUN,m_mapSelectCity[GOKSEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_BOSEONG_GUN,m_mapSelectCity[BOSEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_GURYE_GUN,m_mapSelectCity[GURYE_GUN]);
	DDX_Check(pDX,IDC_CHECK_GOHEUNG_GUN ,m_mapSelectCity[GOHEUNG_GUN]);
	DDX_Check(pDX,IDC_CHECK_HWASUN_GUN ,m_mapSelectCity[HWASUN_GUN]);
	DDX_Check(pDX,IDC_CHECK_JANGHEUNG_GUN ,m_mapSelectCity[JANGHEUNG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_GANGJIN_GUN,m_mapSelectCity[GANGJIN_GUN]);
	DDX_Check(pDX,IDC_CHECK_HAENAM_GUN,m_mapSelectCity[HAENAM_GUN]);
	DDX_Check(pDX,IDC_CHECK_YEONGAM_GUN,m_mapSelectCity[YEONGAM_GUN]);
	DDX_Check(pDX,IDC_CHECK_MUAN_GUN,m_mapSelectCity[MUAN_GUN]);
	DDX_Check(pDX,IDC_CHECK_HAMPYEONG_GUN,m_mapSelectCity[HAMPYEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_YEONGGWANG_GUN,m_mapSelectCity[YEONGGWANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_JANGSEONG_GUN ,m_mapSelectCity[JANGSEONG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_WANDO_GUN,m_mapSelectCity[WANDO_GUN]);
	DDX_Check(pDX,IDC_CHECK_JINDO_GUN,m_mapSelectCity[JINDO_GUN]);
	DDX_Check(pDX,IDC_CHECK_SINAN_GUN,m_mapSelectCity[SINAN_GUN]);
	DDX_Check(pDX,IDC_CHECK_HEUKSAN_MYEON,m_mapSelectCity[HEUKSAN_MYEON]);

	DDX_Check(pDX,IDC_CHECK_GYEONGSANGBUKDO_ENTIRE,m_mapSelectCity[GYEONGSANGBUKDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_POHANG_SI  ,m_mapSelectCity[POHANG_SI]);
	DDX_Check(pDX,IDC_CHECK_GYEONGJU_SI ,m_mapSelectCity[GYEONGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_GIMCHEON_SI ,m_mapSelectCity[GIMCHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_ANDONG_SI ,m_mapSelectCity[ANDONG_SI]);
	DDX_Check(pDX,IDC_CHECK_GUMI_SI ,m_mapSelectCity[GUMI_SI]);
	DDX_Check(pDX,IDC_CHECK_YEONGJU_SI ,m_mapSelectCity[YEONGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_YEONGCHEON_SI ,m_mapSelectCity[YEONGCHEON_SI]);
	DDX_Check(pDX,IDC_CHECK_SANGJU_SI ,m_mapSelectCity[SANGJU_SI]);
	DDX_Check(pDX,IDC_CHECK_MUNGYEONG_SI ,m_mapSelectCity[MUNGYEONG_SI]);
	DDX_Check(pDX,IDC_CHECK_GYEONGSAN_SI ,m_mapSelectCity[GYEONGSAN_SI]);
	DDX_Check(pDX,IDC_CHECK_GUNWI_GUN ,m_mapSelectCity[GUNWI_GUN]);
	DDX_Check(pDX,IDC_CHECK_CHILGOK_GUN  ,m_mapSelectCity[CHILGOK_GUN]);
	DDX_Check(pDX,IDC_CHECK_UISEONG_GUN ,m_mapSelectCity[UISEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_YECHEON_GUN ,m_mapSelectCity[YECHEON_GUN]);
	DDX_Check(pDX,IDC_CHECK_CHEONGSONG_GUN ,m_mapSelectCity[CHEONGSONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_BONGHWA_GUN ,m_mapSelectCity[BONGHWA_GUN]);
	DDX_Check(pDX,IDC_CHECK_YEONGYANG_GUN ,m_mapSelectCity[YEONGYANG_GUN]);
	DDX_Check(pDX,IDC_CHECK_ULJIN_GUN ,m_mapSelectCity[ULJIN_GUN]);
	DDX_Check(pDX,IDC_CHECK_YEONGDEOK_GUN ,m_mapSelectCity[YEONGDEOK_GUN]);
	DDX_Check(pDX,IDC_CHECK_ULLEUNG_GUN ,m_mapSelectCity[ULLEUNG_GUN]);
	DDX_Check(pDX,IDC_CHECK_CHEONGDO_GUN ,m_mapSelectCity[CHEONGDO_GUN]);
	DDX_Check(pDX,IDC_CHECK_GORYEONG_GUN ,m_mapSelectCity[GORYEONG_GUN]);
	DDX_Check(pDX,IDC_CHECK_SEONGJU_GUN ,m_mapSelectCity[SEONGJU_GUN]);


	DDX_Check(pDX,IDC_CHECK_GYEONGSANGNAMDO_ENTIRE ,m_mapSelectCity[GYEONGSANGNAMDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_CHANGWON_SI ,m_mapSelectCity[CHANGWON_SI ]);
	DDX_Check(pDX,IDC_CHECK_JINJU_SI ,m_mapSelectCity[JINJU_SI ]);
	DDX_Check(pDX,IDC_CHECK_TONGYEONG_SI ,m_mapSelectCity[TONGYEONG_SI ]);
	DDX_Check(pDX,IDC_CHECK_SACHEON_SI ,m_mapSelectCity[SACHEON_SI ]);
	DDX_Check(pDX,IDC_CHECK_GIMHAE_SI ,m_mapSelectCity[GIMHAE_SI ]);
	DDX_Check(pDX,IDC_CHECK_MIRYANG_SI ,m_mapSelectCity[MIRYANG_SI ]);
	DDX_Check(pDX,IDC_CHECK_YANGSAN_SI ,m_mapSelectCity[YANGSAN_SI ]);
	DDX_Check(pDX,IDC_CHECK_GEOJE_SI ,m_mapSelectCity[GEOJE_SI ]);
	DDX_Check(pDX,IDC_CHECK_UIRYEONG_GUN ,m_mapSelectCity[UIRYEONG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_GOSEONG_GUN ,m_mapSelectCity[GOSEONG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_HAMAN_GUN ,m_mapSelectCity[HAMAN_GUN ]);
	DDX_Check(pDX,IDC_CHECK_NAMHAE_GUN ,m_mapSelectCity[NAMHAE_GUN ]);
	DDX_Check(pDX,IDC_CHECK_CHANGNYEONG_GUN ,m_mapSelectCity[CHANGNYEONG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_HADONG_GUN ,m_mapSelectCity[HADONG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_SANCHEONG_GUN ,m_mapSelectCity[SANCHEONG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_HAMYANG_GUN ,m_mapSelectCity[HAMYANG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_GEOCHANG_GUN ,m_mapSelectCity[GEOCHANG_GUN ]);
	DDX_Check(pDX,IDC_CHECK_HAPCHEON_GUN ,m_mapSelectCity[HAPCHEON_GUN ]);

	DDX_Check(pDX,IDC_CHECK_JEJUDO_ENTIRE ,m_mapSelectCity[JEJUDO_ENTIRE]);
	DDX_Check(pDX,IDC_CHECK_JEJU_SI,m_mapSelectCity[JEJU_SI]);
	DDX_Check(pDX,IDC_CHECK_SEOGWIPO_SI,m_mapSelectCity[SEOGWIPO_SI]);
	DDX_Check(pDX,IDC_CHECK_HALLIM_EUP,m_mapSelectCity[HALLIM_EUP]);
	DDX_Check(pDX,IDC_CHECK_CHUJA_MYEON,m_mapSelectCity[CHUJA_MYEON]);
	DDX_Check(pDX,IDC_CHECK_SEONGSAN_EUP,m_mapSelectCity[SEONGSAN_EUP]);

	DDX_Check(pDX,IDC_CHECK_SEOUL,m_mapSelectCity[SEOUL ]);
	DDX_Check(pDX,IDC_CHECK_BUSAN,m_mapSelectCity[BUSAN]);
	DDX_Check(pDX,IDC_CHECK_INCHEON,m_mapSelectCity[INCHEON]);
	DDX_Check(pDX,IDC_CHECK_DAEJEON,m_mapSelectCity[DAEJEON ]);
	DDX_Check(pDX,IDC_CHECK_GWANGJU,m_mapSelectCity[GWANGJU]);
	DDX_Check(pDX,IDC_CHECK_DAEGU ,m_mapSelectCity[DAEGU]);
	DDX_Check(pDX,IDC_CHECK_ULSAN,m_mapSelectCity[ULSAN]);

	DDX_Check(pDX,IDC_CHECK_ENTIRE_MAJOR_CITIES,m_mapSelectCity[ENTIRE_MAJOR_CITIES]);
	DDX_Check(pDX,IDC_CHECK_ALL_CITIES ,m_mapSelectCity[ALL_CITIES]);

}


BEGIN_MESSAGE_MAP(CWeatherSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_GYEONGGIDO_ENTIRE, OnBnClickedCheckGyeonggidoEntire)
	ON_BN_CLICKED(IDC_CHECK_GANGWONDO_ENTIRE, OnBnClickedCheckGangwondoEntire)
	ON_BN_CLICKED(IDC_CHECK_CHUNGCHEONGBUKDO_ENTIRE, OnBnClickedCheckChungcheongbukdoEntire)
	ON_BN_CLICKED(IDC_CHECK_CHUNGCHEONGNAMDO_ENTIRE, OnBnClickedCheckChungcheongnamdoEntire)
	ON_BN_CLICKED(IDC_CHECK_JEOLLABUKDO_ENTIRE, OnBnClickedCheckJeollabukdoEntire)
	ON_BN_CLICKED(IDC_CHECK_JEOLLANAMDO_ENTIRE, OnBnClickedCheckJeollanamdoEntire)
	ON_BN_CLICKED(IDC_CHECK_GYEONGSANGBUKDO_ENTIRE, OnBnClickedCheckGyeongsangbukdoEntire)
	ON_BN_CLICKED(IDC_CHECK_GYEONGSANGNAMDO_ENTIRE, OnBnClickedCheckGyeongsangnamdoEntire)
	ON_BN_CLICKED(IDC_CHECK_JEJUDO_ENTIRE, OnBnClickedCheckJejudoEntire)
	ON_BN_CLICKED(IDC_CHECK_SEOUL, OnBnClickedCheckSeoul)
	ON_BN_CLICKED(IDC_CHECK_BUSAN, OnBnClickedCheckBusan)
	ON_BN_CLICKED(IDC_CHECK_DAEJEON, OnBnClickedCheckDaejeon)
	ON_BN_CLICKED(IDC_CHECK_GWANGJU, OnBnClickedCheckGwangju)
	ON_BN_CLICKED(IDC_CHECK_DAEGU, OnBnClickedCheckDaegu)
	ON_BN_CLICKED(IDC_CHECK_ULSAN, OnBnClickedCheckUlsan)
	ON_BN_CLICKED(IDC_CHECK_ALL_CITIES, OnBnClickedCheckAllCities)
	ON_BN_CLICKED(IDC_CHECK_INCHEON, OnBnClickedCheckIncheon)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_COMMAND_RANGE(IDC_CHECK_SUWON_SI,IDC_CHECK_ULSAN,OnBnclickedCheck)
	ON_BN_CLICKED(IDC_CHECK_ENTIRE_MAJOR_CITIES, OnBnClickedCheckEntireMajorCities)
END_MESSAGE_MAP()


// CWeatherSelectDlg 메시지 처리기입니다.
BOOL CWeatherSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_mapSelectCity = m_pStatus->WeatherConf()->MapCode_SelectCity();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CWeatherSelectDlg::OnBnclickedCheck(UINT nID)
{
	switch(nID)
	{

	case IDC_CHECK_SUWON_SI:		m_mapSelectCity[SUWON_SI]        =       IsDlgButtonChecked(IDC_CHECK_SUWON_SI); break;
	case IDC_CHECK_SEONGNAM_SI:		m_mapSelectCity[SEONGNAM_SI]		=		IsDlgButtonChecked(IDC_CHECK_SEONGNAM_SI);	break;
	case IDC_CHECK_BUCHEON_SI:		m_mapSelectCity[BUCHEON_SI]		=		IsDlgButtonChecked(IDC_CHECK_BUCHEON_SI);	break;
	case IDC_CHECK_ANYANG_SI:		m_mapSelectCity[ANYANG_SI]		=		IsDlgButtonChecked(IDC_CHECK_ANYANG_SI);		break;
	case IDC_CHECK_ANSAN_SI:		m_mapSelectCity[ANSAN_SI]		=		IsDlgButtonChecked(IDC_CHECK_ANSAN_SI);		break;
	case IDC_CHECK_YONGIN_SI:		m_mapSelectCity[YONGIN_SI]		=		IsDlgButtonChecked(IDC_CHECK_YONGIN_SI);		break;
	case IDC_CHECK_PYEONGTAEK_SI:	m_mapSelectCity[PYEONGTAEK_SI]	=		IsDlgButtonChecked(IDC_CHECK_PYEONGTAEK_SI);	break;
	case IDC_CHECK_GWANGMYEONG_SI:	m_mapSelectCity[GWANGMYEONG_SI]	=		IsDlgButtonChecked(IDC_CHECK_GWANGMYEONG_SI);break;
	case IDC_CHECK_SIHEUNG_SI:		m_mapSelectCity[SIHEUNG_SI]		=		IsDlgButtonChecked(IDC_CHECK_SIHEUNG_SI);	break;
	case IDC_CHECK_GUNPO_SI:		m_mapSelectCity[GUNPO_SI]		=		IsDlgButtonChecked(IDC_CHECK_GUNPO_SI);		break;
	case IDC_CHECK_HWASEONG_SI:		m_mapSelectCity[HWASEONG_SI]		=		IsDlgButtonChecked(IDC_CHECK_HWASEONG_SI);	break;
	case IDC_CHECK_ICHEON_SI:		m_mapSelectCity[ICHEON_SI]		=		IsDlgButtonChecked(IDC_CHECK_ICHEON_SI);		break;
	case IDC_CHECK_GIMPO_SI:		m_mapSelectCity[GIMPO_SI]		=		IsDlgButtonChecked(IDC_CHECK_GIMPO_SI);		break;
	case IDC_CHECK_GWANGJU_SI:		m_mapSelectCity[GWANGJU_SI]		=		IsDlgButtonChecked(IDC_CHECK_GWANGJU_SI);	break;
	case IDC_CHECK_ANSEONG_SI:		m_mapSelectCity[ANSEONG_SI]		=		IsDlgButtonChecked(IDC_CHECK_ANSEONG_SI);	break;
	case IDC_CHECK_HANAM_SI:		m_mapSelectCity[HANAM_SI]		=		IsDlgButtonChecked(IDC_CHECK_HANAM_SI);		break;
	case IDC_CHECK_UIWANG_SI:		m_mapSelectCity[UIWANG_SI]		=		IsDlgButtonChecked(IDC_CHECK_UIWANG_SI);		break;
	case IDC_CHECK_OSAN_SI:			m_mapSelectCity[OSAN_SI]			=		IsDlgButtonChecked(IDC_CHECK_OSAN_SI);		break;
	case IDC_CHECK_GWACHEON_SI:		m_mapSelectCity[GWACHEON_SI]		=		IsDlgButtonChecked(IDC_CHECK_GWACHEON_SI);	break;
	case IDC_CHECK_YEOJU_GUN:		m_mapSelectCity[YEOJU_GUN]		=		IsDlgButtonChecked(IDC_CHECK_YEOJU_GUN);		break;
	case IDC_CHECK_YANGPYEONG_GUN:	m_mapSelectCity[YANGPYEONG_GUN]	=		IsDlgButtonChecked(IDC_CHECK_YANGPYEONG_GUN);break;
	case IDC_CHECK_GOYANG_SI:		m_mapSelectCity[GOYANG_SI]		=		IsDlgButtonChecked(IDC_CHECK_GOYANG_SI);		break;
	case IDC_CHECK_UIJEONGBU_SI:	m_mapSelectCity[UIJEONGBU_SI]	=		IsDlgButtonChecked(IDC_CHECK_UIJEONGBU_SI);	break;
	case IDC_CHECK_NAMYANGJU_SI:	m_mapSelectCity[NAMYANGJU_SI]	=		IsDlgButtonChecked(IDC_CHECK_NAMYANGJU_SI);	break;
	case IDC_CHECK_PAJU_SI:			m_mapSelectCity[PAJU_SI]			=		IsDlgButtonChecked(IDC_CHECK_PAJU_SI);		break;
	case IDC_CHECK_GURI_SI:			m_mapSelectCity[GURI_SI]			=		IsDlgButtonChecked(IDC_CHECK_GURI_SI);		break;
	case IDC_CHECK_POCHEON_SI:		m_mapSelectCity[POCHEON_SI]		=		IsDlgButtonChecked(IDC_CHECK_POCHEON_SI);	break;
	case IDC_CHECK_YANGJU_SI:		m_mapSelectCity[YANGJU_SI]		=		IsDlgButtonChecked(IDC_CHECK_YANGJU_SI);		break;
	case IDC_CHECK_DONGDUCHEON_SI:	m_mapSelectCity[DONGDUCHEON_SI]	=		IsDlgButtonChecked(IDC_CHECK_DONGDUCHEON_SI);break;
	case IDC_CHECK_GAPYEONG_GUN:	m_mapSelectCity[GAPYEONG_GUN]	=		IsDlgButtonChecked(IDC_CHECK_GAPYEONG_GUN);	break;
	case IDC_CHECK_YEONCHEON_GUN:	m_mapSelectCity[YEONCHEON_GUN]	=		IsDlgButtonChecked(IDC_CHECK_YEONCHEON_GUN);	break;

	case IDC_CHECK_CHUNCHEON_SI :	m_mapSelectCity[CHUNCHEON_SI]	=		IsDlgButtonChecked(IDC_CHECK_CHUNCHEON_SI);	break;
	case IDC_CHECK_GANGNEUNG_SI :	m_mapSelectCity[GANGNEUNG_SI]	=		IsDlgButtonChecked(IDC_CHECK_GANGNEUNG_SI);	break;
	case IDC_CHECK_WONJU_SI:		m_mapSelectCity[WONJU_SI]		=		IsDlgButtonChecked(IDC_CHECK_WONJU_SI);		break;
	case IDC_CHECK_DONGHAE_SI:		m_mapSelectCity[DONGHAE_SI]		=		IsDlgButtonChecked(IDC_CHECK_DONGHAE_SI);	break;
	case IDC_CHECK_SAMCHEOK_SI:		m_mapSelectCity[SAMCHEOK_SI]		=		IsDlgButtonChecked(IDC_CHECK_SAMCHEOK_SI);	break;
	case IDC_CHECK_SOKCHO_SI:		m_mapSelectCity[SOKCHO_SI]		=		IsDlgButtonChecked(IDC_CHECK_SOKCHO_SI);		break;
	case IDC_CHECK_TAEBAEK_SI:		m_mapSelectCity[TAEBAEK_SI]		=		IsDlgButtonChecked(IDC_CHECK_TAEBAEK_SI);	break;
	case IDC_CHECK_CHEORWON_GUN:	m_mapSelectCity[CHEORWON_GUN]	=		IsDlgButtonChecked(IDC_CHECK_CHEORWON_GUN);	break;
	case IDC_CHECK_HWACHEON_GUN:	m_mapSelectCity[HWACHEON_GUN]	=		IsDlgButtonChecked(IDC_CHECK_HWACHEON_GUN);	break;
	case IDC_CHECK_HOENGSEONG_GUN:	m_mapSelectCity[HOENGSEONG_GUN]	=		IsDlgButtonChecked(IDC_CHECK_HOENGSEONG_GUN);break;
	case IDC_CHECK_YANGGU_GUN:		m_mapSelectCity[YANGGU_GUN]		=		IsDlgButtonChecked(IDC_CHECK_YANGGU_GUN);	break;
	case IDC_CHECK_YEONGWOL_GUN:	m_mapSelectCity[YEONGWOL_GUN]	=		IsDlgButtonChecked(IDC_CHECK_YEONGWOL_GUN);	break;
	case IDC_CHECK_INJE_GUN :		m_mapSelectCity[INJE_GUN ]		=		IsDlgButtonChecked(IDC_CHECK_INJE_GUN );		break;
	case IDC_CHECK_PYEONGCHANG_GUN:	m_mapSelectCity[PYEONGCHANG_GUN] =		IsDlgButtonChecked(IDC_CHECK_PYEONGCHANG_GUN); break;
	case IDC_CHECK_GOSEONG_GUN_GANGWON_DO:m_mapSelectCity[GOSEONG_GUN_GANGWON_DO] = IsDlgButtonChecked(IDC_CHECK_GOSEONG_GUN_GANGWON_DO);break;
	case IDC_CHECK_JEONGSEON_GUN :	m_mapSelectCity[JEONGSEON_GUN ]	= IsDlgButtonChecked(IDC_CHECK_JEONGSEON_GUN );	break;
	case IDC_CHECK_YANGYANG_GUN:	m_mapSelectCity[YANGYANG_GUN]	= IsDlgButtonChecked(IDC_CHECK_YANGYANG_GUN);	break;
	case IDC_CHECK_HONGCHEON_GUN:	m_mapSelectCity[HONGCHEON_GUN]	= IsDlgButtonChecked(IDC_CHECK_HONGCHEON_GUN);	break;


	case IDC_CHECK_CHEONGJU_SI:		m_mapSelectCity[CHEONGJU_SI]		= IsDlgButtonChecked(IDC_CHECK_CHEONGJU_SI);		break;
	case IDC_CHECK_CHUNGJU_SI:		m_mapSelectCity[CHUNGJU_SI]		= IsDlgButtonChecked(IDC_CHECK_CHUNGJU_SI);		break;
	case IDC_CHECK_JECHEON_SI:		m_mapSelectCity[JECHEON_SI]		= IsDlgButtonChecked(IDC_CHECK_JECHEON_SI);		break;
	case IDC_CHECK_CHEONGWON_GUN:	m_mapSelectCity[CHEONGWON_GUN]	= IsDlgButtonChecked(IDC_CHECK_CHEONGWON_GUN);	break;
	case IDC_CHECK_BOEUN_GUN :		m_mapSelectCity[BOEUN_GUN ]		= IsDlgButtonChecked(IDC_CHECK_BOEUN_GUN );		break;
	case IDC_CHECK_OKCHEON_GUN:		m_mapSelectCity[OKCHEON_GUN]		= IsDlgButtonChecked(IDC_CHECK_OKCHEON_GUN);		break;
	case IDC_CHECK_YEONGDONG_GUN:	m_mapSelectCity[YEONGDONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_YEONGDONG_GUN);	break;
	case IDC_CHECK_JINCHEON_GUN:	m_mapSelectCity[JINCHEON_GUN]	= IsDlgButtonChecked(IDC_CHECK_JINCHEON_GUN);	break;
	case IDC_CHECK_GOESAN_GUN:		m_mapSelectCity[GOESAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_GOESAN_GUN);		break;
	case IDC_CHECK_EUMSEONG_GUN:	m_mapSelectCity[EUMSEONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_EUMSEONG_GUN);	break;
	case IDC_CHECK_DANYANG_GUN:		m_mapSelectCity[DANYANG_GUN]		= IsDlgButtonChecked(IDC_CHECK_DANYANG_GUN);		break;
	case IDC_CHECK_JEUNGPYEONG_GUN:	m_mapSelectCity[JEUNGPYEONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_JEUNGPYEONG_GUN);	break;

	case IDC_CHECK_CHEONAN_SI:		m_mapSelectCity[CHEONAN_SI]		= IsDlgButtonChecked(IDC_CHECK_CHEONAN_SI);		break;
	case IDC_CHECK_GONGJU_SI:		m_mapSelectCity[GONGJU_SI]		= IsDlgButtonChecked(IDC_CHECK_GONGJU_SI);		break;
	case IDC_CHECK_BORYEONG_SI:		m_mapSelectCity[BORYEONG_SI]		= IsDlgButtonChecked(IDC_CHECK_BORYEONG_SI);		break;
	case IDC_CHECK_ASAN_SI:			m_mapSelectCity[ASAN_SI]			= IsDlgButtonChecked(IDC_CHECK_ASAN_SI);			break;
	case IDC_CHECK_SEOSAN_SI:		m_mapSelectCity[SEOSAN_SI]		= IsDlgButtonChecked(IDC_CHECK_SEOSAN_SI);		break;
	case IDC_CHECK_NONSAN_SI:		m_mapSelectCity[NONSAN_SI]		= IsDlgButtonChecked(IDC_CHECK_NONSAN_SI);		break;
	case IDC_CHECK_SEJONG_SI:		m_mapSelectCity[SEJONG_SI]		= IsDlgButtonChecked(IDC_CHECK_SEJONG_SI);		break;
	case IDC_CHECK_BUYEO_GUN :		m_mapSelectCity[BUYEO_GUN ]		= IsDlgButtonChecked(IDC_CHECK_BUYEO_GUN);		break;
	case IDC_CHECK_SEOCHEON_GUN:	m_mapSelectCity[SEOCHEON_GUN]	= IsDlgButtonChecked(IDC_CHECK_SEOCHEON_GUN);	break;
	case IDC_CHECK_CHEONGYANG_GUN:	m_mapSelectCity[CHEONGYANG_GUN]	= IsDlgButtonChecked(IDC_CHECK_CHEONGYANG_GUN);	break;
	case IDC_CHECK_HONGSEONG_GUN:	m_mapSelectCity[HONGSEONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_HONGSEONG_GUN);	break;
	case IDC_CHECK_YESAN_GUN :		m_mapSelectCity[YESAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_YESAN_GUN);		break;
	case IDC_CHECK_TAEAN_GUN :		m_mapSelectCity[TAEAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_TAEAN_GUN);		break;
	case IDC_CHECK_DANGJIN_SI:		m_mapSelectCity[DANGJIN_SI]		= IsDlgButtonChecked(IDC_CHECK_DANGJIN_SI);		break;
	case IDC_CHECK_GEUMSAN_GUN:		m_mapSelectCity[GEUMSAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_GEUMSAN_GUN);		break;
	case IDC_CHECK_GYERYONG_SI:		m_mapSelectCity[GYERYONG_SI]		= IsDlgButtonChecked(IDC_CHECK_GYERYONG_SI);		break;

	case IDC_CHECK_JEONJU_SI:		m_mapSelectCity[JEONJU_SI]		= IsDlgButtonChecked(IDC_CHECK_JEONJU_SI);		break;
	case IDC_CHECK_GUNSAN_SI :		m_mapSelectCity[GUNSAN_SI]		= IsDlgButtonChecked(IDC_CHECK_GUNSAN_SI);		break;
	case IDC_CHECK_IKSAN_SI:		m_mapSelectCity[IKSAN_SI]		= IsDlgButtonChecked(IDC_CHECK_IKSAN_SI);		break;
	case IDC_CHECK_JEONGEUP_SI:		m_mapSelectCity[JEONGEUP_SI]		= IsDlgButtonChecked(IDC_CHECK_JEONGEUP_SI);		break;
	case IDC_CHECK_NAMWON_SI:		m_mapSelectCity[NAMWON_SI]		= IsDlgButtonChecked(IDC_CHECK_NAMWON_SI);		break;
	case IDC_CHECK_GIMJE_SI:		m_mapSelectCity[GIMJE_SI]		= IsDlgButtonChecked(IDC_CHECK_GIMJE_SI);		break;
	case IDC_CHECK_WANJU_GUN:		m_mapSelectCity[WANJU_GUN]		= IsDlgButtonChecked(IDC_CHECK_WANJU_GUN);		break;
	case IDC_CHECK_JINAN_GUN:		m_mapSelectCity[JINAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_JINAN_GUN);		break;
	case IDC_CHECK_MUJU_GUN:		m_mapSelectCity[MUJU_GUN]		= IsDlgButtonChecked(IDC_CHECK_MUJU_GUN);		break;
	case IDC_CHECK_JANGSU_GUN :		m_mapSelectCity[JANGSU_GUN]		= IsDlgButtonChecked(IDC_CHECK_JANGSU_GUN);		break;
	case IDC_CHECK_IMSIL_GUN :		m_mapSelectCity[IMSIL_GUN]		= IsDlgButtonChecked(IDC_CHECK_IMSIL_GUN);		break;
	case IDC_CHECK_SUNCHANG_GUN:	m_mapSelectCity[SUNCHANG_GUN]	= IsDlgButtonChecked(IDC_CHECK_SUNCHANG_GUN);	break;
	case IDC_CHECK_GOCHANG_GUN:		m_mapSelectCity[GOCHANG_GUN]		= IsDlgButtonChecked(IDC_CHECK_GOCHANG_GUN);		break;
	case IDC_CHECK_BUAN_GUN :		m_mapSelectCity[BUAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_BUAN_GUN);		break;

	case IDC_CHECK_MOKPO_SI :		m_mapSelectCity[MOKPO_SI]		= IsDlgButtonChecked(IDC_CHECK_MOKPO_SI);		break;
	case IDC_CHECK_YEOSU_SI :		m_mapSelectCity[YEOSU_SI]		= IsDlgButtonChecked(IDC_CHECK_YEOSU_SI);		break;
	case IDC_CHECK_SUNCHEON_SI:		m_mapSelectCity[SUNCHEON_SI]		= IsDlgButtonChecked(IDC_CHECK_SUNCHEON_SI);		break;
	case IDC_CHECK_NAJU_SI:			m_mapSelectCity[NAJU_SI]			= IsDlgButtonChecked(IDC_CHECK_NAJU_SI);			break;
	case IDC_CHECK_GWANGYANG_SI:	m_mapSelectCity[GWANGYANG_SI]	= IsDlgButtonChecked(IDC_CHECK_GWANGYANG_SI);	break;
	case IDC_CHECK_DAMYANG_GUN:		m_mapSelectCity[DAMYANG_GUN]		= IsDlgButtonChecked(IDC_CHECK_DAMYANG_GUN);		break;
	case IDC_CHECK_GOKSEONG_GUN:	m_mapSelectCity[GOKSEONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_GOKSEONG_GUN);	break;
	case IDC_CHECK_BOSEONG_GUN:		m_mapSelectCity[BOSEONG_GUN]		= IsDlgButtonChecked(IDC_CHECK_BOSEONG_GUN);		break;
	case IDC_CHECK_GURYE_GUN:		m_mapSelectCity[GURYE_GUN]		= IsDlgButtonChecked(IDC_CHECK_GURYE_GUN);		break;
	case IDC_CHECK_GOHEUNG_GUN :	m_mapSelectCity[GOHEUNG_GUN]		= IsDlgButtonChecked(IDC_CHECK_GOHEUNG_GUN);		break;
	case IDC_CHECK_HWASUN_GUN :		m_mapSelectCity[HWASUN_GUN]		= IsDlgButtonChecked(IDC_CHECK_HWASUN_GUN);		break;
	case IDC_CHECK_JANGHEUNG_GUN :	m_mapSelectCity[JANGHEUNG_GUN ]	= IsDlgButtonChecked(IDC_CHECK_JANGHEUNG_GUN);	break;
	case IDC_CHECK_GANGJIN_GUN:		m_mapSelectCity[GANGJIN_GUN]		= IsDlgButtonChecked(IDC_CHECK_GANGJIN_GUN);		break;
	case IDC_CHECK_HAENAM_GUN:		m_mapSelectCity[HAENAM_GUN]		= IsDlgButtonChecked(IDC_CHECK_HAENAM_GUN);		break;
	case IDC_CHECK_YEONGAM_GUN:		m_mapSelectCity[YEONGAM_GUN]		= IsDlgButtonChecked(IDC_CHECK_YEONGAM_GUN);		break;
	case IDC_CHECK_MUAN_GUN:		m_mapSelectCity[MUAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_MUAN_GUN);		break;
	case IDC_CHECK_HAMPYEONG_GUN:	m_mapSelectCity[HAMPYEONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_HAMPYEONG_GUN);	break;
	case IDC_CHECK_YEONGGWANG_GUN:	m_mapSelectCity[YEONGGWANG_GUN]	= IsDlgButtonChecked(IDC_CHECK_YEONGGWANG_GUN);	break;
	case IDC_CHECK_JANGSEONG_GUN :	m_mapSelectCity[JANGSEONG_GUN ]	= IsDlgButtonChecked(IDC_CHECK_JANGSEONG_GUN);	break;
	case IDC_CHECK_WANDO_GUN:		m_mapSelectCity[WANDO_GUN]		= IsDlgButtonChecked(IDC_CHECK_WANDO_GUN);		break;
	case IDC_CHECK_JINDO_GUN:		m_mapSelectCity[JINDO_GUN]		= IsDlgButtonChecked(IDC_CHECK_JINDO_GUN);		break;
	case IDC_CHECK_SINAN_GUN:		m_mapSelectCity[SINAN_GUN]		= IsDlgButtonChecked(IDC_CHECK_SINAN_GUN);		break;
	case IDC_CHECK_HEUKSAN_MYEON:	m_mapSelectCity[HEUKSAN_MYEON]	= IsDlgButtonChecked(IDC_CHECK_HEUKSAN_MYEON);	break;

	case IDC_CHECK_POHANG_SI:		m_mapSelectCity[POHANG_SI]		= IsDlgButtonChecked(IDC_CHECK_POHANG_SI);		break;
	case IDC_CHECK_GYEONGJU_SI:		m_mapSelectCity[GYEONGJU_SI]		= IsDlgButtonChecked(IDC_CHECK_GYEONGJU_SI);		break;
	case IDC_CHECK_GIMCHEON_SI:		m_mapSelectCity[GIMCHEON_SI]		= IsDlgButtonChecked(IDC_CHECK_GIMCHEON_SI);		break;
	case IDC_CHECK_ANDONG_SI:		m_mapSelectCity[ANDONG_SI]		= IsDlgButtonChecked(IDC_CHECK_ANDONG_SI);		break;
	case IDC_CHECK_GUMI_SI:			m_mapSelectCity[GUMI_SI]			= IsDlgButtonChecked(IDC_CHECK_GUMI_SI);			break;
	case IDC_CHECK_YEONGJU_SI :		m_mapSelectCity[YEONGJU_SI]		= IsDlgButtonChecked(IDC_CHECK_YEONGJU_SI);		break;
	case IDC_CHECK_YEONGCHEON_SI:	m_mapSelectCity[YEONGCHEON_SI]	= IsDlgButtonChecked(IDC_CHECK_YEONGCHEON_SI);	break;
	case IDC_CHECK_SANGJU_SI :		m_mapSelectCity[SANGJU_SI]		= IsDlgButtonChecked(IDC_CHECK_SANGJU_SI);		break;
	case IDC_CHECK_MUNGYEONG_SI :	m_mapSelectCity[MUNGYEONG_SI]	= IsDlgButtonChecked(IDC_CHECK_MUNGYEONG_SI);	break;
	case IDC_CHECK_GYEONGSAN_SI :	m_mapSelectCity[GYEONGSAN_SI]	= IsDlgButtonChecked(IDC_CHECK_GYEONGSAN_SI);	break;
	case IDC_CHECK_GUNWI_GUN :		m_mapSelectCity[GUNWI_GUN]		= IsDlgButtonChecked(IDC_CHECK_GUNWI_GUN);		break;
	case IDC_CHECK_CHILGOK_GUN  :	m_mapSelectCity[CHILGOK_GUN]		= IsDlgButtonChecked(IDC_CHECK_CHILGOK_GUN);		break;
	case IDC_CHECK_UISEONG_GUN :	m_mapSelectCity[UISEONG_GUN]		= IsDlgButtonChecked(IDC_CHECK_UISEONG_GUN);		break;
	case IDC_CHECK_YECHEON_GUN :	m_mapSelectCity[YECHEON_GUN]		= IsDlgButtonChecked(IDC_CHECK_YECHEON_GUN);		break;
	case IDC_CHECK_CHEONGSONG_GUN :	m_mapSelectCity[CHEONGSONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_CHEONGSONG_GUN);	break;
	case IDC_CHECK_BONGHWA_GUN :	m_mapSelectCity[BONGHWA_GUN]		= IsDlgButtonChecked(IDC_CHECK_BONGHWA_GUN);		break;
	case IDC_CHECK_YEONGYANG_GUN :	m_mapSelectCity[YEONGYANG_GUN]	= IsDlgButtonChecked(IDC_CHECK_YEONGYANG_GUN);	break;
	case IDC_CHECK_ULJIN_GUN :		m_mapSelectCity[ULJIN_GUN]		= IsDlgButtonChecked(IDC_CHECK_ULJIN_GUN);		break;
	case IDC_CHECK_YEONGDEOK_GUN :	m_mapSelectCity[YEONGDEOK_GUN]	= IsDlgButtonChecked(IDC_CHECK_YEONGDEOK_GUN);	break;
	case IDC_CHECK_ULLEUNG_GUN :	m_mapSelectCity[ULLEUNG_GUN]		= IsDlgButtonChecked(IDC_CHECK_ULLEUNG_GUN);		break;
	case IDC_CHECK_CHEONGDO_GUN :	m_mapSelectCity[CHEONGDO_GUN]	= IsDlgButtonChecked(IDC_CHECK_CHEONGDO_GUN);	break;
	case IDC_CHECK_GORYEONG_GUN :	m_mapSelectCity[GORYEONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_GORYEONG_GUN);	break;
	case IDC_CHECK_SEONGJU_GUN :	m_mapSelectCity[SEONGJU_GUN]		= IsDlgButtonChecked(IDC_CHECK_SEONGJU_GUN);		break;

	case IDC_CHECK_CHANGWON_SI :	m_mapSelectCity[CHANGWON_SI ]	= IsDlgButtonChecked(IDC_CHECK_CHANGWON_SI);	break;

	case IDC_CHECK_JINJU_SI :		m_mapSelectCity[JINJU_SI ]		= IsDlgButtonChecked(IDC_CHECK_JINJU_SI );		break;

	case IDC_CHECK_TONGYEONG_SI :	m_mapSelectCity[TONGYEONG_SI ]	= IsDlgButtonChecked(IDC_CHECK_TONGYEONG_SI);	break;
	case IDC_CHECK_SACHEON_SI :		m_mapSelectCity[SACHEON_SI ]		= IsDlgButtonChecked(IDC_CHECK_SACHEON_SI);		break;
	case IDC_CHECK_GIMHAE_SI :		m_mapSelectCity[GIMHAE_SI ]		= IsDlgButtonChecked(IDC_CHECK_GIMHAE_SI);		break;
	case IDC_CHECK_MIRYANG_SI :		m_mapSelectCity[MIRYANG_SI ]		= IsDlgButtonChecked(IDC_CHECK_MIRYANG_SI);		break;
	case IDC_CHECK_YANGSAN_SI :		m_mapSelectCity[YANGSAN_SI ]		= IsDlgButtonChecked(IDC_CHECK_YANGSAN_SI);		break;
	case IDC_CHECK_GEOJE_SI :		m_mapSelectCity[GEOJE_SI ]		= IsDlgButtonChecked(IDC_CHECK_GEOJE_SI);		break;
	case IDC_CHECK_UIRYEONG_GUN :	m_mapSelectCity[UIRYEONG_GUN ]	= IsDlgButtonChecked(IDC_CHECK_UIRYEONG_GUN);	break;
	case IDC_CHECK_GOSEONG_GUN :	m_mapSelectCity[GOSEONG_GUN ]	= IsDlgButtonChecked(IDC_CHECK_GOSEONG_GUN );	break;
	case IDC_CHECK_HAMAN_GUN :		m_mapSelectCity[HAMAN_GUN ]		= IsDlgButtonChecked(IDC_CHECK_HAMAN_GUN );		break;
	case IDC_CHECK_NAMHAE_GUN :		m_mapSelectCity[NAMHAE_GUN ]		= IsDlgButtonChecked(IDC_CHECK_NAMHAE_GUN );		break;
	case IDC_CHECK_CHANGNYEONG_GUN :m_mapSelectCity[CHANGNYEONG_GUN]	= IsDlgButtonChecked(IDC_CHECK_CHANGNYEONG_GUN);	break;
	case IDC_CHECK_HADONG_GUN :		m_mapSelectCity[HADONG_GUN ]		= IsDlgButtonChecked(IDC_CHECK_HADONG_GUN );		break;
	case IDC_CHECK_SANCHEONG_GUN :	m_mapSelectCity[SANCHEONG_GUN ]	= IsDlgButtonChecked(IDC_CHECK_SANCHEONG_GUN);	break;
	case IDC_CHECK_HAMYANG_GUN :	m_mapSelectCity[HAMYANG_GUN ]	= IsDlgButtonChecked(IDC_CHECK_HAMYANG_GUN);	break;
	case IDC_CHECK_GEOCHANG_GUN :	m_mapSelectCity[GEOCHANG_GUN ]	= IsDlgButtonChecked(IDC_CHECK_GEOCHANG_GUN);	break;
	case IDC_CHECK_HAPCHEON_GUN :	m_mapSelectCity[HAPCHEON_GUN ]	= IsDlgButtonChecked(IDC_CHECK_HAPCHEON_GUN);	break;

	case IDC_CHECK_JEJU_SI:			m_mapSelectCity[JEJU_SI]			= IsDlgButtonChecked(IDC_CHECK_JEJU_SI);			break;
	case IDC_CHECK_SEOGWIPO_SI:		m_mapSelectCity[SEOGWIPO_SI]		= IsDlgButtonChecked(IDC_CHECK_SEOGWIPO_SI);		break;
	case IDC_CHECK_HALLIM_EUP:		m_mapSelectCity[HALLIM_EUP]		= IsDlgButtonChecked(IDC_CHECK_HALLIM_EUP);		break;
	case IDC_CHECK_CHUJA_MYEON:		m_mapSelectCity[CHUJA_MYEON]		= IsDlgButtonChecked(IDC_CHECK_CHUJA_MYEON);		break;
	case IDC_CHECK_SEONGSAN_EUP:	m_mapSelectCity[SEONGSAN_EUP]	= IsDlgButtonChecked(IDC_CHECK_SEONGSAN_EUP);	break;

		default: break;
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckGyeonggidoEntire()
{	 
	m_mapSelectCity[GYEONGGIDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_GYEONGGIDO_ENTIRE);
	for(int i=SUWON_SI;i<=YEONCHEON_GUN;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[GYEONGGIDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckGangwondoEntire()
{
	m_mapSelectCity[GANGWONDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_GANGWONDO_ENTIRE);
	for(int i=CHUNCHEON_SI ;i<=HONGCHEON_GUN ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[GANGWONDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckChungcheongbukdoEntire()
{
	m_mapSelectCity[CHUNGCHEONGBUKDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_CHUNGCHEONGBUKDO_ENTIRE);
	for(int i=CHEONGJU_SI ;i<=JEUNGPYEONG_GUN  ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[CHUNGCHEONGBUKDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckChungcheongnamdoEntire()
{
	m_mapSelectCity[CHUNGCHEONGNAMDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_CHUNGCHEONGNAMDO_ENTIRE);
	for(int i=CHEONAN_SI ;i<=GYERYONG_SI  ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[CHUNGCHEONGNAMDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckJeollabukdoEntire()
{
	m_mapSelectCity[JEOLLABUKDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_JEOLLABUKDO_ENTIRE);
	for(int i=JEONJU_SI ;i<=BUAN_GUN  ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[JEOLLABUKDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckJeollanamdoEntire()
{
	m_mapSelectCity[JEOLLANAMDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_JEOLLANAMDO_ENTIRE);
	for(int i=MOKPO_SI ;i<=HEUKSAN_MYEON  ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[JEOLLANAMDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckGyeongsangbukdoEntire()
{
	m_mapSelectCity[GYEONGSANGBUKDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_GYEONGSANGBUKDO_ENTIRE);
	for(int i=POHANG_SI   ;i<=SEONGJU_GUN;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[GYEONGSANGBUKDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckGyeongsangnamdoEntire()
{
	m_mapSelectCity[GYEONGSANGNAMDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_GYEONGSANGNAMDO_ENTIRE);
	for(int i=CHANGWON_SI;i<=HAPCHEON_GUN;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[GYEONGSANGNAMDO_ENTIRE];
	}
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckJejudoEntire()
{
	m_mapSelectCity[JEJUDO_ENTIRE] = IsDlgButtonChecked(IDC_CHECK_JEJUDO_ENTIRE);
	for(int i=JEJU_SI;i<=SEONGSAN_EUP;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[JEJUDO_ENTIRE];
	}
	UpdateData(FALSE);

}



void CWeatherSelectDlg::OnBnClickedCheckSeoul()
{	 
	m_mapSelectCity[SEOUL] = IsDlgButtonChecked(IDC_CHECK_SEOUL);
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckBusan()
{
	 
	m_mapSelectCity[BUSAN] = IsDlgButtonChecked(IDC_CHECK_BUSAN);
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckIncheon()
{
	 	
	m_mapSelectCity[INCHEON] = IsDlgButtonChecked(IDC_CHECK_INCHEON);
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckDaejeon()
{
	 
	m_mapSelectCity[DAEJEON] = IsDlgButtonChecked(IDC_CHECK_DAEJEON);
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckGwangju()
{
	 
	m_mapSelectCity[GWANGJU] = IsDlgButtonChecked(IDC_CHECK_GWANGJU);
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckDaegu()
{
	 
	m_mapSelectCity[DAEGU] = IsDlgButtonChecked(IDC_CHECK_DAEGU);
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckUlsan()
{
	 
	m_mapSelectCity[ULSAN] = IsDlgButtonChecked(IDC_CHECK_ULSAN);
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedCheckAllCities()
{
	 
	m_mapSelectCity[ALL_CITIES] = IsDlgButtonChecked(IDC_CHECK_ALL_CITIES);
	for(int i=GYEONGGIDO_ENTIRE ;i<=YEONCHEON_GUN ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=GANGWONDO_ENTIRE ;i<=HONGCHEON_GUN ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=CHUNGCHEONGBUKDO_ENTIRE ;i<=JEUNGPYEONG_GUN ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=CHUNGCHEONGNAMDO_ENTIRE ;i<=GYERYONG_SI ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=JEOLLABUKDO_ENTIRE ;i<=BUAN_GUN ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=JEOLLANAMDO_ENTIRE ;i<=HEUKSAN_MYEON ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=GYEONGSANGBUKDO_ENTIRE ;i<=SEONGJU_GUN ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=GYEONGSANGNAMDO_ENTIRE ;i<=HAPCHEON_GUN ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	for(int i=JEJUDO_ENTIRE ;i<=SEONGSAN_EUP ;i++)
	{
		m_mapSelectCity[i] = m_mapSelectCity[ALL_CITIES];
	}
	m_mapSelectCity[ENTIRE_MAJOR_CITIES] = m_mapSelectCity[ALL_CITIES];
	m_mapSelectCity[SEOUL] = m_mapSelectCity[ALL_CITIES];
	m_mapSelectCity[BUSAN] = m_mapSelectCity[ALL_CITIES];
	m_mapSelectCity[INCHEON] = m_mapSelectCity[ALL_CITIES];
	m_mapSelectCity[DAEGU] = m_mapSelectCity[ALL_CITIES];
	m_mapSelectCity[DAEJEON] = m_mapSelectCity[ALL_CITIES];
	m_mapSelectCity[GWANGJU] = m_mapSelectCity[ALL_CITIES];
	m_mapSelectCity[ULSAN] = m_mapSelectCity[ALL_CITIES];
	UpdateData(FALSE);
}

void CWeatherSelectDlg::OnBnClickedOk()
{
	m_pStatus->WeatherConf()->MapCode_SelectCity(m_mapSelectCity);
	OnOK();
}

void CWeatherSelectDlg::OnBnClickedCheckEntireMajorCities()
{
	 
	m_mapSelectCity[ENTIRE_MAJOR_CITIES] = IsDlgButtonChecked(IDC_CHECK_ENTIRE_MAJOR_CITIES);
	m_mapSelectCity[SEOUL] = m_mapSelectCity[ENTIRE_MAJOR_CITIES];
	m_mapSelectCity[BUSAN] = m_mapSelectCity[ENTIRE_MAJOR_CITIES];
	m_mapSelectCity[INCHEON] = m_mapSelectCity[ENTIRE_MAJOR_CITIES];
	m_mapSelectCity[DAEGU] = m_mapSelectCity[ENTIRE_MAJOR_CITIES];
	m_mapSelectCity[DAEJEON] = m_mapSelectCity[ENTIRE_MAJOR_CITIES];
	m_mapSelectCity[GWANGJU] = m_mapSelectCity[ENTIRE_MAJOR_CITIES];
	m_mapSelectCity[ULSAN] = m_mapSelectCity[ENTIRE_MAJOR_CITIES];
	
	UpdateData(FALSE);
}


void CWeatherSelectDlg::OnBnClickedCancel()
{
	OnCancel();
}

