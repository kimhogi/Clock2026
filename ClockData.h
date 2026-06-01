#pragma once

#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

extern CString GetDataDir();
extern CString GetLogsDir();
extern CString GetMirDir();
extern CString GetLiveVideoDir();
extern CString GetScrollDir();
extern CString GetNoticeDir();
extern CString GetExportDir();
extern CString GetManualUpDir();
extern CString GetImagesDir();
extern CString GetWeatherIconsDir();

using namespace JSONPP;
using namespace libxl;

#define WAIT_TIME_FOR_WEATHER_READING 5000


constexpr auto WM_DISP_CHECK = (WM_APP + 400);
constexpr auto WM_APPLY_NOTICE = (WM_APP + 401);


constexpr auto NUM_SNAP = 3;

#define DEFAULT_FILENAME   GetDataDir() + _T("최근사용.mpt")
#define DEFAULT_WEATHER_DATA_CONFIG_FILENAME GetDataDir() + _T("날씨설정.TXT")
#define AIR_REGION_EXCEL_FILE GetDataDir() + _T("Air_Region.xls")
#define WEATHER_CODE_INFO_XLS_FILENAME GetDataDir() + _T("Weather_code_Info.xls")

#define DEFAULT_NOTICE_FILENAME      CString(_T("공백_5초.fpg"))
#define AGE_ALIAS                    CString(_T("연령"))
#define DEFAULT_LIVE_VIDEO_TEMPLATE  CString(_T("입력비디오_우하단.fpg"))


const static CString MAIN_DIALOG_TITLE(_T("MPT 2025 : "));
const static CString _DATA_FILE_EXT_(_T(".Mpt"));

const static CString MPT_EXT(_T(".Mpt"));

static wchar_t MPT_FILTER[] = _T("MPT 2025 Files(*.Mpt)|*.Mpt|All Files(*.*)|*.*||");

constexpr auto _DATA_FILE_FILTER_ = _T("MPT 2025 Files(*.Mpt)\0*.Mpt\0All Files(*.*)\0*.*\0\0");

constexpr auto INVALID_PM25VALUE = -1;

constexpr auto MAX_NEWS_NUM = 100;

constexpr auto MIN_NEWS_SCROLL_SPEED = 1;
constexpr auto MAX_NEWS_SCROLL_SPEED = 100;

constexpr auto MIN_NEWS_SCROLL_ITERATION = 1;
constexpr auto MAX_NEWS_SCROLL_ITERATION = INT_MAX;

constexpr auto MIN_NEWS_SCROLL_ITEM_GAP = 1;
constexpr auto MAX_NEWS_SCROLL_ITEM_GAP = 1920;

constexpr auto MIN_NEWS_SCROLL_PREFIX_GAP = 1;
constexpr auto MAX_NEWS_SCROLL_PREFIX_GAP = 1920;

constexpr auto MIN_NEWS_DISPLAY_DURATION = 5;
constexpr auto MAX_NEWS_DISPLAY_DURATION = 60;

constexpr auto MIN_WEATHER_DISPLAY_DURATION = 7;
constexpr auto MAX_WEATHER_DISPLAY_DURATION = 60;

constexpr auto MIN_NOTICE_REPETITION = 1;
constexpr auto MAX_NOTICE_REPETITION = INT_MAX;

constexpr auto MAX_ALIAS_NUM = 30;


constexpr auto  NUM_CITY = 1600;
constexpr auto  NUM_WEATHERDATA = 300;

const static CString XML_TAG_ITEMS(_T("Items"));
const static CString XML_TAG_ITEM(_T("Item"));
const static CString XML_TAG_TEXT(_T("Text"));
const static CString XML_TAG_IMAGE(_T("Image"));
const static CString XML_TAG_DEFAULT_FONT(_T("_Default_Font_"));

constexpr auto XML_TAG_DEFAULT_TEXT_COLOR = 0x33010101;
constexpr auto XML_TAG_DEFAULT_BACK_COLOR = 0x00000000;

const static CString  XML_ATTR_PREFIX(_T("Prefix"));
const static CString  XML_ATTR_USE(_T("Use"));
const static CString  XML_ATTR_SIZE(_T("Size"));
const static CString  XML_ATTR_FONT(_T("Font"));
const static CString  XML_ATTR_COLOR(_T("Color"));
const static CString  XML_ATTR_CHARSET(_T("CharSet"));
const static CString  XML_ATTR_SPEED(_T("Scroll_Speed"));
const static CString  XML_ATTR_ITERATION(_T("Scroll_Iteration"));
const static CString  XML_ATTR_ITEM_GAP(_T("Scroll_Item_Gap"));
const static CString  XML_ATTR_FRONT_MARGIN(_T("Scroll_Front_Margin"));
const static CString  XML_ATTR_BACK_MARGIN(_T("Scroll_Back_Margin"));
const static CString  XML_ATTR_PREFIX_GAP(_T("Scroll_Prefix_Gap"));
const static CString  XML_ATTR_FPG_FILENAME(_T("Scroll_Fpg_FileName"));
const static CString  XML_ATTR_USE_IMAGE_PREFIX(_T("UseImagePrefix"));
const static CString  XML_ATTR_USE_ITEM_PREFIX(_T("UseItemPrefix"));
const static CString  XML_ATTR_IMAGE_PREFIX_FILENAME(_T("ImagePrefixFileName"));
const static CString  XML_ATTR_PROGRAM_NAME(_T("ProgramName"));
const static CString  XML_ATTR_PROGRAM_TIME(_T("ProgramTime"));

const static CString DEFAULT_DATE_FORMAT(_T("%D (%A)"));

constexpr auto MAX_ERROR_NUM = 30;

enum { _HIDE_ = FALSE, _SHOW_ = TRUE};

enum { HANGUL_MODE = 0, ENGLISH_MODE };

enum class GET_POS
{
	YES = 0,
	NO
};

enum class DIRECTION  // 시계 반대 방향으로 설정
{
	UP = 0,
	LEFT,
	DOWN,
	RIGHT
};

enum class STATION_LOGO
{
	KBS_1 = 0,
	KBS_2,
	KBS_LOCAL,
	KBS_ANI

};

enum {
	IMAGE_SHORTCUT_1 = 0,
	IMAGE_SHORTCUT_2,
	IMAGE_SHORTCUT_3,
	IMAGE_SHORTCUT_4,
	IMAGE_SHORTCUT_5,
	IMAGE_SHORTCUT_6,
	IMAGE_SHORTCUT_NUM
};


enum class CLOCK_LAYERS {
	BASE,
	SEC,
	LAST_NUM
};

static CString clockLayers[FROM_E(CLOCK_LAYERS::LAST_NUM)] =
      { 
	    _T("기본레이어"),
	    _T("초레이어") 
      };

enum class CLOCK_OBJECTS {
	DATE,
	HOUR,
	MIN_COLON,
	TEN_MIN,
	MIN,
	SEC_COLON,
	TEN_SEC,
	SEC,
	LAST_NUM
};

static CString clockObjects[FROM_E(CLOCK_OBJECTS::LAST_NUM)] =
  {
	_T("날짜"),
	_T("시"),
	_T("분콜론"),
	_T("십분"),
	_T("분"),
	_T("초콜론"),
	_T("십초"),
	_T("초")
 };



enum class BIG_CLOCK_LAYERS {
	BASE,
	LAST_NUM
};

static CString bigClockLayers[FROM_E(BIG_CLOCK_LAYERS::LAST_NUM)] =
{
  _T("기본레이어")
};

enum class BIG_CLOCK_OBJECTS {
	HOUR,
	MIN_COLON,
	TEN_MIN,
	MIN,
	SEC_COLON,
	TEN_SEC,
	SEC,
	LAST_NUM
};

static CString bigClockObjects[FROM_E(BIG_CLOCK_OBJECTS::LAST_NUM)] =
{
  _T("시"),
  _T("분콜론"),
  _T("십분"),
  _T("분"),
  _T("초콜론"),
  _T("십초"),
  _T("초")
};

enum class LOGO_OBJECTS {
	LOGO,
	LIVE,
	UHD,
	DONGSI,
	LAST_NUM
};

static CString logoObjects[FROM_E(LOGO_OBJECTS::LAST_NUM)]{
	_T("로고"),
	_T("Live"),
	_T("UHD"),
	_T("동시방송")
};

enum class NEWS_LAYERS {
	CONTENTS,
	CONTENTS1,
	FIXED_CATEGORY,
	MOVING_CATEGORY,
	CATEGORY_EFFECT,
	SCROLL,
	LAST_NUM
};

static CString newsLayers[FROM_E(NEWS_LAYERS::LAST_NUM)] =
{
  _T("ContentsLayer"),
  _T("Contents1Layer"),
  _T("Fixed_Category_Layer"),
  _T("Moving_Category_Layer"),
  _T("Category_Effect_Layer"),
  _T("ScrollLayer") 
};

enum class NEWS_EFFECTS {
	SCROLL,
	LAST_NUM
};

static CString newsEffects[FROM_E(NEWS_EFFECTS::LAST_NUM)] = { _T("ScrollEffect") };

enum class NEWS_OBJECTS {
	CONTENTS,
	CONTENTS1,
	FIXED_CATEGORY,
	FIXED_CATEGORY_BAND,
	MOVING_CATEGORY,
	MOVING_CATEGORY_BAND,
	CATEGORY_EFFECT,
	BAND,
	PREFIX,
	LAST_NUM
};

static CString newsObjects[FROM_E(NEWS_OBJECTS::LAST_NUM)] =
{
  _T("contents"),
  _T("contents1"),
  _T("Fixed_Category"),
  _T("Fixed_Category_Band"),
  _T("Moving_Category"),
  _T("Moving_Category_Band"),
  _T("Category_Effect"),
  _T("Band"),
  _T("Prefix")
};


enum class WEATHER_LAYERS {
	WEATHER1,
	WEATHER2,
	DUST,
	LAST_NUM
};

static CString weatherLayers[FROM_E(WEATHER_LAYERS::LAST_NUM)] =
{
  _T("날씨레이어1"),
  _T("날씨레이어2"),
  _T("먼지레이어")
};

enum class WEATHER_OBJECTS {
	WEATHER_CITYNAME_1,
	WEATHER_ICON_1,
	WEATHER_TEMP_1,
	WEATHER_TEMP_SYMBOL_1,

	WEATHER_CITYNAME_2,
	WEATHER_ICON_2,
	WEATHER_TEMP_2,
	WEATHER_TEMP_SYMBOL_2,

	DUST_CITYNAME,
	DUST_GRADE,
	DUST_CONCENTRATION,
	DUST_SYMBOL,

	LAST_NUM
};

static CString weatherObjects[FROM_E(WEATHER_OBJECTS::LAST_NUM)] =
{
  _T("날씨_지역명1"),
  _T("날씨_아이콘1"),
  _T("날씨_온도1"),
  _T("날씨_온도기호1"),

  _T("날씨_지역명2"),
  _T("날씨_아이콘2"),
  _T("날씨_온도2"),
  _T("날씨_온도기호2"),

  _T("먼지_지역명"),
  _T("먼지_등급"),
  _T("먼지_농도"),
  _T("먼지_농도기호")
};


enum class AIR_LEVEL
{
	NONE = 0,
	GOOD = 1,
	NOMAL = 2,
	BAD = 3,
	VERY_BAD = 4,
	LAST_NUM
};

static CString strAirLevel[FROM_E(AIR_LEVEL::LAST_NUM)] = {
   _T(""),
   _T("좋음"),
   _T("보통"),
   _T("나쁨"),
   _T("매우나쁨")
};


static CString strWeatherName[24] = {
	_T(""),
	_T("맑음"),
	_T("구름조금과 해"),
	_T(""),
	_T("구름많음"),
	_T("연무"),
	_T("황사"),
	_T("먼지"),
	_T("강풍"),
	_T("안개"),
	_T("번개"),
	_T("비"),
	_T("소나기"),
	_T("용오름"),
	_T("눈"),
	_T("진눈깨비"),
	_T("우박"),
	_T("많은비"),
	_T("많은눈"),
	_T("비 비구름 번개"),
	_T("눈 구름 번개"),
	_T("박무"),
	_T("흐림"),
	_T("진눈깨비")
};

static ULONG ulAirLevelColor[FROM_E(AIR_LEVEL::LAST_NUM)] = {
	RGBToULONG(RGB(0x00,0x00,0x00)),
	RGBToULONG(RGB(35,89,196)),
	RGBToULONG(RGB(1,181,110)),
	RGBToULONG(RGB(245,201,50)),
	RGBToULONG(RGB(218,53,57))
};

enum GetWeatherResult
{
	FAIL = 0,
	SUCCESS,
	PARTIAL
};

enum AGE_MODE
{
	AGE_ALL=0,
	AGE_7,
	AGE_12,
	AGE_15,
	AGE_19,
	LAST_NUM
};


enum class AGE_LAYERS {
	AGE,
	LAST_NUM
};

static CString AgeLayers[FROM_E(AGE_LAYERS::LAST_NUM)] =
{ _T("연령레이어") };

enum class AGE_OBJECTS {
	AGE,
	LAST_NUM
};

static CString AgeObjects[FROM_E(AGE_OBJECTS::LAST_NUM)] =
{
  _T("연령"),
 };

enum class LIVE_VIDEO_LAYERS {
	LIVE_INPUT_VIDEO,
	LAST_NUM
};

static CString LiveVideoLayers[FROM_E(LIVE_VIDEO_LAYERS::LAST_NUM)] =
{ 
	_T("입력비디오레이어") 
};

enum class LIVE_VIDEO_OBJECTS {
	EXPRESSION_AREA,
	MASK,
	LAST_NUM
};

static CString  LiveVideoObjects[FROM_E(LIVE_VIDEO_OBJECTS::LAST_NUM)] =
{
  _T("표출영역"),
  _T("마스크"),
};

enum class ALIAS_TYPE
{
	NONE,
	TEXT,
	IMAGE,
	LAST_NUM
};

constexpr auto MANUALUP_PAGE_NUMBER = 0;
constexpr auto NEWS_PAGE_NUMBER = 1;
constexpr auto WEATHER_PAGE_NUMBER = 2;
constexpr auto CLOCK_PAGE_NUMBER = 3;
constexpr auto LOGO_PAGE_NUMBER = 4;
constexpr auto NOTICE_PAGE_NUMBER = 5;
constexpr auto AGE_PAGE_NUMBER = 6;
constexpr auto LIVE_VIDEO_PAGE_NUMBER = 7;

enum {
	DISP_MANUALUP = 0,

	DISP_NEWS,
	DISP_NEWS_SCROLL,

	DISP_WEATHER,

	DISP_BIG_CLOCK,
	DISP_UPPER_CLOCK,

	DISP_STATION_LOGO,

	DISP_NOTICE,

	DISP_AGE,
	DISP_AGE_NOTICE,

	DISP_LIVE_VIDEO,
	
	MAX_DISP_MENU_NUM
};

enum {
	GYEONGGIDO_ENTIRE = 0,

	SUWON_SI, // 경기도
	SEONGNAM_SI,
	BUCHEON_SI,
	ANYANG_SI,
	ANSAN_SI,
	YONGIN_SI,
	PYEONGTAEK_SI,
	GWANGMYEONG_SI,
	SIHEUNG_SI,
	GUNPO_SI,
	HWASEONG_SI,
	ICHEON_SI,
	GIMPO_SI,
	GWANGJU_SI,
	ANSEONG_SI,
	HANAM_SI,
	UIWANG_SI,
	OSAN_SI,
	GWACHEON_SI,
	YEOJU_GUN,
	YANGPYEONG_GUN,
	GOYANG_SI,
	UIJEONGBU_SI,
	NAMYANGJU_SI,
	PAJU_SI,
	GURI_SI,
	POCHEON_SI,
	YANGJU_SI,
	DONGDUCHEON_SI,
	GAPYEONG_GUN,
	YEONCHEON_GUN,

	GANGWONDO_ENTIRE = 100,
	CHUNCHEON_SI,     // 강원도
	GANGNEUNG_SI,
	WONJU_SI,
	DONGHAE_SI,
	SAMCHEOK_SI,
	SOKCHO_SI,
	TAEBAEK_SI,
	CHEORWON_GUN,
	HWACHEON_GUN,
	HOENGSEONG_GUN,
	YANGGU_GUN,
	YEONGWOL_GUN,
	INJE_GUN,
	PYEONGCHANG_GUN,
	GOSEONG_GUN_GANGWON_DO,
	JEONGSEON_GUN,
	YANGYANG_GUN,
	HONGCHEON_GUN,

	CHUNGCHEONGBUKDO_ENTIRE = 200, // 충청북도
	CHEONGJU_SI,
	CHUNGJU_SI,
	JECHEON_SI,
	CHEONGWON_GUN,
	BOEUN_GUN,
	OKCHEON_GUN,
	YEONGDONG_GUN,
	JINCHEON_GUN,
	GOESAN_GUN,
	EUMSEONG_GUN,
	DANYANG_GUN,
	JEUNGPYEONG_GUN,

	CHUNGCHEONGNAMDO_ENTIRE = 300, // 충청남도
	CHEONAN_SI,
	GONGJU_SI,
	BORYEONG_SI,
	ASAN_SI,
	SEOSAN_SI,
	NONSAN_SI,
	SEJONG_SI,
	BUYEO_GUN,
	SEOCHEON_GUN,
	CHEONGYANG_GUN,
	HONGSEONG_GUN,
	YESAN_GUN,
	TAEAN_GUN,
	DANGJIN_SI,
	GEUMSAN_GUN,
	GYERYONG_SI,

	JEOLLABUKDO_ENTIRE = 400, // 전라남도 
	JEONJU_SI,
	GUNSAN_SI,
	IKSAN_SI,
	JEONGEUP_SI,
	NAMWON_SI,
	GIMJE_SI,
	WANJU_GUN,
	JINAN_GUN,
	MUJU_GUN,
	JANGSU_GUN,
	IMSIL_GUN,
	SUNCHANG_GUN,
	GOCHANG_GUN,
	BUAN_GUN,

	JEOLLANAMDO_ENTIRE = 500, // 전라북도
	MOKPO_SI,
	YEOSU_SI,
	SUNCHEON_SI,
	NAJU_SI,
	GWANGYANG_SI,
	DAMYANG_GUN,
	GOKSEONG_GUN,
	BOSEONG_GUN,
	GURYE_GUN,
	GOHEUNG_GUN,
	HWASUN_GUN,
	JANGHEUNG_GUN,
	GANGJIN_GUN,
	HAENAM_GUN,
	YEONGAM_GUN,
	MUAN_GUN,
	HAMPYEONG_GUN,
	YEONGGWANG_GUN,
	JANGSEONG_GUN,
	WANDO_GUN,
	JINDO_GUN,
	SINAN_GUN,
	HEUKSAN_MYEON,

	GYEONGSANGBUKDO_ENTIRE = 600, // 경상북도
	POHANG_SI,
	GYEONGJU_SI,
	GIMCHEON_SI,
	ANDONG_SI,
	GUMI_SI,
	YEONGJU_SI,
	YEONGCHEON_SI,
	SANGJU_SI,
	MUNGYEONG_SI,
	GYEONGSAN_SI,
	GUNWI_GUN,
	CHILGOK_GUN,
	UISEONG_GUN,
	YECHEON_GUN,
	CHEONGSONG_GUN,
	BONGHWA_GUN,
	YEONGYANG_GUN,
	ULJIN_GUN,
	YEONGDEOK_GUN,
	ULLEUNG_GUN,
	CHEONGDO_GUN,
	GORYEONG_GUN,
	SEONGJU_GUN,
	DOKDO_RI,

	GYEONGSANGNAMDO_ENTIRE = 700, // 경상남도
	CHANGWON_SI,
	JINJU_SI,
	TONGYEONG_SI,
	SACHEON_SI,
	GIMHAE_SI,
	MIRYANG_SI,
	YANGSAN_SI,
	GEOJE_SI,
	UIRYEONG_GUN,
	GOSEONG_GUN,
	HAMAN_GUN,
	NAMHAE_GUN,
	CHANGNYEONG_GUN,
	HADONG_GUN,
	SANCHEONG_GUN,
	HAMYANG_GUN,
	GEOCHANG_GUN,
	HAPCHEON_GUN,

	JEJUDO_ENTIRE = 800, //제주도
	JEJU_SI,
	SEOGWIPO_SI,
	HALLIM_EUP,
	CHUJA_MYEON,
	SEONGSAN_EUP,

	SEOUL = 900, // 서울시
	BUSAN = 1000, // 부산시
	INCHEON = 1100, // 인천시
	DAEJEON = 1200, // 대전시 
	GWANGJU = 1300, // 광주시
	DAEGU = 1400, // 대구시
	ULSAN = 1500, // 울산시

	ENTIRE_MAJOR_CITIES = 1598,
	ALL_CITIES = 1599
};


enum {
	SCROLL_LAYER_ALIAS = 0,
	CONTENTS_LAYER_ALIAS,
	CONTENTS1_LAYER_ALIAS,
	SCROLL_EFFECT_ALIAS,
	PREFIX_ALIAS,
	CONTENTS_ALIAS,
	BAND_ALIAS,
	ALIAS_NUM
};

enum {
	ALIAS_CHECK_RETURN_CODE_NORMAL,
	ALIAS_CHECK_RETURN_CODE_WARNING,
	ALIAS_CHECK_RETURN_CODE_CRITICAL,
	ALIAS_CHECK_RETURN_CODE_NUM
};




namespace ns_News {
	enum NEWS_FIELD_INDEX
	{
		_FIELD_PRIMARY_KEY_INDEX_ = 0,
		_FIELD_ROW_NUM_INDEX_,
		USE = 2,
		CATEGORY,
		CONTENTS,
		SIZES,
		COLORS,
		FONTS,
		CHARSETS,
		MAX_NEWS_FILED_INDEX
	};

	static std::vector<CString> vNews_Field_Name =
	{
		_PRIMARY_KEY_FIELD_NAME_,
		_ROW_NUM_FIELD_NAME_,
		_T("Use"),
		_T("Category"),
		_T("Contents"),
		_T("Sizes"),
		_T("Colors"),
		_T("Fonts"),
		_T("CharSets")

	};


	static std::vector<CString> vNews_Field_Title =
	{
		_PRIMARY_KEY_FIELD_NAME_,
		_ROW_NUM_FIELD_NAME_,
		_T("사용"),
		_T("카테고리"),
		_T("내용"),
		_T("크기"),
		_T("색상"),
		_T("폰트"),
		_T("CharSet")
	};

	static std::vector<CFieldInfo> vNews_Field =
	{
		PRIMARY_KEY_FIELD,
		ROW_NUM_FIELD,
		CFieldInfo(vNews_Field_Name[NEWS_FIELD_INDEX::USE], FI_DATA_TYPE::_BOOL_TYPE_,
				   vNews_Field_Title[NEWS_FIELD_INDEX::USE],CELL_TYPE::_CHECK_TYPE_, 5, _CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vNews_Field_Name[NEWS_FIELD_INDEX::CATEGORY], FI_DATA_TYPE::_STRING_TYPE_,
				   vNews_Field_Title[NEWS_FIELD_INDEX::CATEGORY],CELL_TYPE::_COMBOBOX_TYPE_ ,20, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vNews_Field_Name[NEWS_FIELD_INDEX::CONTENTS], FI_DATA_TYPE::_STRING_TYPE_,
				   vNews_Field_Title[NEWS_FIELD_INDEX::CONTENTS],CELL_TYPE::_READONLY_TYPE_ , 90, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vNews_Field_Name[NEWS_FIELD_INDEX::SIZES], FI_DATA_TYPE::_STRING_TYPE_,
				   vNews_Field_Title[NEWS_FIELD_INDEX::SIZES],CELL_TYPE::_NORMAL_TYPE_, 0, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_HIDE_),

		CFieldInfo(vNews_Field_Name[NEWS_FIELD_INDEX::COLORS], FI_DATA_TYPE::_STRING_TYPE_,
				   vNews_Field_Title[NEWS_FIELD_INDEX::COLORS],CELL_TYPE::_NORMAL_TYPE_, 0, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_HIDE_),

		CFieldInfo(vNews_Field_Name[NEWS_FIELD_INDEX::FONTS], FI_DATA_TYPE::_STRING_TYPE_,
				   vNews_Field_Title[NEWS_FIELD_INDEX::FONTS],CELL_TYPE::_NORMAL_TYPE_,  0, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_HIDE_),

		CFieldInfo(vNews_Field_Name[NEWS_FIELD_INDEX::CHARSETS], FI_DATA_TYPE::_STRING_TYPE_,
				   vNews_Field_Title[NEWS_FIELD_INDEX::CHARSETS], CELL_TYPE::_NORMAL_TYPE_, 0, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_HIDE_),

	};

}

namespace ns_Weather
{
	enum WEATHER_FIELD_INDEX
	{
		_FIELD_PRIMARY_KEY_INDEX_ = 0,
		_FIELD_ROW_NUM_INDEX_,
		HJD_CODE,
		CITY_CODE,
		CITY_NAME,
		WEATHER_CODE,
		WEATHER_NAME,
		WEATHER_TEMP,
		PM25AVG,
		PM25GRADE,
		MAX_WEATHER_DATA_FILED_INDEX
	};

	static std::vector<CString> vWeather_Field_Name = {
		_PRIMARY_KEY_FIELD_NAME_,
		_ROW_NUM_FIELD_NAME_,
		_T("HjdCode"),
		_T("CityCode"),
		_T("CityName"),
		_T("WeatherCode"),
		_T("WeatherName"),
		_T("WeatherTemp"),
		_T("Pm25Avg"),
		_T("Pm25Grade")
	};


	static std::vector<CString> vWeather_Field_Title = {
		_PRIMARY_KEY_FIELD_NAME_,
		_ROW_NUM_FIELD_NAME_,
		_T("코드"),
		_T("도시코드"),
		_T("시도명"),
		_T("아이콘명"),
		_T("날씨"),
		_T("온도"),
		_T("초미세먼지"),
		_T("초미세먼지등급"),
	};


	static std::vector <CFieldInfo> vWeather_Field =
	{
		PRIMARY_KEY_FIELD,
		ROW_NUM_FIELD,
		CFieldInfo(vWeather_Field_Name[HJD_CODE],FI_DATA_TYPE::_STRING_TYPE_,
				   vWeather_Field_Title[HJD_CODE],CELL_TYPE::_READONLY_TYPE_,0,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_HIDE_),

		CFieldInfo(vWeather_Field_Name[CITY_CODE],FI_DATA_TYPE::_STRING_TYPE_,
				   vWeather_Field_Title[CITY_CODE],CELL_TYPE::_READONLY_TYPE_,0,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_HIDE_),

		CFieldInfo(vWeather_Field_Name[CITY_NAME],FI_DATA_TYPE::_STRING_TYPE_,
				   vWeather_Field_Title[CITY_NAME],CELL_TYPE::_READONLY_TYPE_,5,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vWeather_Field_Name[WEATHER_CODE],FI_DATA_TYPE::_STRING_TYPE_,
				   vWeather_Field_Title[WEATHER_CODE],CELL_TYPE::_NORMAL_TYPE_,6,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vWeather_Field_Name[WEATHER_NAME],FI_DATA_TYPE::_STRING_TYPE_,
				   vWeather_Field_Title[WEATHER_NAME],CELL_TYPE::_READONLY_TYPE_,10,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vWeather_Field_Name[WEATHER_TEMP],FI_DATA_TYPE::_FLOAT_TYPE_,
				   vWeather_Field_Title[WEATHER_TEMP],CELL_TYPE::_NORMAL_TYPE_,5,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vWeather_Field_Name[PM25AVG],FI_DATA_TYPE::_FLOAT_TYPE_,
				   vWeather_Field_Title[PM25AVG],CELL_TYPE::_NORMAL_TYPE_,6,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_SHOW_),

		CFieldInfo(vWeather_Field_Name[PM25GRADE],FI_DATA_TYPE::_STRING_TYPE_,
				   vWeather_Field_Title[PM25GRADE],CELL_TYPE::_READONLY_TYPE_,9,_CELL_DEFAULT_HEIGHT_,CELL_SHOW::_CELL_SHOW_),
	};

}
namespace ns_Notice
{
	enum NOTICE_FIELD_INDEX
	{
		_FIELD_PRIMARY_KEY_INDEX_ = 0,
		_FIELD_ROW_NUM_INDEX_,
		USE,
		TEMPLATE,
		ANNOTATION,

		// Alias Fields
		ALIAS_1, ALIAS_2, ALIAS_3, ALIAS_4, ALIAS_5,
		ALIAS_6, ALIAS_7, ALIAS_8, ALIAS_9, ALIAS_10,
		ALIAS_11, ALIAS_12, ALIAS_13, ALIAS_14, ALIAS_15,
		ALIAS_16, ALIAS_17, ALIAS_18, ALIAS_19, ALIAS_20,
		ALIAS_21, ALIAS_22, ALIAS_23, ALIAS_24, ALIAS_25,
		ALIAS_26, ALIAS_27, ALIAS_28, ALIAS_29, ALIAS_30,

		// Value Fields
		VALUE_1, VALUE_2, VALUE_3, VALUE_4, VALUE_5,
		VALUE_6, VALUE_7, VALUE_8, VALUE_9, VALUE_10,
		VALUE_11, VALUE_12, VALUE_13, VALUE_14, VALUE_15,
		VALUE_16, VALUE_17, VALUE_18, VALUE_19, VALUE_20,
		VALUE_21, VALUE_22, VALUE_23, VALUE_24, VALUE_25,
		VALUE_26, VALUE_27, VALUE_28, VALUE_29, VALUE_30,

		// Type Fields
		TYPE_1, TYPE_2, TYPE_3, TYPE_4, TYPE_5,
		TYPE_6, TYPE_7, TYPE_8, TYPE_9, TYPE_10,
		TYPE_11, TYPE_12, TYPE_13, TYPE_14, TYPE_15,
		TYPE_16, TYPE_17, TYPE_18, TYPE_19, TYPE_20,
		TYPE_21, TYPE_22, TYPE_23, TYPE_24, TYPE_25,
		TYPE_26, TYPE_27, TYPE_28, TYPE_29, TYPE_30,

		LAST_NUM
	};

	static std::vector<CString> vNotice_Field_Name;
	static std::vector<CString> vNotice_Field_Title;
	static std::vector<CFieldInfo> vNotice_Field;

	enum class FieldCategory { ALIAS, VALUE, TYPE };

	static CString MakeFieldName(FieldCategory category, int index)
	{
		switch (category)
		{
		case FieldCategory::ALIAS: return CString(_T("Alias") + GETSTR(index));
		case FieldCategory::VALUE: return CString(_T("Value") + GETSTR(index));
		case FieldCategory::TYPE:  return CString(_T("Type") + GETSTR(index));
		default: return _T("");
		}
	}

	static CString MakeFieldTitle(FieldCategory category, int index)
	{
		switch (category)
		{
		case FieldCategory::ALIAS: return CString(_T("별칭") + GETSTR(index));
		case FieldCategory::VALUE: return CString(_T("값") + GETSTR(index));
		case FieldCategory::TYPE:  return CString(_T("타입") + GETSTR(index));
		default: return _T("");
		}
	}

	static void AddFieldRange(int startEnumIndex, int count, FieldCategory category)
	{
		for (int i = 0; i < count; ++i)
		{
			int fieldIndex = startEnumIndex + i;
			vNotice_Field_Name.emplace_back(MakeFieldName(category, i + 1));
			vNotice_Field_Title.emplace_back(MakeFieldTitle(category, i + 1));
			vNotice_Field.emplace_back(CFieldInfo(
				vNotice_Field_Name[fieldIndex], FI_DATA_TYPE::_STRING_TYPE_,
				vNotice_Field_Title[fieldIndex], CELL_TYPE::_NORMAL_TYPE_,
				0, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_HIDE_));
		}
	}

	static void InitNoticeFieldVectors()
	{
		vNotice_Field_Name.clear();
		vNotice_Field_Title.clear();
		vNotice_Field.clear();

		vNotice_Field_Name.emplace_back(_PRIMARY_KEY_FIELD_NAME_);
		vNotice_Field_Name.emplace_back(_ROW_NUM_FIELD_NAME_);
		vNotice_Field_Name.emplace_back(_T("Use"));
		vNotice_Field_Name.emplace_back(_T("Template"));
		vNotice_Field_Name.emplace_back(_T("Annotaion"));

		vNotice_Field_Title.emplace_back(_PRIMARY_KEY_FIELD_NAME_);
		vNotice_Field_Title.emplace_back(_ROW_NUM_FIELD_NAME_);
		vNotice_Field_Title.emplace_back(_T("사용"));
		vNotice_Field_Title.push_back(_T("템플릿"));
		vNotice_Field_Title.emplace_back(_T("데이터"));

		vNotice_Field.emplace_back(PRIMARY_KEY_FIELD);
		vNotice_Field.emplace_back(ROW_NUM_FIELD);
		vNotice_Field.emplace_back(CFieldInfo(vNotice_Field_Name[USE], FI_DATA_TYPE::_INT_TYPE_, vNotice_Field_Title[USE], CELL_TYPE::_CHECK_TYPE_, 5, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_SHOW_));
		vNotice_Field.emplace_back(CFieldInfo(vNotice_Field_Name[TEMPLATE], FI_DATA_TYPE::_STRING_TYPE_, vNotice_Field_Title[TEMPLATE], CELL_TYPE::_READONLY_TYPE_, 20, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_SHOW_));
		vNotice_Field.emplace_back(CFieldInfo(vNotice_Field_Name[ANNOTATION], FI_DATA_TYPE::_STRING_TYPE_, vNotice_Field_Title[ANNOTATION], CELL_TYPE::_NORMAL_TYPE_, 35, _CELL_DEFAULT_HEIGHT_, CELL_SHOW::_CELL_SHOW_));

		

		AddFieldRange(ALIAS_1, MAX_ALIAS_NUM, FieldCategory::ALIAS);
		AddFieldRange(VALUE_1, MAX_ALIAS_NUM, FieldCategory::VALUE);
		AddFieldRange(TYPE_1, MAX_ALIAS_NUM, FieldCategory::TYPE);
	}
} // namespace ns_Notice

using namespace ns_News;
using namespace ns_Weather;
using namespace ns_Notice;

////////////////////////////////////////////////////////////////////////////////////
// CClockDataObject 명령 대상입니다.

class CClockDataObject : public CParentObject
{
	DECLARE_DYNAMIC(CClockDataObject)

public:
	CClockDataObject() {}

	virtual ~CClockDataObject() {}

	virtual void Serialize(CArchive& ar)
	{
		CParentObject::Serialize(ar);
	}
};

/////////////////////////////////////////////////////////////////////
//////////////////// CTimeString ///////////////////////////////////

class CTimeString : public CClockDataObject
{
	DECLARE_DYNAMIC(CTimeString)

public:
	CTime   m_tmSystem;
	CString m_strYear;
	CString m_strMonth;
	CString m_strDay;
	CString m_strDayOfWeek;
	CString m_strPlus;
	CString m_strHour10;
	CString m_strHour1;
	CString m_strMinColon;
	CString m_strMin10;
	CString m_strMin1;
	CString m_strSecColon;
	CString m_strSec10;
	CString m_strSec1;

	std::array<CString, 7> DAYOFWEEK;

public:
	CTimeString() :
		m_strYear(BLANK_STRING),
		m_strMonth(BLANK_STRING),
		m_strDay(BLANK_STRING),
		m_strDayOfWeek(BLANK_STRING),
		m_strPlus(BLANK_STRING),
		m_strHour10(BLANK_STRING),
		m_strHour1(BLANK_STRING),
		m_strMinColon(BLANK_STRING),
		m_strMin10(BLANK_STRING),
		m_strMin1(BLANK_STRING),
		m_strSecColon(BLANK_STRING),
		m_strSec10(BLANK_STRING),
		m_strSec1(BLANK_STRING)
	{
		m_tmSystem = CTime::GetCurrentTime();
		DAYOFWEEK = { _T("일"),_T("월"),_T("화"),_T("수"),_T("목"),_T("금"),_T("토") };
	}

	CTimeString(const CTimeString& rhs) :
		m_tmSystem(rhs.m_tmSystem),
		m_strYear(rhs.m_strYear),
		m_strMonth(rhs.m_strMonth),
		m_strDay(rhs.m_strDay),
		m_strDayOfWeek(rhs.m_strDayOfWeek),
		m_strPlus(rhs.m_strPlus),
		m_strHour10(rhs.m_strHour10),
		m_strHour1(rhs.m_strHour1),
		m_strMinColon(rhs.m_strMinColon),
		m_strMin10(rhs.m_strMin10),
		m_strMin1(rhs.m_strMin1),
		m_strSecColon(rhs.m_strSecColon),
		m_strSec10(rhs.m_strSec10),
		m_strSec1(rhs.m_strSec1)
	{
		DAYOFWEEK = { _T("일"),_T("월"),_T("화"),_T("수"),_T("목"),_T("금"),_T("토") };
	}

	~CTimeString() {}

public:
	CTimeString& GetCurrentTimeString(BOOL bResync = TRUE, BOOL b24Hour = TRUE)
	{
		CString strClockTime;

		if (bResync)
		{
			m_tmSystem = CTime::GetCurrentTime();
		}

		if (b24Hour)
		{
			strClockTime = m_tmSystem.Format(_T("%Y %m %d %H %M %S"));
		}
		else
		{
			strClockTime = m_tmSystem.Format(_T("%Y %m %d %I %M %S"));
		}

		this->m_strYear = strClockTime.Mid(0, 4).TrimLeft(ZERO_STRING);
		this->m_strMonth = strClockTime.Mid(5, 2).TrimLeft(ZERO_STRING);
		this->m_strDay = strClockTime.Mid(8, 2).TrimLeft(ZERO_STRING);

		this->m_strDayOfWeek = DAYOFWEEK[m_tmSystem.GetDayOfWeek() - 1];

		this->m_strHour10 = strClockTime.Mid(11, 1);//.TrimLeft(ZERO_STRING);
		this->m_strHour1 = strClockTime.Mid(12, 1);
		this->m_strMin10 = strClockTime.Mid(14, 1);
		this->m_strMin1 = strClockTime.Mid(15, 1);
		this->m_strSec10 = strClockTime.Mid(17, 1);
		this->m_strSec1 = strClockTime.Mid(18, 1);

		return *this;

	}

	void Resync(void) { m_tmSystem = CTime::GetCurrentTime(); }

	CString GetString(BOOL bResync = TRUE)
	{
		CString strClock;

		if (bResync)
		{
			m_tmSystem = CTime::GetCurrentTime();
		}

		strClock = m_tmSystem.Format(_T("%Y")) + _T("년")
			+ m_tmSystem.Format(_T("%#m")) + _T("월")
			+ m_tmSystem.Format(_T("%#d")) + _T("일")
			+ _T(" (") + DAYOFWEEK[m_tmSystem.GetDayOfWeek() - 1] + _T(")")
			+ m_tmSystem.Format(_T(" %#H:%M:%S"));

		return strClock;
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strYear;
			ar << m_strMonth;
			ar << m_strDay;
			ar << m_strDayOfWeek;
			ar << m_strPlus;
			ar << m_strHour10;
			ar << m_strHour1;
			ar << m_strMinColon;
			ar << m_strMin10;
			ar << m_strMin1;
			ar << m_strSecColon;
			ar << m_strSec10;
			ar << m_strSec1;

		}
		else
		{
			ar >> m_strYear;
			ar >> m_strMonth;
			ar >> m_strDay;
			ar >> m_strDayOfWeek;
			ar >> m_strPlus;
			ar >> m_strHour10;
			ar >> m_strHour1;
			ar >> m_strMinColon;
			ar >> m_strMin10;
			ar >> m_strMin1;
			ar >> m_strSecColon;
			ar >> m_strSec10;
			ar >> m_strSec1;
		}
		m_tmSystem.Serialize64(ar);
	}

	bool IsEmpty() override
	{
		CTimeString InitialData;
		if (*this == InitialData)
			return TRUE;
		else
			return FALSE;
	}

	const CTimeString& operator= (const CTimeString& rhs)
	{
		m_tmSystem = rhs.m_tmSystem;
		m_strYear = rhs.m_strYear;
		m_strMonth = rhs.m_strMonth;
		m_strDay = rhs.m_strDay;
		m_strDayOfWeek = rhs.m_strDayOfWeek;
		m_strPlus = rhs.m_strPlus;
		m_strHour10 = rhs.m_strHour10;
		m_strHour1 = rhs.m_strHour1;
		m_strMinColon = rhs.m_strMinColon;
		m_strMin10 = rhs.m_strMin10;
		m_strMin1 = rhs.m_strMin1;
		m_strSecColon = rhs.m_strSecColon;
		m_strSec10 = rhs.m_strSec10;
		m_strSec1 = rhs.m_strSec1;

		return *this;
	};

	const BOOL operator== (const CTimeString& rhs)
	{
		if (m_tmSystem != rhs.m_tmSystem) return FALSE;
		if (m_strYear != rhs.m_strYear) return FALSE;
		if (m_strMonth != rhs.m_strMonth) return FALSE;
		if (m_strDay != rhs.m_strDay) return FALSE;
		if (m_strDayOfWeek != rhs.m_strDayOfWeek) return FALSE;
		if (m_strPlus != rhs.m_strPlus) return FALSE;
		if (m_strHour10 != rhs.m_strHour10) return FALSE;
		if (m_strHour1 != rhs.m_strHour1) return FALSE;
		if (m_strMinColon != rhs.m_strMinColon) return FALSE;
		if (m_strMin10 != rhs.m_strMin10) return FALSE;
		if (m_strMin1 != rhs.m_strMin1) return FALSE;
		if (m_strSecColon != rhs.m_strSecColon) return FALSE;
		if (m_strSec10 != rhs.m_strSec10) return FALSE;
		if (m_strSec1 != rhs.m_strSec1) return FALSE;

		return TRUE;
	}
	const BOOL operator!= (const CTimeString& rhs)
	{
		return !(*this == rhs);
	}

	friend const BOOL operator== (const CTimeString& lhs, const CTimeString& rhs)
	{
		if (lhs.m_tmSystem != rhs.m_tmSystem) return FALSE;
		if (lhs.m_strYear != rhs.m_strYear) return FALSE;
		if (lhs.m_strMonth != rhs.m_strMonth) return FALSE;
		if (lhs.m_strDay != rhs.m_strDay) return FALSE;
		if (lhs.m_strDayOfWeek != rhs.m_strDayOfWeek) return FALSE;
		if (lhs.m_strPlus != rhs.m_strPlus) return FALSE;
		if (lhs.m_strHour10 != rhs.m_strHour10) return FALSE;
		if (lhs.m_strHour1 != rhs.m_strHour1) return FALSE;
		if (lhs.m_strMinColon != rhs.m_strMinColon) return FALSE;
		if (lhs.m_strMin10 != rhs.m_strMin10) return FALSE;
		if (lhs.m_strMin1 != rhs.m_strMin1) return FALSE;
		if (lhs.m_strSecColon != rhs.m_strSecColon) return FALSE;
		if (lhs.m_strSec10 != rhs.m_strSec10) return FALSE;
		if (lhs.m_strSec1 != rhs.m_strSec1) return FALSE;

		return TRUE;
	};
	friend const BOOL operator!= (const CTimeString& lhs, const CTimeString& rhs)
	{
		return !(lhs == rhs);
	}
};

class CCharInfo : public CClockDataObject
{
	DECLARE_DYNAMIC(CCharInfo)

public:
	CCharInfo()
	{
		Char(BLANK_STRING);
		Size(0);
		Color(0);
		Font(BLANK_STRING);
		CharSet(129);	
	}

	// 복사 생성자 (Copy constructor)
	CCharInfo(const CCharInfo& rhs) noexcept
		: CClockDataObject(rhs)  // 부모 클래스 복사 생성자 호출
		, m_strChar(rhs.m_strChar)
		, m_nSize(rhs.m_nSize)
		, m_ulColor(rhs.m_ulColor)
		, m_strFont(rhs.m_strFont)
		, m_btCharSet(rhs.m_btCharSet)
	{
	}

	// 이동 생성자 (Move constructor)
	CCharInfo(CCharInfo&& rhs) noexcept
		: CClockDataObject(std::move(rhs))  // 부모 클래스 이동 생성자 호출
		, m_strChar(std::move(rhs.m_strChar))
		, m_nSize(rhs.m_nSize)
		, m_ulColor(rhs.m_ulColor)
		, m_strFont(std::move(rhs.m_strFont))
		, m_btCharSet(rhs.m_btCharSet)
	{
	}

	// 복사 대입 연산자 (Copy assignment operator)
	CCharInfo& operator=(const CCharInfo& rhs) noexcept
	{
		if (this != &rhs)
		{
			CClockDataObject::operator=(rhs);  // 부모 클래스 복사 대입 연산자 호출
			m_strChar = rhs.m_strChar;
			m_nSize = rhs.m_nSize;
			m_ulColor = rhs.m_ulColor;
			m_strFont = rhs.m_strFont;
			m_btCharSet = rhs.m_btCharSet;
		}
		return *this;
	}

	// 이동 대입 연산자 (Move assignment operator)
	CCharInfo& operator=(CCharInfo&& rhs) noexcept
	{
		if (this != &rhs)
		{
			CClockDataObject::operator=(std::move(rhs));  // 부모 클래스 이동 대입 연산자 호출
			m_strChar = std::move(rhs.m_strChar);
			m_nSize = rhs.m_nSize;
			m_ulColor = rhs.m_ulColor;
			m_strFont = std::move(rhs.m_strFont);
			m_btCharSet = rhs.m_btCharSet;
		}
		return *this;
	}

	CCharInfo(const CString& strData)
	{
		CString strToken;
		int curPos = 0;

		strToken = strData.Tokenize(_T("|"), curPos);
		Char(strToken);
		strToken = strData.Tokenize(_T("|"), curPos);
		Size(GETINT(strToken));
		strToken = strData.Tokenize(_T("|,"), curPos);
		Color(GETULONG(strData));
		strToken = strData.Tokenize(_T("|"), curPos);
		Font(strToken);
		strToken = strData.Tokenize(_T("|"), curPos);
		CharSet(static_cast<BYTE>(GETINT(strData)));
	}

	CCharInfo(CString& strContents, CString& strSize, CString& strColor, CString& strFontName, CString& strCharSet)
	{
		Char(strContents);
		Size(GETINT(strSize));
		Color(GETULONG(strColor));
		Font(strFontName);
		CharSet(static_cast<BYTE>(GETINT(strCharSet)));
	}


	CCharInfo(CString& strContents, ULONG ulColor, CString& strFontName)
	{
		Char(strContents);
		Size(100);
		Color(ulColor);
		Font(strFontName);
		CharSet(129);
	}


	CCharInfo(CString& strContents, INT nSize, ULONG ulColor, CString& strFontName)
	{
		Char(strContents);
		Size(nSize);
		Color(ulColor);
		Font(strFontName);
		CharSet(129);
	}

	CCharInfo(CString& strContents, ULONG ulColor, const CString& strFontName)
	{
		Char(strContents);
		Size(100);
		Color(ulColor);
		Font(strFontName);
		CharSet(129);
	}

	~CCharInfo() {}


private:
	CString      m_strChar;
	int          m_nSize;
	ULONG        m_ulColor;
	CString      m_strFont;
	BYTE         m_btCharSet;

public:

	CString Char() const { return m_strChar; }
	void Char(CString val) { m_strChar = val; }

	int Size() const { return m_nSize; }
	void Size(int val) { m_nSize = val; }

	ULONG Color() const { return m_ulColor; }
	void Color(ULONG val) { m_ulColor = val; }

	CString Font() const { return m_strFont; }
	void Font(CString val) { m_strFont = val; }

	BYTE CharSet() const { return m_btCharSet; }
	void CharSet(BYTE val) { m_btCharSet = val; }

	void Serialize(CArchive& ar) override
	{

		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strChar;
			ar << m_nSize;
			ar << m_ulColor;
			ar << m_strFont;
			ar << m_btCharSet;
		}
		else
		{
			ar >> m_strChar;
			ar >> m_nSize;
			ar >> m_ulColor;
			ar >> m_strFont;
			ar >> m_btCharSet;
		}
	}

	bool IsEmpty() override
	{
		CCharInfo InitialData;
		if (*this == InitialData)
			return true;
		else
			return false;
	}

	void Clear() override
	{
		Char(BLANK_STRING);
		Size(0);
		Color(0);
		Font(BLANK_STRING);
		CharSet(129);
	}

	CCharInfo& operator=(CCharInfo& rhs)
	{
		Char(rhs.Char());
		Size(rhs.Size());
		Color(rhs.Color());
		Font(rhs.Font());
		CharSet(rhs.CharSet());

		return *this;
	}



	const BOOL operator== (const CCharInfo& rhs)
	{
		if (Char() != rhs.Char()) return FALSE;
		if (Size() != rhs.Size()) return FALSE;
		if (Color() != rhs.Color()) return FALSE;
		if (Font() != rhs.Font()) return FALSE;
		if (CharSet() != rhs.CharSet()) return FALSE;

		return TRUE;
	}

	const BOOL operator!= (const CCharInfo& rhs)
	{
		return !(*this == rhs);
	}

	bool IsImageChar(void)
	{
		return (Font() == IMAGE_FILE_TAG);
	}

	friend const BOOL operator== (const CCharInfo& lhs, const CCharInfo& rhs)
	{
		if (lhs.Char() != rhs.Char()) return FALSE;
		if (lhs.Size() != rhs.Size()) return FALSE;
		if (lhs.Color() != rhs.Color()) return FALSE;
		if (lhs.Font() != rhs.Font()) return FALSE;
		if (lhs.CharSet() != rhs.CharSet()) return FALSE;
		return TRUE;
	}

	friend const BOOL operator!= (const CCharInfo& lhs, const CCharInfo& rhs)
	{
		return !(lhs == rhs);
	}

	void SetCharInfoFromString(CString strData)
	{
		CCharInfo ci(strData);
		*this = ci;
	}

#ifdef DEBUG
	void TraceData(CString strMessage)
	{
		TRACE(_T("CCharInfo Data[%s]=========================================================\n"), strMessage);
		TRACE(_T("m_strChar : %s | "), Char());
		TRACE(_T("m_nSize : %s | "), GETSTR(Size()));
		TRACE(_T("m_ulColor : %s | "), GETSTR(Color()));
		TRACE(_T("m_strFontName : %s | "), Font());
		TRACE(_T("m_btCharSet : %s | "), GETSTR(CharSet()));
	}
#endif
};

//=======================================================
class CCharInfoList : public CClockDataObject
{
	DECLARE_DYNAMIC(CCharInfoList)

public:
	CCharInfoList()
	{
		m_CharInfoList.clear();
	}

	CCharInfoList(vector<CString> vecData)
	{
		m_CharInfoList.clear();
		for(auto& strData : vecData)
		{
			CCharInfo ci(strData);
			m_CharInfoList.emplace_back(ci);
		}

	}

	CCharInfoList(CString strContents, CString strSizes, CString strColors, CString strFonts, CString strCharSets)
	{
		SetCharInfoString(strContents, strSizes, strColors, strFonts, strCharSets);
	}

	CCharInfoList(const CCharInfoList& rhs)
	{
		m_CharInfoList = rhs.m_CharInfoList;
	}

	~CCharInfoList() {};


private:
	std::vector<CCharInfo> m_CharInfoList;
	std::vector<CString> m_vstrContents;
	std::vector<CString> m_vstrFonts;
	std::vector<ULONG> m_vulColors;
	std::vector<INT> m_vnSizes;
	BOOL m_bMixed;

public:

	std::vector<CCharInfo>& CharInfoList() { return m_CharInfoList; }
	void CharInfoList(std::vector<CCharInfo> val) { m_CharInfoList = val; }

	std::vector<CString>& Contents() { return m_vstrContents; }
	void Contents(std::vector<CString> val) { m_vstrContents = val; }

	std::vector<CString>& Fonts() { return m_vstrFonts; }
	void Fonts(std::vector<CString> val) { m_vstrFonts = val; }

	std::vector<ULONG>& Colors() { return m_vulColors; }
	void Colors(std::vector<ULONG> val) { m_vulColors = val; }

	std::vector<INT>& Sizes() { return m_vnSizes; }
	void Sizes(std::vector<INT> val) { m_vnSizes = val; }

	BOOL IsMixed() const { return m_bMixed; }
	void Mixed(BOOL val) { m_bMixed = val; }

	const CCharInfoList& operator=(const CCharInfoList& rhs)
	{
		m_CharInfoList = rhs.m_CharInfoList;

		return *this;
	}

	const BOOL operator== (const CCharInfoList& rhs)
	{
		if (m_CharInfoList == rhs.m_CharInfoList) return FALSE;

		return TRUE;
	}

	const BOOL operator!= (const CCharInfoList& rhs)
	{
		return !(*this == rhs);
	}

	friend const BOOL operator== (const CCharInfoList& lhs, const CCharInfoList& rhs)
	{
		if (lhs.m_CharInfoList == rhs.m_CharInfoList) return FALSE;

		return TRUE;
	}

	friend const BOOL operator!= (const CCharInfoList& lhs, const CCharInfoList& rhs)
	{
		return !(lhs == rhs);
	}

	const CCharInfoList& operator+(const CCharInfo& rhs)
	{
		m_CharInfoList.emplace_back(rhs);

		return *this;
	}

	const CCharInfoList& operator+(CCharInfoList& rhs)
	{

		for (auto& iter : rhs.m_CharInfoList)
		{
			m_CharInfoList.emplace_back(iter);
		}

		return *this;
	}

	void SetCharInfoString(CString strContents, CString strSizes, CString strColors, CString strFonts, CString strCharSets)
	{
		m_CharInfoList.clear();

		vector<CString> vContents;
		SplitString(strContents, CHARINFO_DELIMITER, vContents);
		vector<CString> vSizes;
		SplitString(strSizes, CHARINFO_DELIMITER, vSizes);
		vector<CString> vColors;
		SplitString(strColors, CHARINFO_DELIMITER, vColors);
		vector<CString> vFonts;
		SplitString(strFonts, CHARINFO_DELIMITER, vFonts);
		vector<CString> vCharSets;
		SplitString(strCharSets, CHARINFO_DELIMITER, vCharSets);

		int nSize = static_cast<int>(vContents.size());
		for (int nIndex = 0; nIndex < nSize; nIndex++)
		{
			m_CharInfoList.emplace_back(vContents.at(nIndex), vSizes.at(nIndex), vColors.at(nIndex), vFonts.at(nIndex), vCharSets.at(nIndex));
		}

		ExtractTextFormat();
	}

	void GetCharInfoString(CString& strContents, CString& strSizes, CString& strColors, CString& strFonts, CString& strCharSets)
	{
		strContents = BLANK_STRING;
		strSizes = BLANK_STRING;
		strColors = BLANK_STRING;
		strFonts = BLANK_STRING;
		strCharSets = BLANK_STRING;

		for (auto& charInfo : m_CharInfoList)
		{
			strContents += charInfo.Char() + CString(CHARINFO_DELIMITER);
			strSizes += GETSTR(charInfo.Size()) + CString(CHARINFO_DELIMITER);
			strColors += GETSTR(charInfo.Color()) + CString(CHARINFO_DELIMITER);
			strFonts += charInfo.Font() + CString(CHARINFO_DELIMITER);
			strCharSets += GETSTR<BYTE>(charInfo.CharSet()) + CString(CHARINFO_DELIMITER);
		}

		strContents.TrimRight(CHARINFO_DELIMITER);
		strSizes.TrimRight(CHARINFO_DELIMITER);
		strColors.TrimRight(CHARINFO_DELIMITER);
		strFonts.TrimRight(CHARINFO_DELIMITER);
		strCharSets.TrimRight(CHARINFO_DELIMITER);

	}

	BOOL MakeCharInfoListFromTextFormat(void)  // 만들기는 했는데 크게 의미가 없을 것 같음
	{
		BOOL bResult = FALSE;

		m_CharInfoList.clear();

		INT nSize = m_vstrContents.size();

		for (INT i = 0; i < nSize; i++)
		{
			INT nContentsIndex = min(i, m_vstrContents.size() - 1);
			INT nSizesIndex =  min(i, m_vnSizes.size() - 1);
			INT nColorsIndex = min(i, m_vulColors.size() - 1);
			INT nFontsIndex = min(i, m_vstrFonts.size() - 1);

			CString strContent = m_vstrContents.at(nContentsIndex);
			INT nCharSize = m_vnSizes.at(nSizesIndex);
			ULONG ulColor = m_vulColors.at(nColorsIndex);
			CString strFont = m_vstrFonts.at(nFontsIndex);
			
			CCharInfo ci(strContent, nCharSize, ulColor, strFont);
	
			m_CharInfoList.emplace_back(ci);
		}

		return TRUE;
	}


	BOOL FixedTextFormatFromCharInfoList(void)  // 만들기는 했는데 크게 의미가 없을 것 같음
	{
		BOOL bFixed = FALSE;

		CCharInfo charInfo;

		int nSize = static_cast<int>(m_CharInfoList.size());

		m_vstrContents.clear();
		m_vstrFonts.clear();
		m_vulColors.clear();
		m_vnSizes.clear();

		for (int i = 0; i < nSize; ++i)
		{
			charInfo = GetAt(i);

			INT nCount = charInfo.Char().GetLength();

			if (nCount > 1)
			{
				for (int j = 0; j < nCount; j++)
				{
					m_vstrContents.emplace_back(charInfo.Char().Mid(j, 1));
					m_vstrFonts.emplace_back(charInfo.Font());
					m_vulColors.emplace_back(charInfo.Color());
					m_vnSizes.emplace_back(charInfo.Size());
				}
				bFixed = TRUE;
			}
			else
			{
				m_vstrContents.emplace_back(charInfo.Char());
				m_vstrFonts.emplace_back(charInfo.Font());
				m_vulColors.emplace_back(charInfo.Color());
				m_vnSizes.emplace_back(charInfo.Size());
			}
		}

		return bFixed;
	}

	BOOL HasErrorTextFormat()
	{
		BOOL bHasError = FALSE;

		CCharInfo charInfo;
		int nSize = static_cast<int>(m_CharInfoList.size());

		for (int i = 0; i < nSize; ++i)
		{
			charInfo = GetAt(i);

			INT nCount = charInfo.Char().GetLength();

			if (nCount > 1)
			{
				bHasError = TRUE;
				return bHasError;
			}
		}

		return bHasError;

	}

	BOOL ExtractTextFormat(void)
	{
		BOOL bSuccess = TRUE;

		CCharInfo charInfo;
		int nSize = static_cast<int>(m_CharInfoList.size());

		m_vstrContents.clear();
		m_vstrFonts.clear();
		m_vulColors.clear();
		m_vnSizes.clear();

		for (int i = 0; i < nSize; ++i)
		{
			charInfo = GetAt(i);

			//INT nCount = charInfo.Char().GetLength();

			//if (nCount > 1)
			//{
			//	for (int j = 0; j < nCount; j++)
			//	{
			//		m_vstrContents.emplace_back(charInfo.Char().Mid(j,1));
			//		m_vstrFonts.emplace_back(charInfo.Font());
			//		m_vulColors.emplace_back(charInfo.Color());
			//		m_vnSizes.emplace_back(charInfo.Size());
			//	}
			//	bModified = TRUE;
			//}
			//else
			{
				m_vstrContents.emplace_back(charInfo.Char());
				m_vstrFonts.emplace_back(charInfo.Font());
				m_vulColors.emplace_back(charInfo.Color());
				m_vnSizes.emplace_back(charInfo.Size());
			}
		}

		return bSuccess;
	}

//	BOOL ExtractTextFormat(vector<CString>& vstrContents, vector<CString>& vstrFonts, vector<ULONG>& vulColors, vector<INT>& vnSizes)
//	{
//		BOOL bModified = FALSE;
//		CCharInfo charInfo;
//		int nSize = static_cast<int>(m_CharInfoList.size());
//
//		vstrContents.clear();
//		vstrFonts.clear();
//		vulColors.clear();
//		vnSizes.clear();
//
//		for (int i = 0; i < nSize; ++i)
//		{
//			charInfo = GetAt(i);
//			INT nCount = charInfo.Char().GetLength();
//
//			//if (nCount > 1)
//			//{
//			//	//for (int j = 0; j < nCount; j++)
//			//	//{
//			//	//	m_vstrContents.emplace_back(charInfo.Char().Mid(j, 1));
//			//	//	m_vstrFonts.emplace_back(charInfo.Font());
//			//	//	m_vulColors.emplace_back(charInfo.Color());
//			//	//	m_vnSizes.emplace_back(charInfo.Size());
//			//	//}
//
//			//	bModified = TRUE;
//
//			//}
//			//else
//			{
//				vstrContents.emplace_back(charInfo.Char());
//				vstrFonts.emplace_back(charInfo.Font());
//				vulColors.emplace_back(charInfo.Color());
//				vnSizes.emplace_back(charInfo.Size());
//			}
//		}
//
////		if (bModified)
//		{
//			MakeCharInfoListFromTextFormat();
//		}
//
//		return bModified;
//	}

	CString GetContentString(void)
	{
		CString strResult = BLANK_STRING;
		int nSize = static_cast<int>(m_CharInfoList.size());
		CCharInfo CharInfo;

		for (int i = 0; i < nSize; i++)
		{
			CharInfo = GetAt(i);
			strResult += CharInfo.Char();
		}

		return strResult;
	}

	CString GetConentStringForSpread(void)
	{
		CString strResult = BLANK_STRING;

		for (auto& iter : m_CharInfoList)
		{
			if (iter.IsImageChar())
			{
				strResult += CFileUtils::ExtractFileName(iter.Char());
			}
			else
			{
				strResult += iter.Char();
			}

		}
		return strResult;
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		int nSize = 0;
		CCharInfo CharInfo;

		if (ar.IsStoring())
		{
			nSize = static_cast<int>(m_CharInfoList.size());

			ar << nSize;

			for (auto& iter : m_CharInfoList)
			{
				iter.Serialize(ar);
			}

			ar << m_bMixed;
		}
		else
		{
			m_CharInfoList.clear();

			ar >> nSize;
			for (int i = 0; i < nSize; i++)
			{
				CharInfo.Serialize(ar);
				m_CharInfoList.emplace_back(CharInfo);
			}

			ar >> m_bMixed;
		}
	}

	bool IsEmpty() override
	{
		return m_CharInfoList.empty();
	}

	void Clear() override
	{
		m_CharInfoList.clear();
	}

	int Size(void) override { return static_cast<int>(m_CharInfoList.size()); }

	CCharInfo& GetAt(int n) { return m_CharInfoList.at(n); }

	void AddCharInfo(CCharInfo& charInfo)
	{
		m_CharInfoList.emplace_back(std::move(charInfo));
	}

	BOOL CalcMixed()
	{
		INT nSize = m_CharInfoList.size();

		if (nSize < 1) return FALSE;

		CCharInfo& charFirst = m_CharInfoList.at(0);

		if (charFirst.Font() == IMAGE_FILE_TAG)
		{
			m_bMixed = TRUE;
			return m_bMixed;
		}

		for (INT nIndex = 1; nIndex < nSize; nIndex++)
		{
			CCharInfo& charInfo = m_CharInfoList.at(nIndex);

			if (charFirst.Font() != charInfo.Font() || charInfo.Font() == IMAGE_FILE_TAG) 
			{
				m_bMixed = TRUE;
				return m_bMixed;
			}

			if (charFirst.Color() != charInfo.Color())
			{
				m_bMixed = TRUE;
				return m_bMixed;
			}
			if (charFirst.Size() != charInfo.Size())
			{
				m_bMixed = TRUE;
				return m_bMixed;
			}
		}

		m_bMixed = FALSE;

		return m_bMixed;
	}

	void Remove(LONG nStart, LONG nCount)  // nStart 는 인덱스처럼 생각해서 0부터 시작
	{
		auto iter_start = m_CharInfoList.begin() + nStart;
		auto iter_end = m_CharInfoList.begin() + nStart;

		if ((nStart + nCount) >= Size())
		{
			iter_end = m_CharInfoList.end();
		}
		else
		{
			std::advance(iter_end, nCount);
		}

		m_CharInfoList.erase(iter_start, iter_end);
	}

	void Mid(LONG nStart, LONG nEnd, CCharInfoList& charInfoList)
	{
		charInfoList.m_CharInfoList.clear();

		if (nEnd == -1) { nEnd = Size(); }

		for (LONG i = nStart; i < nEnd && i < Size(); i++)
		{
			charInfoList.m_CharInfoList.emplace_back(m_CharInfoList.at(i));
		}
	}


#ifdef DEBUG
	void TraceData(CString strMessage)
	{
		CCharInfo charInfo;

		TRACE(_T("=================================================   %s  =========================================================\n"), strMessage);

		for (int i = 0; i < Size(); i++)
		{
			charInfo = GetAt(i);
			charInfo.TraceData(strMessage);

		}

		TRACE(_T("======================================================================================================================\n\n"));
	}
#endif

};
//========================================================
class CNewsData : public CClockDataObject
{
	DECLARE_DYNAMIC(CNewsData)

public:
	CNewsData()
	{
		AssignFieldsToMap(vNews_Field_Name);
		SetValue(NEWS_FIELD_INDEX::USE, TRUE_STRING);
	}

	CNewsData(const CNewsData& rhs)
	{
		this->m_dwVersion = rhs.m_dwVersion;
		this->m_Data = rhs.m_Data;

		SetCharInfoListFromData(m_CLNewsData);
	}

	CNewsData(const CParentObject& rhs)
	{
		this->m_dwVersion = rhs.m_dwVersion;
		this->m_Data = rhs.m_Data;

		SetCharInfoListFromData(m_CLNewsData);
	}

	const CNewsData& operator= (const CParentObject& rhs)
	{
		this->m_dwVersion = rhs.m_dwVersion;
		this->m_Data = rhs.m_Data;

		SetCharInfoListFromData(m_CLNewsData);

		return *this;
	}

	~CNewsData() {};

private:
	CCharInfoList m_CLNewsData;

public:
	std::vector<CString>& Contents(void)
	{
		return m_CLNewsData.Contents();
	}

	std::vector<CString>& Fonts(void)
	{
		return m_CLNewsData.Fonts();
	}

	std::vector<ULONG>& Colors(void)
	{
		return m_CLNewsData.Colors();
	}

	std::vector<INT>& Sizes(void)
	{
		return m_CLNewsData.Sizes();
	}

	BOOL IsMixed(void)
	{
		return m_CLNewsData.IsMixed();
	}

	BOOL FixedTextFormatFromCharInfoList()
	{
		return m_CLNewsData.FixedTextFormatFromCharInfoList();
	}

	BOOL HasErrorTextFormat(void)
	{
		return m_CLNewsData.HasErrorTextFormat();
	}

	BOOL ExtractTextFormat(void)
	{
		return m_CLNewsData.ExtractTextFormat();
	}

	BOOL MakeCharInfoListFromTextFormat(void)
	{
		return m_CLNewsData.MakeCharInfoListFromTextFormat();
	}

	//BOOL  ExtractTextFormat(vector<CString>& vstrContents, vector<CString>& vstrFonts, vector<ULONG>& vulColors, vector<INT>& vnSizes)
	//{
	//	 return m_CLNewsData.ExtractTextFormat(vstrContents, vstrFonts, vulColors, vnSizes);
	//}

	// 뉴스데이터를 리플레이스하기 위해 파싱하면 텍스트와 이미지를 분리한 뉴스데이터 벡터를 만들게 되므로
	// 뉴스데이터 벡터에서 뉴스데이터를 꺼내서 이미지 파트인지 텍스트 파트인지를 구분하게 된다

	BOOL IsOnlyText(void)
	{
		for (auto& iter : m_CLNewsData.Fonts())
		{
			if (iter.CompareNoCase(IMAGE_FILE_TAG) == 0) return FALSE;
		}

		return TRUE;
	}

	BOOL IsImagePart(void)
	{
		INT nSize = m_CLNewsData.Fonts().size();

		if (nSize < 1) return FALSE;

		CString strFont = m_CLNewsData.Fonts().at(0);      // 이미지와 텍스트가 분리되어 있으므로 첫번째 문자정보로 구분이 가능하다.

		return (strFont.CompareNoCase(IMAGE_FILE_TAG) == 0);
	}

	CString ImageFileName(void)
	{
		std::vector<CString>& contents = m_CLNewsData.Contents();

		if (contents.size() < 1) return BLANK_STRING;

		return m_CLNewsData.Contents().at(0);
	}

	CCharInfoList GetCharInfoList(void)
	{
		SetCharInfoListFromData(m_CLNewsData);
		return m_CLNewsData;
	}

	void AddCharInfoList(CString strChar, ULONG ulColor, CString strFont)
	{
		int nLength = strChar.GetLength();
		for (int i = 0; i < nLength; i++)
		{
			CString strMid = strChar.Mid(i, 1);
			CCharInfo ci(strMid, ulColor, strFont);
			m_CLNewsData.AddCharInfo(ci);
		}
	}

	void AddCharInfoForImage(CString strImage)
	{
		CCharInfo ci(strImage, 0, const_cast<CString&>(IMAGE_FILE_TAG));
		m_CLNewsData.AddCharInfo(ci);
	}

	void SetCharInfoList(CCharInfoList& charInfoList)
	{
		SetDataFromCharInfoList(charInfoList);
	}

	void RefreshData(void)
	{
		SetDataFromCharInfoList(m_CLNewsData);
	}

	void RefreshCharInfoList(void)
	{
		SetCharInfoListFromData(m_CLNewsData);
	}

	void SetCharInfoListFromData(CCharInfoList& charInfoList)
	{
		CString strContents = GetValue(NEWS_FIELD_INDEX::CONTENTS);
		CString strSizes = GetValue(NEWS_FIELD_INDEX::SIZES);
		CString strColors = GetValue(NEWS_FIELD_INDEX::COLORS);
		CString strFonts = GetValue(NEWS_FIELD_INDEX::FONTS);
		CString strCharSet = GetValue(NEWS_FIELD_INDEX::CHARSETS);

		charInfoList.SetCharInfoString(strContents, strSizes, strColors, strFonts, strCharSet);
	}

	void SetDataFromCharInfoList(CCharInfoList& charInfoList)
	{
		m_CLNewsData = charInfoList;

		CString strContents;
		CString strSizes;
		CString strColors;
		CString strFonts;
		CString strCharSet;

		m_CLNewsData.GetCharInfoString(strContents, strSizes, strColors, strFonts, strCharSet);

		SetValue(NEWS_FIELD_INDEX::CONTENTS, strContents);
		SetValue(NEWS_FIELD_INDEX::SIZES, strSizes);
		SetValue(NEWS_FIELD_INDEX::COLORS, strColors);
		SetValue(NEWS_FIELD_INDEX::FONTS, strFonts);
		SetValue(NEWS_FIELD_INDEX::CHARSETS, strCharSet);

	}

	void ResetContents(void) override
	{
		CParentObject::ResetContents();
		m_CLNewsData.Clear();
	}

	CString GetContentString(void)
	{
		CString strContent = GetValue(NEWS_FIELD_INDEX::CONTENTS);

		strContent.Remove(CHARINFO_DELIMITER);

		return strContent;
	}

	CString GetContentStringForSpread(void)
	{
		return m_CLNewsData.GetConentStringForSpread();
	}

	bool IsAvailable(void)
	{
		bool bUse = (GetValue(NEWS_FIELD_INDEX::USE) == TRUE_STRING);

		return (bUse && GetContentString() != BLANK_STRING);
	}

	void ParseItem(std::vector<CNewsData>& vNewsData)
	{

		CNewsData newsDataBuffer;
		CCharInfo CharInfo;

		int nSize = m_CLNewsData.Size();

		for (int i = 0; i < nSize; i++)
		{
			CharInfo = m_CLNewsData.GetAt(i);

			if (CharInfo.Font() == IMAGE_FILE_TAG)
			{
				if (newsDataBuffer.m_CLNewsData.Size() > 0)   // 이전에 들어간 문자가 있으면 아이템을 하나 만들고 비운다.
				{
					newsDataBuffer.RefreshData();
					vNewsData.emplace_back(newsDataBuffer);
					newsDataBuffer = CNewsData();
				}

				newsDataBuffer.m_CLNewsData.CharInfoList().emplace_back(CharInfo);
				newsDataBuffer.RefreshData();
				vNewsData.emplace_back(newsDataBuffer);
				newsDataBuffer = CNewsData();
			}
			else
			{

				newsDataBuffer.m_CLNewsData.CharInfoList().emplace_back(CharInfo);

				if (i == m_CLNewsData.Size() - 1)
				{
					newsDataBuffer.RefreshData();
					vNewsData.emplace_back(newsDataBuffer);
					newsDataBuffer = CNewsData();
				}
			}
		}

	}

};
//=========================================================
class CNewsDataList : public CTemplateList<CNewsData>
{
	DECLARE_DYNAMIC(CNewsDataList)

public:

	CNewsDataList()
	{
		SetTableName(MakeKeyString());
		CFields fields(vNews_Field);
		SetFields(fields);
	}

	CNewsDataList(CFields& fields)
	{
		SetTableName(MakeKeyString());
		SetFields(fields);
	}

	~CNewsDataList() {}

	void OptimizeData(void)
	{
		std::vector<CNewsData> vItemData;
		std::vector<CNewsData> vUseData;

		SelectToObjectsWithField(vUseData, vNews_Field_Name[NEWS_FIELD_INDEX::USE], TRUE_STRING);

		for (auto& iter : vUseData)
		{
			if (iter.GetContentString() == BLANK_STRING)
			{
				iter.SetValue(NEWS_FIELD_INDEX::USE, FALSE_STRING);
				vItemData.emplace_back(iter);
			}
		}

		UpdateRecords(vItemData);
	}

	BOOL GetAvailableData(vector<CNewsData>& vItemData)
	{
		std::vector<CNewsData> vUseData;

		SelectToObjectsWithField(vUseData, vNews_Field_Name[NEWS_FIELD_INDEX::USE], TRUE_STRING);

		vItemData.clear();

		for (auto& iter : vUseData)
		{
			if (iter.GetContentString() != BLANK_STRING)
			{
				vItemData.emplace_back(iter);
			}
		}

		return (!vItemData.empty());
	}


	BOOL HasAvailableData(void)
	{
		std::vector<CNewsData> vUseData;

		SelectToObjectsWithField(vUseData, vNews_Field_Name[NEWS_FIELD_INDEX::USE], TRUE_STRING);

		for (auto& iter : vUseData)
		{
			if (iter.GetContentString() != BLANK_STRING)
			{
				return TRUE;
			}
		}

		return FALSE;
	}

};


class CWeatherData : public CClockDataObject
{
	DECLARE_DYNAMIC(CWeatherData)

public:
	CWeatherData()
	{
		AssignFieldsToMap(vWeather_Field_Name);
		MakeKeyString();
	}

	CWeatherData(const CWeatherData& rhs)
	{
		this->m_dwVersion = rhs.m_dwVersion;
		this->m_Data = rhs.m_Data;
	}

	CWeatherData(const CClockDataObject& rhs)
	{
		this->m_dwVersion = rhs.m_dwVersion;
		this->m_Data = rhs.m_Data;
	}

	void ClearWeatherData(void)
	{
		SetValue(WEATHER_CODE, BLANK_STRING);
		SetValue(WEATHER_NAME, BLANK_STRING);
		SetValue(WEATHER_TEMP, BLANK_STRING);
	}

	void ClearAirData(void)
	{
		SetValue(PM25AVG, BLANK_STRING);
		SetValue(PM25GRADE, BLANK_STRING);
	}

	void ClearWeatherAndAirData(void)
	{
		SetValue(WEATHER_CODE, BLANK_STRING);
		SetValue(WEATHER_NAME, BLANK_STRING);
		SetValue(WEATHER_TEMP, BLANK_STRING);
		SetValue(PM25AVG, BLANK_STRING);
		SetValue(PM25GRADE, BLANK_STRING);
	}

	CString MakeWeatherCode(CString strCode)
	{
		INT nCode = GETINT(strCode.TrimLeft(_T("DW0")));
		CString strWeatherCode;

		strWeatherCode.Format(_T("%03d"), nCode);

		strWeatherCode = _T("DW") + strWeatherCode;

		return strWeatherCode;
	}

	BOOL IsValidWeatherCode(CString strWeatherCode)
	{
		INT nCode = GETINT(strWeatherCode.TrimLeft(_T("DW0")));

		return (nCode > 0 && nCode < 24 && nCode != 3);
	}

	CString DecisionWeatherName(CString strWeatherCode)
	{
		INT nCode = GETINT(strWeatherCode.TrimLeft(_T("DW0")));

		if (IsValidWeatherCode(strWeatherCode))
		{
			return strWeatherName[nCode];
		}

		return BLANK_STRING;
	}

	CString DecisionAirLevel(CString strPM25AVG)
	{
		FLOAT fAirLevel = GETFLOAT(strPM25AVG);

		AIR_LEVEL airLevelValue = AIR_LEVEL::NONE;

		if (fAirLevel < 0.0f)
		{
			airLevelValue = AIR_LEVEL::NONE;
		}
		else if (fAirLevel <= 15.0f)
		{
			airLevelValue = AIR_LEVEL::GOOD;
		}
		else if (fAirLevel <= 35.0f)
		{
			airLevelValue = AIR_LEVEL::NOMAL;
		}
		else if (fAirLevel <= 75.0f)
		{
			airLevelValue = AIR_LEVEL::BAD;
		}
		else
		{
			airLevelValue = AIR_LEVEL::VERY_BAD;
		}

		return strAirLevel[FROM_E(airLevelValue)];
	}

	ULONG GetDustColor(void)
	{
		CString strDustGrade = GetValue(WEATHER_FIELD_INDEX::PM25GRADE);

		if (strDustGrade == strAirLevel[FROM_E(AIR_LEVEL::NONE)])
		{
			return ulAirLevelColor[FROM_E(AIR_LEVEL::NONE)];
		}
		else if (strDustGrade == strAirLevel[FROM_E(AIR_LEVEL::GOOD)])
		{
			return ulAirLevelColor[FROM_E(AIR_LEVEL::GOOD)];
		}
		else if (strDustGrade == strAirLevel[FROM_E(AIR_LEVEL::NOMAL)])
		{
			return ulAirLevelColor[FROM_E(AIR_LEVEL::NOMAL)];
		}
		else if (strDustGrade == strAirLevel[FROM_E(AIR_LEVEL::BAD)])
		{
			return ulAirLevelColor[FROM_E(AIR_LEVEL::BAD)];
		}
		else if (strDustGrade == strAirLevel[FROM_E(AIR_LEVEL::VERY_BAD)])
		{
			return ulAirLevelColor[FROM_E(AIR_LEVEL::VERY_BAD)];
		}

		return ulAirLevelColor[FROM_E(AIR_LEVEL::NONE)];
	}

	CString GetDustGradeFileName(void)
	{
		CString strDustGrade = GetValue(WEATHER_FIELD_INDEX::PM25GRADE);
		CString strGradeFileName = GetWeatherIconsDir() + _T("미세먼지_") + strDustGrade + _T(".PNG");

		if (!CFileUtils::ExistFile(strGradeFileName))
		{
			strGradeFileName = GetWeatherIconsDir() + _T("미세먼지_") + strDustGrade + _T(".TGA");
		}

		return strGradeFileName;

	}

	CString GetIconFileName(void)
	{
		CString strWeatherCode = GetValue(WEATHER_FIELD_INDEX::WEATHER_CODE);
		CString strIconFileName = GetWeatherIconsDir() + strWeatherCode + _T(".PNG");

		if (!CFileUtils::ExistFile(strIconFileName))
		{
			strIconFileName = GetWeatherIconsDir() + strWeatherCode + _T(".TGA");
		}

		return strIconFileName;
	}

	BOOL IsEmptyWeather(void)
	{
		if (GetValue(WEATHER_FIELD_INDEX::WEATHER_CODE) == BLANK_STRING) return TRUE;
		if (GetValue(WEATHER_FIELD_INDEX::WEATHER_TEMP) == BLANK_STRING) return TRUE;

		return FALSE;
	}

	BOOL IsEmptyDust(void)
	{
		if (GetValue(WEATHER_FIELD_INDEX::PM25AVG) == BLANK_STRING) return TRUE;
		if (GetValue(WEATHER_FIELD_INDEX::PM25GRADE) == BLANK_STRING) return TRUE;

		return false;
	}

	BOOL IsAvailable(void)
	{
		return !IsEmptyWeather();
	}

	virtual ~CWeatherData() {};
};


class CWeatherDataList : public CTemplateList<CWeatherData>
{
	DECLARE_DYNAMIC(CWeatherDataList)

private:
	std::map<CString, CWeatherData> m_mapHjdCode_WeatherData;  // 서버로부터 받은 데이터를 모두 저장하기 위한 맵이다.
	std::map<INT, CString> m_mapCityCode_HdjCode;
	std::map<CString, CString> m_mapHjdCode_CityName;
	std::map<CString, CString> m_mapStationName_HjdCode;
	std::map<CString, int> m_mapStationName_Pm25Value;
	std::map<CString, vector<int>> m_mapHjdCode_Pm25Values;
	std::map<CString, float> m_mapHjdCode_Pm25Avg;

	CTime m_timeReadWeatherAndAir;

	bool m_bLoadXlsFile;

	// WeatherData 
	//	1. Xls파일의 시티코드 시트에서 행정동코드와 시군구명을 읽어 m_mapHjdCode_CityName을 작성한다
	//     m_mapCityCode_HdjCode도 만든다 
	//  2. m_mapHjdCode_CityName을 이용하여 각 도시에 해당되는 CWeatherData를 만들어 리스트에 저장한다. m_mapHjdCode_WeatherData를 만든다.
	//	3. 재난 방송시스템에서 날씨 정보를 읽어 m_mapHjdCode_WeatherData에 있는 CWeatherData들을 갱신한다.

	//  Air Data 추출
	//  1. Xls파일 미세먼지 시트에서 StationName과 HjdCode를 읽어 m_mapStationName_HjdCode 맵을 작성한다.
	//	2. Air Korea API에서 Json데이터를 읽어 온다.
	//	3. Json 데이터를 파싱하면서 StationName과 pm25Value를 읽어 m_mapStationName_Pm25Value 맵을 작성한다.
	//	4. m_mapStationName_HjdCode 와 m_mapStationName_Pm25Value 를 이용하여 m_mapHjdCode_Pm25Values를 작성하여
	//	   각 도시에 여러개의 미세먼지 값들을 벡터로 저장한다.
	//	5. m_mapHjdCode_Pm25Values에서 각 City에 해당하는 값들의 평균을 계산하여 m_mapHjdCode_Pm25Avg 맵을 작성한다.
	//	6. m_mapHjdCode_Pm25Avg를 이용하여 공기질을 판단한다.
	//	8. m_mapHjdCode_WeatherData과 m_mapHjd_Pm25Avg를 이용하여 각 WeatherData에 PM25AVG, PM25GRADE 값을 넣어주면서
	//	   vector<CWeatherData>를 만들어준다.
	//	9. UpdateRecords(vector<CWeatherData>)를 한다.


public:
	CWeatherDataList()
	{
		SetTableName(MakeKeyString());
		CFields fields(vWeather_Field);
		SetFields(fields);

		ResetReadTime();

		//GetAllWeatherAndAirData();
	};

	void ResetReadTime(void)
	{
		m_timeReadWeatherAndAir = CTime(2023, 1, 1, 0, 0, 0);
		
	}

	CTime GetTimeReadWeatherAndAir(void) { return m_timeReadWeatherAndAir; }

	void SetTimeReadWeatherAndAir(CTime timeReadWeatherAndAir) { m_timeReadWeatherAndAir = timeReadWeatherAndAir; }

	bool LoadCodeFromXls(CString& strError)
	{
		m_bLoadXlsFile = true;

		if (!LoadHjdCodeFromXls(WEATHER_CODE_INFO_XLS_FILENAME, strError))
		{
			PrintLog_Message(strError, TRUE);
			m_bLoadXlsFile = false;
		}

		if (!LoadStationNameFromXls(WEATHER_CODE_INFO_XLS_FILENAME, strError))
		{
			PrintLog_Message(strError, TRUE);
			m_bLoadXlsFile = false;
		}

		return m_bLoadXlsFile;
	}

	INT GetAllWeatherAndAirData(BOOL bExceptAir)
	{
		CString strError;

		INT nResult = 0;

		if (!m_bLoadXlsFile)
		{
			LoadCodeFromXls(strError);
		}

		if (!GetWeatherData(strError))
		{
			PrintLog_Message(strError, TRUE);
			nResult = 1;
		}

		if (!bExceptAir)
		{
			if (!GetAirData(strError))
			{
				PrintLog_Message(_T("Get Air Data Error"), TRUE);
				nResult += 1;
			}
		}

		return nResult;
	}

	bool FindWeatherDataByCityCode(INT nCityCode, CWeatherData& weatherData)
	{
		if (m_mapCityCode_HdjCode.find(nCityCode) == m_mapCityCode_HdjCode.end())
		{
			return false;
		}

		CString strHdj = m_mapCityCode_HdjCode[nCityCode];

		if (m_mapHjdCode_WeatherData.find(strHdj) == m_mapHjdCode_WeatherData.end())
		{
			return false;
		}

		weatherData = m_mapHjdCode_WeatherData[strHdj];

		return true;
	}

	bool UpdateSelectCity(std::map<INT, BOOL>& mapSelectCity)
	{
		DeleteAllData();

		std::vector<CWeatherData> vWeatherData;
		for (auto& iter : mapSelectCity)
		{
			CWeatherData weatherData;

			if (iter.second == TRUE)
			{
				if (FindWeatherDataByCityCode(iter.first, weatherData))
				{
					vWeatherData.emplace_back(weatherData);
				}
			}
		}

		AddRecords(vWeatherData);

		return true;
	}

	bool HaveAvailableData(void)
	{
		std::vector<CWeatherData> vWeatherData;

		SelectToAllObjects(vWeatherData);

		for (auto& iter : vWeatherData)
		{
			if (iter.IsAvailable()) return true;
		}

		return false;
	}

	bool RefreshWeatherAndAirData(bool bReget,bool bExceptAir)
	{

		if (bReget)
		{
			if (GetAllWeatherAndAirData(bExceptAir) > 1)
			{
				return false;
			}
		}

		std::vector<CWeatherData> vWeatherData;

		SelectToAllObjects(vWeatherData);

		std::vector<CWeatherData> vNewData;

		for (auto& iter : vWeatherData)
		{
			CString strHjd = iter.GetValue(WEATHER_FIELD_INDEX::HJD_CODE);

			CWeatherData& weatherData = m_mapHjdCode_WeatherData[strHjd];

			vNewData.emplace_back(weatherData);
		}

		DeleteAllData();

		AddRecords(vNewData);

		return true;
	}

	bool LoadHjdCodeFromXls(CString strXlsFileName, CString& strError)
	{
		Book* book = xlCreateBook();
		book->setKey(LIBXL_KEY_1,LIBXL_KEY_2);

		bool bResult = false;

		try
		{
			m_mapHjdCode_CityName.clear();
			m_mapHjdCode_WeatherData.clear();

			if (book->load(strXlsFileName))
			{
				int nCount = book->sheetCount();

				Sheet* sheet = book->getSheet(0);  // 시도 코드 시트

				int nLastRow = sheet->lastRow();

				if (sheet)
				{
					for (int nRow = 1; nRow < nLastRow; nRow++)  // 타이틀 줄을 빼고 읽어 들인다.
					{
						CWeatherData weatherData;

						CString strHjdCode = sheet->readStr(nRow, 0);
						CString strCityName = sheet->readStr(nRow, 2);
						CString strCityCode = sheet->readStr(nRow, 3);
						CString strGCode = sheet->readStr(nRow, 4);

						INT nCityCode = GETINT(strCityCode);
						if (!strHjdCode.IsEmpty() && strGCode != _T("G"))
						{
							weatherData.SetValue(WEATHER_FIELD_INDEX::HJD_CODE, strHjdCode);
							weatherData.SetValue(WEATHER_FIELD_INDEX::CITY_CODE, strCityCode);
							weatherData.SetValue(WEATHER_FIELD_INDEX::CITY_NAME, strCityName);


							m_mapHjdCode_CityName[strHjdCode] = strCityName;
							m_mapCityCode_HdjCode[GETINT(strCityCode)] = strHjdCode;
							m_mapHjdCode_WeatherData[strHjdCode] = weatherData;
						}
					}

				}
				strError = strXlsFileName + _T("가져오기 성공!!");
				bResult = true;

			}
			else
			{
				strError = strXlsFileName + _T(" 가져오기 실패!!!");
				bResult = false;
			}
		}
		catch (CException* e)
		{
			strError = CString(book->errorMessage());
			bResult = false;
		}

		book->release();

		return bResult;
	}

	static size_t jsonWriter(void* buffer, size_t itemsize, size_t nitems, void* param)
	{
		std::string& textData = *static_cast<std::string*>(param);
		size_t bytes = itemsize * nitems;

		textData.append(static_cast<char*>(buffer), bytes);

		return bytes;
	}

	void ClearAirDataForHdj_WeatherMap(void)
	{
		for (auto& iter : m_mapHjdCode_WeatherData)
		{
			iter.second.ClearAirData();
		}
	}

	void ClearWeatherDataForHdj_WeatherMap(void)
	{
		for (auto& iter : m_mapHjdCode_WeatherData)
		{
			iter.second.ClearWeatherData();
		}
	}

	void ClearWeatherAndAirDataForHdj_WeatherMap(void)
	{
		for (auto& iter : m_mapHjdCode_WeatherData)
		{
			iter.second.ClearWeatherAndAirData();
		}
	}

	bool GetWeatherData(CString& strError)
	{
		ClearWeatherDataForHdj_WeatherMap();

		CString strWeatherJsonData;

		CURLcode resultCode = GetWeatherDataFromRestAPI(strWeatherJsonData);

		if (resultCode != CURLcode::CURLE_OK) return false;

		int nReadCount = 0;

		if (ParseWeatherDataFromRestAPI(strWeatherJsonData, nReadCount) == -1)
		{
			strError = _T("Weather Data Parse Error!");
			return false;
		}

		if (nReadCount == 0)
		{
			strError = _T("Weather Data Read Count 0!");
			return false;
		}

		return true;
	}

	CURLcode GetWeatherDataFromRestAPI(CString& strWeatherJsonData)
	{

		CURLcode result = CURLE_OK;

		std::string strWeather;

		CURL* curl = curl_easy_init();

		struct curl_slist* headers = nullptr;

		if (!curl) {
			return CURLE_FAILED_INIT;
		}

		headers = curl_slist_append(headers, "accept:application/json");
		headers = curl_slist_append(headers, "Content-Type:application/json");
		headers = curl_slist_append(headers, "authorization:Basic bWlyd2VhdGhlcjprYnMxMjM0");

		curl_easy_setopt(curl, CURLOPT_URL, "https://diapi.kbs.co.kr/v2/api/kma/getMaCurrentWeather");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{ \"pageNo\": 1, \"pageSize\": 1000, \"unitCode\": 2}");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->jsonWriter);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strWeather);

		result = curl_easy_perform(curl);

		curl_slist_free_all(headers);

		curl_easy_cleanup(curl);

		if (result != CURLE_OK) {
			return result;
		}

		strWeatherJsonData = UTF8toCString(strWeather);

		if (strWeatherJsonData.Find(_T("html")) != -1 || strWeatherJsonData.Find(_T("Time-out")) != -1)  // 만약 시간이 오래걸려 html로 응답이 오면
		{
			PrintLog_Message(_T("날씨 데이터 읽기 Time-Out이 발생했습니다."), TRUE);

			return CURLE_UNSUPPORTED_PROTOCOL;
		}

		return  result;
	}

	int ParseWeatherDataFromRestAPI(CString& strWeatherData, int& nReadCount)
	{
		int nResult = -1;

		if (!IsJsonFormatsSimple(strWeatherData)) return -1;

		if (strWeatherData.Find(_T("<")) != -1) return -1; // error 발생하면 html로 넘어옴

		CWeatherData weatherData;
		INT nZeroCount = 0;

		JSONPP::CValue jsonParser;

		if (strWeatherData.IsEmpty())
		{
			return nResult;
		}

		jsonParser.Parse(strWeatherData.GetBuffer());

		if (jsonParser.IsObject())
		{
			CValue& jResult = jsonParser.Child(_T("resultCode"));

			nResult = GETINT(jResult.AsString().c_str());

			if (nResult != 0)
			{
				PrintLog_Message(_T("Weather Response Error"), TRUE);
				return nResult;
			}

			CValue& jtotCnt = jsonParser.Child(_T("totCnt"));

			int nTotal = static_cast<int>(jtotCnt.AsNumber());

			CValue& jData = jsonParser.Child(_T("data"));

			if (jData.IsArray())
			{
				JSONPP::Array& jsonArray = jData.AsArray();

				for (auto& data : jsonArray)
				{
					CString strHjdCode = data.Child(_T("hjdCode")).AsString().c_str();

					auto iter = m_mapHjdCode_WeatherData.find(strHjdCode);

					if (iter != m_mapHjdCode_WeatherData.end())
					{
						weatherData = m_mapHjdCode_WeatherData[strHjdCode];

						weatherData.ClearWeatherData();

						if (data.HasChild(_T("t1h")))
						{
							if (data.Child(_T("t1h")).IsString())
							{
								CString strTemp = data.Child(_T("t1h")).AsString().c_str();
								weatherData.SetValue(WEATHER_FIELD_INDEX::WEATHER_TEMP, strTemp);
								if (GETINT(strTemp) == 0) nZeroCount++;
							}
						}

						if (data.HasChild(_T("present_weath_cd")))
						{
							if (data.Child(_T("present_weath_cd")).IsString())
							{
								CString strIcon = data.Child(_T("present_weath_cd")).AsString().c_str();
								weatherData.SetValue(WEATHER_FIELD_INDEX::WEATHER_CODE, strIcon);
							}
						}

						if (data.HasChild(_T("present_weath_name")))
						{
							if (data.Child(_T("present_weath_name")).IsString())
							{
								CString strWeatherName = data.Child(_T("present_weath_name")).AsString().c_str();
								weatherData.SetValue(WEATHER_FIELD_INDEX::WEATHER_NAME, strWeatherName);
								nReadCount++;
							}
						}

						m_mapHjdCode_WeatherData[strHjdCode] = weatherData;
					}
				}

			}
		}

		if (nZeroCount == nReadCount) return -1; // 모든 기온이 0으로 들어오는것은 오류

		return nResult;
	}

	bool LoadStationNameFromXls(CString strXlsFileName, CString& strError)
	{
		Book* book = xlCreateBook();
		book->setKey(LIBXL_KEY_1,LIBXL_KEY_2);

		bool bResult = false;

		try
		{
			m_mapStationName_HjdCode.clear();

			if (book->load(strXlsFileName))
			{
				int nCount = book->sheetCount();

				Sheet* sheet = book->getSheet(1);

				int nLastRow = sheet->lastRow();

				if (sheet)
				{
					for (int nRow = 1; nRow < nLastRow; nRow++)  // 타이틀 줄을 빼고 읽어 들인다.
					{
						CString strStationName = sheet->readStr(nRow, 0);
						CString strHjdCode = sheet->readStr(nRow, 1);

						m_mapStationName_HjdCode[strStationName] = strHjdCode;
					}

				}
				strError = strXlsFileName + _T(" 가져오기 성공!!");
				bResult = true;
			}
			else
			{
				strError = strXlsFileName + _T(" 가져오기 실패!!!");
				bResult = false;
			}
		}
		catch (CException* e)
		{
			strError = CString(book->errorMessage());
			bResult = false;
		}

		book->release();

		return bResult;
	}

	bool GetAirData(CString& strError)
	{
		CString strAirJsonData;


		ClearAirDataForHdj_WeatherMap();

		if (!GetPm25DataFromAirKoreaRestAPI(strAirJsonData))
		{
			strError = _T("미세 먼지 정보 읽기 실패");
			return false;
		}

		if (!ParseAirData(strAirJsonData))
		{
			strError = _T("미세 먼지 정보 파싱 실패");
			return false;
		}

		RelocateAirData();

		CalcAvg();

		strError = _T("미세 먼지 정보 읽기 성공");

		return true;
	}

	bool GetPm25DataFromAirKoreaRestAPI(CString& strAirJsonData)
	{
		std::string sAir;

		CURL* curl = curl_easy_init();

		if (!curl) {
			fprintf(stderr, "Init failed\n");
			return false;
		}

		//운용계정으로 바꿈
		curl_easy_setopt(curl, CURLOPT_URL, "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty?serviceKey=FltNnTZKySNKBRXgh7%2bqGYQGcshBni1TGgS4lLhFsyZJOEkXa%2fftEsJ%2b30fo95%2bdaX5MiR5S6yb7BneZqI%2f4jg%3d%3d&returnType=json&numOfRows=800&pageNo=1&sidoName=%EC%A0%84%EA%B5%AD&ver=1.3");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->jsonWriter);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sAir);

		CURLcode result = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

		if (result != CURLE_OK) {
			fprintf(stderr, "download problem: %s\n", curl_easy_strerror(result));

			return false;
		}

		strAirJsonData = UTF8toCString(sAir);

		return true;
	}

	bool IsJsonFormatsSimple(const CString& str)
	{
		CString trimmed = str;
		trimmed.Trim();

		if (trimmed.IsEmpty())
			return false;

		// JSON은 { ... } 또는 [ ... ] 로 시작함
		if ((trimmed[0] == '{' && trimmed[trimmed.GetLength() - 1] == '}') ||
			(trimmed[0] == '[' && trimmed[trimmed.GetLength() - 1] == ']'))
		{
			return true;
		}

		return false;
	}

	bool ParseAirData(CString& strAirJsonData)
	{
		if (strAirJsonData.Find(_T("<")) != -1) return false; // error 발생하면 html로 넘어옴

		if (strAirJsonData.Find(_T("html")) != -1) return false; // error 발생하면 html로 넘어옴

		if (!IsJsonFormatsSimple(strAirJsonData)) return false;  //error로 판정

	//	INT nCount = 1;

		CValue jsonParser;

		jsonParser.Parse(strAirJsonData);

		m_mapStationName_Pm25Value.clear();

		if (jsonParser.IsObject())
		{
			CValue& jResponse = jsonParser.Child(_T("response"));

			CValue& jHeader = jResponse.Child(_T("header"));

			int nResult = GETINT(jHeader.Child(_T("resultCode")).AsString().c_str());

			if (nResult != 0)
			{
				PrintLog_Message(_T("Air Data Parser Error"), TRUE);
				return false;
			}

			CValue& jBody = jResponse.Child(_T("body"));

			int nTotal = static_cast<int>(jBody.Child(_T("totalCount")).AsNumber());

			CValue& jItems = jBody.Child(_T("items"));

			if (jItems.IsArray())
			{
				JSONPP::Array& jsonArray = jItems.AsArray();

				for (auto& data : jsonArray)
				{
					CString strStationName = data.Child(_T("stationName")).AsString().c_str();

					CValue nodeDataTime = data.Child(_T("dataTime"));

					bool bValid = nodeDataTime.IsString();

					if (bValid)
					{
						CString strDataTime = nodeDataTime.AsString().c_str();

						CTime timeData = GetTimeFromString(strDataTime);

						CTimeSpan tsData = GETTIMESPAN(CTime::GetCurrentTime(), timeData);

						bValid = tsData.GetTotalSeconds() < 3600;  // 1시간 이내의 데이터여야 유효

						if (!bValid)
						{
							m_mapStationName_Pm25Value[strStationName] = INVALID_PM25VALUE;
							continue;
						}

					}

					CValue nodePm25Flag = data.Child(_T("pm25Flag"));

					bValid = nodePm25Flag.IsNull();

					if (bValid)
					{
						CValue nodePm25Value = data.Child(_T("pm25Value"));

						bool bValid = nodePm25Value.IsString();

						if (bValid)
						{
							CString strPm25Value = nodePm25Value.AsString().c_str();

							bValid = IsOnlyNumberString(strPm25Value);

							if (bValid)
							{
								m_mapStationName_Pm25Value[strStationName] = GETINT(strPm25Value);
								//	OutputDebugString(GETSTR(nCount++) + _T(" : ") + strStationName + _T(" : ") + strPm25Value +_T("\n\r"));
							}
						}
					}

					if (!bValid)
					{
						m_mapStationName_Pm25Value[strStationName] = INVALID_PM25VALUE;
					}
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	AIR_LEVEL DecisionAirLevel(float fAirLevel)
	{
		AIR_LEVEL airLevelValue = AIR_LEVEL::NONE;

		if (fAirLevel < 0.0f)
		{
			airLevelValue = AIR_LEVEL::NONE;
		}
		else if (fAirLevel <= 15.0f)
		{
			airLevelValue = AIR_LEVEL::GOOD;
		}
		else if (fAirLevel <= 35.0f)
		{
			airLevelValue = AIR_LEVEL::NOMAL;
		}
		else if (fAirLevel <= 75.0f)
		{
			airLevelValue = AIR_LEVEL::BAD;
		}
		else
		{
			airLevelValue = AIR_LEVEL::VERY_BAD;
		}

		return airLevelValue;
	}

	float CalcAvg(std::vector<int>& vAirData)
	{
		int nSum = 0;
		int nCount = 0;
		float fPm25Avg = 0.0f;

		for (auto& iter : vAirData)
		{
			if (iter != INVALID_PM25VALUE && iter >= 0)
			{
				nSum += iter;
				nCount++;
			}
		}

		if (nCount > 0)
		{
			fPm25Avg = (nSum) / nCount;
		}
		else
		{
			fPm25Avg = INVALID_PM25VALUE;
		}

		return fPm25Avg;
	}

	void RelocateAirData(void)
	{
		m_mapHjdCode_Pm25Values.clear();

		for (auto& iter : m_mapStationName_HjdCode)
		{
			CString strStationName = iter.first;

			CString strHjdCode = iter.second;

			int nPm25Value = m_mapStationName_Pm25Value[strStationName];

			auto iter = m_mapHjdCode_Pm25Values.find(strHjdCode);

			if (iter != m_mapHjdCode_Pm25Values.end())
			{
				vector<int>& vValues = m_mapHjdCode_Pm25Values[strHjdCode];

				vValues.emplace_back(nPm25Value);

				m_mapHjdCode_Pm25Values[strHjdCode] = vValues;

			}
			else
			{
				vector<int> vValues;

				vValues.emplace_back(nPm25Value);

				m_mapHjdCode_Pm25Values[strHjdCode] = vValues;
			}
		}
	}

	void CalcAvg(void)
	{
		for (auto& iter : m_mapHjdCode_Pm25Values)
		{
			vector<int>& vValues = iter.second;

			float fAvg = CalcAvg(vValues);

			if (fAvg > 0.0f)
			{
				CWeatherData weatherData = m_mapHjdCode_WeatherData[iter.first];

				weatherData.ClearAirData();

				weatherData.SetValue(WEATHER_FIELD_INDEX::PM25AVG, GETSTR(fAvg));
				weatherData.SetValue(WEATHER_FIELD_INDEX::PM25GRADE, strAirLevel[FROM_E(DecisionAirLevel(fAvg))]);

				m_mapHjdCode_WeatherData[iter.first] = weatherData;
			}
		}
	}

	void ClearWeatherAndAirData(void)
	{
		std::vector<CWeatherData> vWeatherData;

		SelectToAllObjects(vWeatherData);

		for (auto& iter : vWeatherData)
		{
			iter.ClearWeatherAndAirData();
		}

		DeleteAllData();

		ResetReadTime();

		AddRecords(vWeatherData);

	}

	virtual ~CWeatherDataList() {};

	std::map<INT, CString>& MapCityCode_HdjCode() { return m_mapCityCode_HdjCode; }
	void MapCityCode_HdjCode(std::map<INT, CString> val) { m_mapCityCode_HdjCode = val; }

};



class CNoticeData : public CClockDataObject
{
	DECLARE_DYNAMIC(CNoticeData)

public:
	CNoticeData()
	{
		AssignFieldsToMap(vNotice_Field_Name);
		MakeKeyString();
	}

	CNoticeData(const CNoticeData& rhs)
	{
		this->m_dwVersion = rhs.m_dwVersion;
		this->m_Data = rhs.m_Data;
	}

	CNoticeData(const CClockDataObject& rhs)
	{
		this->m_dwVersion = rhs.m_dwVersion;
		this->m_Data = rhs.m_Data;
	}

	BOOL SetFpgFile(CHDdaVinci* pHDdaVinci, CString strFpg)
	{	
		if (!CFileUtils::ExistFile(strFpg)) return FALSE;

		CString strPrevFpg = GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

		if (strPrevFpg == BLANK_STRING && strFpg != BLANK_STRING)
		{
			SetValue(NOTICE_FIELD_INDEX::USE, TRUE_STRING);

			SetValue(NOTICE_FIELD_INDEX::TEMPLATE, strFpg);

			ClearAliasAndValue();

			GetAliases(pHDdaVinci);
		}
		else if (strPrevFpg != BLANK_STRING && strFpg.CompareNoCase(strPrevFpg) != 0)
		{
			UINT nYesNo = AfxMessageBox(_T("템플릿 다른것을 선택했습니다. 이전에 입력된 알리아스는 없어집니다. 진행할까요 ?"), MB_YESNO);

			if (nYesNo == IDYES)
			{
				SetValue(NOTICE_FIELD_INDEX::TEMPLATE, strFpg);

				ClearAliasAndValue();

				GetAliases(pHDdaVinci);

			}
			else
			{
				SetValue(NOTICE_FIELD_INDEX::TEMPLATE, strPrevFpg);
			}
		}
		return TRUE;
	}

	BOOL IsAvailable(void)
	{
		BOOL bUse = GETINT(GetValue(NOTICE_FIELD_INDEX::USE));
		CString strFpgName = GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

		return (bUse && strFpgName != BLANK_STRING && GetKey() !=BLANK_STRING);
	}

	INT CountAlias(void) // 연령 알리아스를 제외한 알리아스 숫자를 구한다.
	{
		INT nCount = 0;
		for (INT nIndex = 0; nIndex < MAX_ALIAS_NUM; nIndex++)
		{
			if (GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nIndex) != BLANK_STRING && GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nIndex) != AGE_ALIAS)
			{
				nCount++;
			}
		}

		return nCount;
	}

	INT GetAliasIndex(CString& strAlias)
	{
		INT nIndex = 0;

		for (nIndex = 0; nIndex < MAX_ALIAS_NUM; nIndex++)
		{
			if (strAlias == GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nIndex))
			{
				return nIndex;
			}
		}

		return -1;
	}

	BOOL HasAlias(CHDdaVinci* pHDdaVinci,CString strAlias)
	{
		CString strFpgFileName = GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

		if (CFileUtils::ExistFile(strFpgFileName))
		{
			pHDdaVinci->Open(strFpgFileName, 1, NOTICE_PAGE_NUMBER);

			CCGXObject* pObject = pHDdaVinci->GetCGXObject(strAlias, NOTICE_PAGE_NUMBER);

			return (pObject != nullptr);

		}
		return FALSE;
	}

	INT GetAliases(CHDdaVinci* pHDdaVinci)
	{
		std::vector<CString> vAliases;

		CString strAlias;

		INT nIndex = 0;
		
		CStringList* pAliasStringList = new CStringList;

		CString strFpgFileName = GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

		if (CFileUtils::ExistFile(strFpgFileName))
		{
			pHDdaVinci->Open(strFpgFileName, 1, NOTICE_PAGE_NUMBER);
			pHDdaVinci->GetAlias(pAliasStringList, NOTICE_PAGE_NUMBER);

			ClearAliasAndValue();

			for (POSITION pos = pAliasStringList->GetHeadPosition(); pos != nullptr;)
			{
				strAlias = pAliasStringList->GetAt(pos);

				vAliases.emplace_back(strAlias);

				pAliasStringList->GetNext(pos);
			}

			std::sort(vAliases.begin(), vAliases.end());

			for (auto& iter : vAliases)
			{
				if (iter.CompareNoCase(AGE_ALIAS) != 0)
				{
					SetAlias(nIndex, iter);

					if (pHDdaVinci->GetCGXObject(iter, NOTICE_PAGE_NUMBER)->GetType() != CG_CMD_OBJECT_TEXT)
					{
						SetAliasType(nIndex, ALIAS_TYPE::IMAGE);
					}
					else
					{
						SetAliasType(nIndex, ALIAS_TYPE::TEXT);
					}

					nIndex++;
				}
			}
		}

		nIndex = min(MAX_ALIAS_NUM, nIndex);

		SAFE_DELETE(pAliasStringList);

		return nIndex;
	}

	CString GetAlias(INT nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_ALIAS_NUM) return BLANK_STRING;

		return GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nIndex);
	}

	void SetAlias(INT nIndex,CString strAlias)
	{
		if (nIndex < 0 || nIndex >= MAX_ALIAS_NUM) return;

		SetValue(NOTICE_FIELD_INDEX::ALIAS_1 + nIndex,strAlias);
	}

	CString GetAliasValue(INT nIndex)
	{
		if (nIndex < 0 || nIndex >= MAX_ALIAS_NUM) return BLANK_STRING;

		return GetValue(NOTICE_FIELD_INDEX::VALUE_1 + nIndex);
	}

	void SetAliasValue(INT nIndex, CString strValue)
	{
		if (nIndex < 0 || nIndex >= MAX_ALIAS_NUM) return;

		SetValue(NOTICE_FIELD_INDEX::VALUE_1 + nIndex, strValue);
	}

	void SetAliasValue(CString strAlias, CString strValue)
	{
		INT nIndex = GetAliasIndex(strAlias);

		SetAliasValue(nIndex, strValue);
	}

	ALIAS_TYPE GetAliasType(INT nIndex)
	{
		if (nIndex == -1) return ALIAS_TYPE::NONE;

		CString strFieldName = GetFieldName(nIndex);

		CString strType = GetValue(NOTICE_FIELD_INDEX::TYPE_1 + nIndex);

		return TO_E<ALIAS_TYPE>(GETINT(strType));
	}

	ALIAS_TYPE GetAliasType(CString& strAlias)
	{
		INT nIndex = GetAliasIndex(strAlias);

		return GetAliasType(nIndex);
	}

	void SetAliasType(INT nIndex, ALIAS_TYPE aliasType)
	{
		if (nIndex == -1) return;

		CString strType = GETSTR(FROM_E(aliasType));

		SetValue(NOTICE_FIELD_INDEX::TYPE_1 + nIndex, strType);

	}

	void SetAliasType(CString& strAlias, ALIAS_TYPE aliasType)
	{
		INT nIndex = GetAliasIndex(strAlias);

		return SetAliasType(nIndex, aliasType);
	}

	void ClearAliasAndValue(void)
	{
		for (INT nIndex = NOTICE_FIELD_INDEX::ALIAS_1; nIndex < NOTICE_FIELD_INDEX::LAST_NUM; nIndex++)
		{
			SetValue(nIndex, BLANK_STRING);
		}
	}

	BOOL IsEmptyAnnotation(void)
	{
		return GetValue(NOTICE_FIELD_INDEX::ANNOTATION).IsEmpty();
	}

	CString MakeAnnotation(void)
	{
		CString strReturn;

		for (INT i = 0; i < MAX_ALIAS_NUM; i++)
		{
			strReturn += (GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + i) == BLANK_STRING) ? BLANK_STRING :
				
				_T("[")+ GetValue(NOTICE_FIELD_INDEX::ALIAS_1 + i) +_T(":") + (
				
				(GetValue(NOTICE_FIELD_INDEX::TYPE_1 + i) == GETSTR(FROM_E(ALIAS_TYPE::IMAGE))) ?

				CFileUtils::ExtractFileName(GetValue(NOTICE_FIELD_INDEX::VALUE_1 + i))  :
				
				GetValue(NOTICE_FIELD_INDEX::VALUE_1 + i)) + _T("]");
		}

		SetValue(NOTICE_FIELD_INDEX::ANNOTATION, strReturn);

		return strReturn;
	}

	virtual ~CNoticeData() {};
};


class CNoticeDataList : public CTemplateList<CNoticeData>
{
	DECLARE_DYNAMIC(CNoticeDataList)

public:
	CNoticeDataList()
	{
		SetTableName(MakeKeyString());
		CFields fields(vNotice_Field);
		SetFields(fields);
	}


	BOOL HasAvailableData(void)
	{
		std::vector<CNoticeData> vNoticeData;

		SelectToObjectsWithField(vNoticeData, NOTICE_FIELD_INDEX::USE, TRUE);

		return (vNoticeData.size() > 0);
	}

	BOOL HasNoAgeNotice(CHDdaVinci* pHDdaVinci,CString& strFpg)
	{
		
		std::vector<CNoticeData> vNoticeData;

		SelectToObjectsWithField(vNoticeData, NOTICE_FIELD_INDEX::USE, TRUE);

		for (auto& iter : vNoticeData)
		{
			if (!iter.HasAlias(pHDdaVinci, AgeObjects[FROM_E(AGE_OBJECTS::AGE)]))
			{
				strFpg = iter.GetValue(NOTICE_FIELD_INDEX::TEMPLATE);

				return FALSE;
			}
		}

		strFpg = BLANK_STRING;

		return TRUE;
	}


	virtual ~CNoticeDataList() {};

};

struct SClockPos
{
	Point m_ptDate;
	Point m_ptHour;
	Point m_ptMinColon;
	Point m_ptTenMin;
	Point m_ptMin;
	Point m_ptSecColon;
	Point m_ptTenSec;
	Point m_ptSec;

	SClockPos() :
		m_ptDate(0, 0),
		m_ptHour(0, 0),
		m_ptMinColon(0, 0),
		m_ptTenMin(0, 0),
		m_ptMin(0, 0),
		m_ptSecColon(0, 0),
		m_ptTenSec(0, 0),
		m_ptSec(0, 0)
	{}

	~SClockPos() {}

	SClockPos& operator= (const SClockPos& rhs)
	{
		m_ptDate = rhs.m_ptDate;
		m_ptHour = rhs.m_ptHour;
		m_ptMinColon = rhs.m_ptMinColon;
		m_ptTenMin = rhs.m_ptTenMin;
		m_ptMin = rhs.m_ptMin;
		m_ptSecColon = rhs.m_ptSecColon;
		m_ptTenSec = rhs.m_ptTenSec;
		m_ptSec = rhs.m_ptSec;

		return *this;
	}
};


class CClockConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CClockConfig)

public:
	CClockConfig() :
	    m_strTitle(BLANK_STRING),
		m_bMoveEnable(TRUE),
		m_nMovingSize(5),
		m_bEntireMove(FALSE),
		m_bDateMove(FALSE),
		m_bTimeMove(FALSE),
		m_b24Hour(FALSE),
		m_bDispSecond(FALSE),
		m_bDispEveryHourSecond(FALSE),
		m_strSecOver(_T("55")),
		m_strSecUnder(_T("5")),
		m_bHideFirstDigit0OfHour(TRUE),
		m_ptDateOffset(0, 0),
		m_ptTimeOffset(0, 0),
		m_nFormatIndex(0),
		m_strFormat(DEFAULT_DATE_FORMAT)
	{
		m_vFormats.emplace_back(m_strFormat);
		m_vFormats.emplace_back(_T("%M월 %d일(%A)"));
	}

	~CClockConfig() {}

public:
	CString Title() const { return m_strTitle; }
	void Title(CString val) { m_strTitle = val; }

	BOOL MoveEnable() const { return m_bMoveEnable; }
	void MoveEnable(BOOL val) { m_bMoveEnable = val; }

	INT MovingSize() const { return m_nMovingSize; }
	void MovingSize(INT val) { m_nMovingSize = val; }

	BOOL EntireMove() const { return m_bEntireMove; }
	void EntireMove(BOOL val) { m_bEntireMove = val; }

	BOOL DateMove() const { return m_bDateMove; }
	void DateMove(BOOL val) { m_bDateMove = val; }

	BOOL TimeMove() const { return m_bTimeMove; }
	void TimeMove(BOOL val) { m_bTimeMove = val; }

	BOOL B24Hour() const { return m_b24Hour; }
	void B24Hour(BOOL val) { m_b24Hour = val; }

	BOOL DispSecond() const { return m_bDispSecond; }
	void DispSecond(BOOL val) { m_bDispSecond = val; }

	BOOL DispEveryHourSecond() const { return m_bDispEveryHourSecond; }
	void DispEveryHourSecond(BOOL val) { m_bDispEveryHourSecond = val; }

	CString SecOver() const { return m_strSecOver; }
	void SecOver(CString val) { m_strSecOver = val; }

	CString SecUnder() const { return m_strSecUnder; }
	void SecUnder(CString val) { m_strSecUnder = val; }

	BOOL HideFirstDigit0OfHour() const { return m_bHideFirstDigit0OfHour; }
	void HideFirstDigit0OfHour(BOOL val) { m_bHideFirstDigit0OfHour = val; }

	SClockPos& OrigPos() { return m_OrigPos; }
	void OrigPos(SClockPos& val) { m_OrigPos = val; }

	Point DateOffset() const { return m_ptDateOffset; }
	void DateOffset(Point val) { m_ptDateOffset = val; }

	Point TimeOffset() const { return m_ptTimeOffset; }
	void TimeOffset(Point val) { m_ptTimeOffset = val; }

	INT FormatIndex() const { return m_nFormatIndex; }
	void FormatIndex(INT val) { m_nFormatIndex = val; }

	CString Format() const { return m_strFormat; }
	void Format(CString val) { m_strFormat = val; }

	std::vector<CString> Formats() const { return m_vFormats; }
	void Formats(std::vector<CString>& val) { m_vFormats = val; }

	CClockConfig& operator= (const CClockConfig& rhs)
	{
		m_strTitle = rhs.m_strTitle;
		m_bMoveEnable = rhs.m_bMoveEnable;
		m_nMovingSize = rhs.m_nMovingSize;
		m_bEntireMove = rhs.m_bEntireMove;
		m_bDateMove = rhs.m_bDateMove;
		m_bTimeMove = rhs.m_bTimeMove;
		m_b24Hour = rhs.m_b24Hour;

		m_bDispSecond = rhs.m_bDispSecond;
		m_bDispEveryHourSecond = rhs.m_bDispEveryHourSecond;
		m_strSecOver = rhs.m_strSecOver;
		m_strSecUnder = rhs.m_strSecUnder;

		m_bHideFirstDigit0OfHour = rhs.m_bHideFirstDigit0OfHour;

//		m_OrigPos = rhs.m_OrigPos;
		m_ptDateOffset = rhs.m_ptDateOffset;
		m_ptTimeOffset = rhs.m_ptTimeOffset;

		m_nFormatIndex = rhs.m_nFormatIndex;
		m_strFormat = rhs.m_strFormat;
		m_vFormats = rhs.m_vFormats;

		return *this;
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strTitle;
			ar << m_bMoveEnable;
			ar << m_nMovingSize;
			ar << m_bEntireMove;
			ar << m_bDateMove;
			ar << m_bTimeMove;

			ar << m_b24Hour;

			ar << m_bDispSecond;
			ar << m_bDispEveryHourSecond;
			ar << m_strSecOver;
			ar << m_strSecUnder;

			ar << m_bHideFirstDigit0OfHour;

			ar << m_ptDateOffset.X;
			ar << m_ptDateOffset.Y;
			ar << m_ptTimeOffset.X;
			ar << m_ptTimeOffset.Y;

			ar << m_nFormatIndex;
			ar << m_strFormat;


		}
		else
		{
			ar >> m_strTitle;
			ar >> m_bMoveEnable;
			ar >> m_nMovingSize;
			ar >> m_bEntireMove;
			ar >> m_bDateMove;
			ar >> m_bTimeMove;

			ar >> m_b24Hour;

			ar >> m_bDispSecond;
			ar >> m_bDispEveryHourSecond;
			ar >> m_strSecOver;
			ar >> m_strSecUnder;

			ar >> m_bHideFirstDigit0OfHour;

			ar >> m_ptDateOffset.X;
			ar >> m_ptDateOffset.Y;
			ar >> m_ptTimeOffset.X;
			ar >> m_ptTimeOffset.Y;

			ar >> m_nFormatIndex;
			ar >> m_strFormat;
		}

		Serialize_Vector<CString>(ar, m_vFormats);
	}

private:
	CString m_strTitle;
	BOOL m_bMoveEnable;
	INT  m_nMovingSize;
	BOOL m_bEntireMove;
	BOOL m_bDateMove;
	BOOL m_bTimeMove;
	BOOL m_b24Hour;
	BOOL m_bDispSecond;
	BOOL m_bDispEveryHourSecond;
	CString m_strSecOver;
	CString m_strSecUnder;
	BOOL m_bHideFirstDigit0OfHour;
	SClockPos m_OrigPos;
	Point m_ptDateOffset;
	Point m_ptTimeOffset;
	INT m_nFormatIndex;

	CString m_strFormat;
	std::vector<CString> m_vFormats;
};

class CBigClockConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CBigClockConfig)

	CBigClockConfig() :
	    m_strTitle(BLANK_STRING),
		m_bMoveEnable(TRUE),
		m_nMovingSize(5),
		m_ptOffset(0, 0)
	{}

	~CBigClockConfig() {}

public:
	CString Title(void) const { return m_strTitle; }
	void Title(CString val) { m_strTitle = val; }

	BOOL MoveEnable() const { return m_bMoveEnable; }
	void MoveEnable(BOOL val) { m_bMoveEnable = val; }

	INT MovingSize() const { return m_nMovingSize; }
	void MovingSize(INT val) { m_nMovingSize = val; }

	Point Offset() const { return m_ptOffset; }
	void Offset(Point val) { m_ptOffset = val; }

	SClockPos& OrigPos() { return m_OrigPos; }
	void OrigPos(SClockPos& val) { m_OrigPos = val; }


	CBigClockConfig& operator= (const CBigClockConfig& rhs)
	{
		m_strTitle = rhs.m_strTitle;
		m_bMoveEnable = rhs.m_bMoveEnable;
		m_nMovingSize = rhs.m_nMovingSize;
		m_ptOffset = rhs.m_ptOffset;
//		m_OrigPos = rhs.m_OrigPos;

		return *this;
	}

	void Serialize(CArchive& ar) override
	{

		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strTitle;
			ar << m_bMoveEnable;
			ar << m_nMovingSize;
			ar << m_ptOffset.X;
			ar << m_ptOffset.Y;
		}
		else
		{
			ar >> m_strTitle;
			ar >> m_bMoveEnable;
			ar >> m_nMovingSize;
			ar >> m_ptOffset.X;
			ar >> m_ptOffset.Y;
		}
	}

private:
	CString m_strTitle;
	BOOL m_bMoveEnable;
	INT m_nMovingSize;
	Point m_ptOffset;
	SClockPos m_OrigPos;
};

class CLogoConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CLogoConfig)

	CLogoConfig() :
	    m_strTitle(BLANK_STRING),
		m_bMoveEnable(TRUE),
		m_nMovingSize(5),
		m_bEntireMove(FALSE),
		m_bLogoMove(FALSE),
		m_bLiveMove(FALSE),
		m_bUHDMove(FALSE),
		m_bDongSiMove(FALSE),

		m_bEntireShow(FALSE),
		m_bLogoShow(TRUE),
		m_bLiveShow(FALSE),
		m_bUHDShow(FALSE),
		m_bDongSiShow(FALSE),

		m_nKBSLogo(FROM_E<STATION_LOGO>(STATION_LOGO::KBS_1)),

		m_ptLogoOffset(0, 0),
		m_ptLiveOffset(0, 0),
		m_ptUHDOffset(0, 0),
		m_ptDongSiOffset(0, 0)
	{}

	~CLogoConfig() {}


public:
	CString Title() const { return m_strTitle; }
	void Title(CString val) { m_strTitle = val; }
	BOOL MoveEnable() const { return m_bMoveEnable; }
	void MoveEnable(BOOL val) { m_bMoveEnable = val; }
	INT MovingSize() const { return m_nMovingSize; }
	void MovingSize(INT val) { m_nMovingSize = val; }
	BOOL EntireMove() const { return m_bEntireMove; }
	void EntireMove(BOOL val) { m_bEntireMove = val; }
	BOOL LogoMove() const { return m_bLogoMove; }
	void LogoMove(BOOL val) { m_bLogoMove = val; }
	BOOL LiveMove() const { return m_bLiveMove; }
	void LiveMove(BOOL val) { m_bLiveMove = val; }
	BOOL UHDMove() const { return m_bUHDMove; }
	void UHDMove(BOOL val) { m_bUHDMove = val; }
	BOOL DongSiMove() const { return m_bDongSiMove; }
	void DongSiMove(BOOL val) { m_bDongSiMove = val; }
	BOOL EntireShow() const { return m_bEntireShow; }
	void EntireShow(BOOL val) { m_bEntireShow = val; }
	BOOL LogoShow() const { return m_bLogoShow; }
	void LogoShow(BOOL val) { m_bLogoShow = val; }
	BOOL LiveShow() const { return m_bLiveShow; }
	void LiveShow(BOOL val) { m_bLiveShow = val; }
	BOOL UHDShow() const { return m_bUHDShow; }
	void UHDShow(BOOL val) { m_bUHDShow = val; }
	BOOL DongSiShow() const { return m_bDongSiShow; }
	void DongSiShow(BOOL val) { m_bDongSiShow = val; }

	INT KBSLogo() const { return m_nKBSLogo; }
	void KBSLogo(INT val) { m_nKBSLogo = val; }

	Point LogoOffset() const { return m_ptLogoOffset; }
	void LogoOffset(Point val) { m_ptLogoOffset = val; }
	Point LiveOffset() const { return m_ptLiveOffset; }
	void LiveOffset(Point val) { m_ptLiveOffset = val; }
	Point UHDOffset() const { return m_ptUHDOffset; }
	void UHDOffset(Point val) { m_ptUHDOffset = val; }
	Point DongSiOffset() const { return m_ptDongSiOffset; }
	void DongSiOffset(Point val) { m_ptDongSiOffset = val; }

	BOOL IsAnimationLogo(void) { return (m_nKBSLogo == FROM_E(STATION_LOGO::KBS_ANI)); }

	CLogoConfig& operator= (const CLogoConfig& rhs)
	{
		m_strTitle = rhs.m_strTitle;
		m_bMoveEnable = rhs.m_bMoveEnable;
		m_nMovingSize = rhs.m_nMovingSize;
		m_bEntireMove = rhs.m_bEntireMove;
		m_bLogoMove = rhs.m_bLogoMove;
		m_bLiveMove = rhs.m_bLiveMove;
		m_bUHDMove = rhs.m_bUHDMove;
		m_bDongSiMove = rhs.m_bDongSiMove;

		m_bEntireShow = rhs.m_bEntireShow;
		m_bLogoShow = rhs.m_bLogoShow;
		m_bLiveShow = rhs.m_bLiveShow;
		m_bUHDShow = rhs.m_bUHDShow;
		m_bDongSiShow = rhs.m_bDongSiShow;

		m_nKBSLogo = rhs.m_nKBSLogo;

		m_ptLogoOffset = rhs.m_ptLogoOffset;
		m_ptLiveOffset = rhs.m_ptLiveOffset;
		m_ptUHDOffset = rhs.m_ptUHDOffset;
		m_ptDongSiOffset = rhs.m_ptDongSiOffset;

		return *this;
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strTitle;
			ar << m_bMoveEnable;
			ar << m_nMovingSize;
			ar << m_bEntireMove;
			ar << m_bLogoMove;
			ar << m_bLiveMove;
			ar << m_bUHDMove;
			ar << m_bDongSiMove;

			ar << m_bEntireShow;
			ar << m_bLogoShow;
			ar << m_bLiveShow;
			ar << m_bUHDShow;
			ar << m_bDongSiShow;

			ar << m_nKBSLogo;

			ar << m_ptLogoOffset.X;
			ar << m_ptLogoOffset.Y;
			ar << m_ptLiveOffset.X;
			ar << m_ptLiveOffset.Y;
			ar << m_ptUHDOffset.X;
			ar << m_ptUHDOffset.Y;
			ar << m_ptDongSiOffset.X;
			ar << m_ptDongSiOffset.Y;
		}
		else
		{
			ar >> m_strTitle;
			ar >> m_bMoveEnable;
			ar >> m_nMovingSize;
			ar >> m_bEntireMove;
			ar >> m_bLogoMove;
			ar >> m_bLiveMove;
			ar >> m_bUHDMove;
			ar >> m_bDongSiMove;

			ar >> m_bEntireShow;
			ar >> m_bLogoShow;
			ar >> m_bLiveShow;
			ar >> m_bUHDShow;
			ar >> m_bDongSiShow;

			ar >> m_nKBSLogo;

			ar >> m_ptLogoOffset.X;
			ar >> m_ptLogoOffset.Y;
			ar >> m_ptLiveOffset.X;
			ar >> m_ptLiveOffset.Y;
			ar >> m_ptUHDOffset.X;
			ar >> m_ptUHDOffset.Y;
			ar >> m_ptDongSiOffset.X;
			ar >> m_ptDongSiOffset.Y;
		}
	}


private:
	CString m_strTitle;
	BOOL m_bMoveEnable;
	INT  m_nMovingSize;
	BOOL m_bEntireMove;
	BOOL m_bLogoMove;
	BOOL m_bLiveMove;
	BOOL m_bUHDMove;
	BOOL m_bDongSiMove;
	BOOL m_bEntireShow;
	BOOL m_bLogoShow;
	BOOL m_bLiveShow;
	BOOL m_bUHDShow;
	BOOL m_bDongSiShow;

	INT  m_nKBSLogo;

	Point m_ptLogoOffset;
	Point m_ptLiveOffset;
	Point m_ptUHDOffset;
	Point m_ptDongSiOffset;
};

class CNTPConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CNTPConfig)

public:
	CNTPConfig() :
		m_nServerIndex(0),
		m_strServerName(_T("time.windows.com")),
		m_bAutoSync(TRUE),
		m_nIntervalIndex(1),
		m_strInterval(_T("60")),
		m_strAdjustTime(ZERO_STRING)
	{}
	~CNTPConfig() {}


public:
	INT ServerIndex() const { return m_nServerIndex; }
	void ServerIndex(INT val) { m_nServerIndex = val; }

	CString ServerName() const { return m_strServerName; }
	void ServerName(CString val) { m_strServerName = val; }
	const BOOL AutoSync() const { return m_bAutoSync; }
	void AutoSync(BOOL val) { m_bAutoSync = val; }

	INT IntervalIndex() const { return m_nIntervalIndex; }
	void IntervalIndex(INT val) { m_nIntervalIndex = val; }
	CString Interval() const { return m_strInterval; }
	void Interval(CString val) { m_strInterval = val; }

	CString AdjustTime() const { return m_strAdjustTime; }
	void AdjustTime(CString val) { m_strAdjustTime = val; }

	const CNTPConfig& operator= (const CNTPConfig& rhs)
	{
		ServerIndex(rhs.ServerIndex());
		ServerName(rhs.ServerName());
		AutoSync(rhs.AutoSync());
		IntervalIndex(rhs.IntervalIndex());
		Interval(rhs.Interval());
		AdjustTime(rhs.AdjustTime());

		return *this;
	}

	void Serialize(CArchive& ar) override
	{

		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_nServerIndex;
			ar << m_strServerName;
			ar << m_bAutoSync;
			ar << m_nIntervalIndex;
			ar << m_strInterval;
			ar << m_strAdjustTime;
		}
		else
		{
			ar >> m_nServerIndex;
			ar >> m_strServerName;
			ar >> m_bAutoSync;
			ar >> m_nIntervalIndex;
			ar >> m_strInterval;
			ar >> m_strAdjustTime;
		}
	}

private:
	INT     m_nServerIndex;
	CString m_strServerName;
	BOOL m_bAutoSync;
	INT m_nIntervalIndex;
	CString m_strInterval;
	CString m_strAdjustTime;

};

class CNewsConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CNewsConfig)

	CNewsConfig() :
		m_strScrollTemplate(BLANK_STRING),
		m_nScrollIteration(1),
		m_nScrollSpeed(3),
		m_nScrollPrefixGap(30),
		m_nScrollItemGap(100),

		m_strCategory(_T("지역뉴스")),
		m_strTextSize(_T("100")),
		m_strSymbolFont(_T("KBSsymbol1")),
		m_strSymbolSize(_T("100")),
		m_strHanjaSize(_T("100")),

		m_ulTextColor(XML_TAG_DEFAULT_TEXT_COLOR),
		m_ulBackColor(XML_TAG_DEFAULT_BACK_COLOR),

		m_strDefaultFontName(XML_TAG_DEFAULT_FONT),

		m_bApplyScrollColor(FALSE),
		m_nDisplayDuration(7)

	{
		m_strScrollTemplate = _T("News_Scroll.FPG");

		m_vCategories.clear();

		m_vCategories.emplace_back(_T("지역뉴스"));
		m_vCategories.emplace_back(_T("주요뉴스"));
		m_vCategories.emplace_back(_T("정치"));
		m_vCategories.emplace_back(_T("경제"));
		m_vCategories.emplace_back(_T("문화"));
		m_vCategories.emplace_back(_T("사회"));
		m_vCategories.emplace_back(_T("국제"));
		m_vCategories.emplace_back(_T("교육"));
		m_vCategories.emplace_back(_T("과학"));
		m_vCategories.emplace_back(_T("스포츠"));
		m_vCategories.emplace_back(_T("인터넷뉴스"));
		m_vCategories.emplace_back(_T("날씨"));
		m_vCategories.emplace_back(_T("영상제보"));

		m_vTextSizes.clear();

		m_vTextSizes.emplace_back(_T("70"));
		m_vTextSizes.emplace_back(_T("80"));
		m_vTextSizes.emplace_back(_T("90"));
		m_vTextSizes.emplace_back(_T("100"));
		m_vTextSizes.emplace_back(_T("110"));
		m_vTextSizes.emplace_back(_T("120"));
		m_vTextSizes.emplace_back(_T("130"));
		m_vTextSizes.emplace_back(_T("140"));
		m_vTextSizes.emplace_back(_T("150"));
		m_vTextSizes.emplace_back(_T("200"));

	}
	~CNewsConfig() {}

public:
	CString ScrollTemplate() const { return m_strScrollTemplate; }
	void ScrollTemplate(CString val) { m_strScrollTemplate = val; }

	INT ScrollIteration() const { return m_nScrollIteration; }
	void ScrollIteration(INT val) { m_nScrollIteration = val; }

	INT ScrollSpeed() const { return m_nScrollSpeed; }
	void ScrollSpeed(INT val) { m_nScrollSpeed = val; }

	INT ScrollPrefixGap() const { return m_nScrollPrefixGap; }
	void ScrollPrefixGap(INT val) { m_nScrollPrefixGap = val; }

	INT ScrollItemGap() const { return m_nScrollItemGap; }
	void ScrollItemGap(INT val) { m_nScrollItemGap = val; }

	CString Category() const { return m_strCategory; }
	void Category(CString val) { m_strCategory = val; }

	std::vector<CString> Categories() const { return m_vCategories; }
	void Categories(std::vector<CString>& val) { m_vCategories = val; }

	void AddCategory(CString strCategory) { m_vCategories.emplace_back(strCategory);}

	CString TextSize() const { return m_strTextSize; }
	void TextSize(CString val) { m_strTextSize = val; }

	std::vector<CString> TextSizes() const { return m_vTextSizes; }
	void TextSizes(std::vector<CString> val) { m_vTextSizes = val; }

	ULONG TextColor() const { return m_ulTextColor; }
	void TextColor(ULONG val) { m_ulTextColor = val; }

	ULONG BackColor() const { return m_ulBackColor; }
	void BackColor(ULONG val) { m_ulBackColor = val; }

	CString DefaultFontName() const { return m_strDefaultFontName; }
	void DefaultFontName(CString val) { m_strDefaultFontName = val; }

	BOOL ApplyScrollColor() const { return m_bApplyScrollColor; }
	void ApplyScrollColor(BOOL val) { m_bApplyScrollColor = val; }

	INT DisplayDuration() const { return m_nDisplayDuration; }
	void DisplayDuration(INT val) { m_nDisplayDuration = val; }

	CString SymbolFont() const { return m_strSymbolFont; }
	void SymbolFont(CString val) { m_strSymbolFont = val; }


	CString SymbolSize() const { return m_strSymbolSize; }
	void SymbolSize(CString val) { m_strSymbolSize = val; }

	CString HanjaSize() const { return m_strHanjaSize; }
	void HanjaSize(CString val) { m_strHanjaSize = val; }

	CNewsData& CurNewsData() { return m_curNewsData; }
	void CurNewsData(CNewsData& val) { m_curNewsData = val; }

	CString ImageShortCut(INT nIndex) { return m_strImageShortCut[nIndex]; }
	void ImageShortCut(INT nIndex, CString val) { m_strImageShortCut[nIndex] = val; }

	const CNewsConfig& operator= (const CNewsConfig& rhs)
	{
		m_strScrollTemplate = rhs.m_strScrollTemplate;
		m_nScrollIteration = rhs.m_nScrollIteration;
		m_nScrollSpeed = rhs.m_nScrollSpeed;
		m_nScrollPrefixGap = rhs.m_nScrollPrefixGap;
		m_nScrollItemGap = rhs.m_nScrollItemGap;

		m_strCategory = rhs.m_strCategory;
		m_vCategories = rhs.m_vCategories;
		m_strTextSize = rhs.m_strTextSize;

		m_ulTextColor = rhs.m_ulTextColor;
		m_ulBackColor = rhs.m_ulBackColor;

		m_strDefaultFontName = rhs.m_strDefaultFontName;

		m_bApplyScrollColor = rhs.m_bApplyScrollColor;
		m_nDisplayDuration = rhs.m_nDisplayDuration;

		SymbolFont(rhs.SymbolFont());
		SymbolSize(rhs.SymbolSize());

		return *this;
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strScrollTemplate;
			ar << m_nScrollIteration;
			ar << m_nScrollSpeed;
			ar << m_nScrollPrefixGap;
			ar << m_nScrollItemGap;

			ar << m_strCategory;
			ar << m_strTextSize;

			ar << m_bApplyScrollColor;
			ar << m_nDisplayDuration;

			for (const auto& iter : m_strImageShortCut)
			{
				ar << iter;
			}
		}
		else
		{
			ar >> m_strScrollTemplate;
			ar >> m_nScrollIteration;
			ar >> m_nScrollSpeed;
			ar >> m_nScrollPrefixGap;
			ar >> m_nScrollItemGap;

			ar >> m_strCategory;
			ar >> m_strTextSize;

			ar >> m_bApplyScrollColor;
			ar >> m_nDisplayDuration;

			for (auto& iter : m_strImageShortCut)
			{
				ar >> iter;
			}

		}

		Serialize_Vector<CString>(ar, m_vCategories);
		Serialize_Vector<CString>(ar, m_vTextSizes);

	}

	CString GetCategoriesForSpread(void)
	{
		CString strResult;

		for (auto& iter : m_vCategories)
		{
			strResult += iter + TAB_STRING;
		}

		strResult.TrimRight(TAB_STRING);

		return strResult;
	}


private:
	CString  m_strScrollTemplate;
	INT      m_nScrollIteration;
	INT      m_nScrollSpeed;
	INT      m_nScrollPrefixGap;
	INT      m_nScrollItemGap;
	CString   m_strCategory;
	std::vector<CString> m_vCategories;
	CString      m_strTextSize;
	std::vector<CString> m_vTextSizes;
	ULONG   m_ulTextColor;
	ULONG   m_ulBackColor;
	CString m_strDefaultFontName;
	BOOL  m_bApplyScrollColor;
	INT   m_nDisplayDuration;

	CString m_strSymbolFont;
	CString m_strSymbolSize;
	CString m_strHanjaSize;

	CNewsData m_curNewsData;

	CString m_strImageShortCut[IMAGE_SHORTCUT_NUM];

};

class CWeatherConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CWeatherConfig)

	CWeatherConfig() :
		m_nAutoReadingPeriod(20),
		m_bAutoReading(TRUE),
		m_bExceptAir(FALSE),
		m_nDisplayDuration(7)
	{
		LoadCodeFromXls(WEATHER_CODE_INFO_XLS_FILENAME);
	}

	~CWeatherConfig() {}

private:
	UINT    m_nAutoReadingPeriod;
	BOOL    m_bAutoReading;
	UINT    m_nDisplayDuration;
	BOOL    m_bExceptAir;
	std::map<INT, CString> m_mapCode_Hdj;
	std::map<INT, BOOL> m_mapCode_SelectCity;

public:
	const CWeatherConfig& operator= (const CWeatherConfig& rhs)
	{
		m_nAutoReadingPeriod = rhs.m_nAutoReadingPeriod;
		m_bAutoReading = rhs.m_bAutoReading;

		m_mapCode_Hdj = rhs.m_mapCode_Hdj;
		m_mapCode_SelectCity = rhs.m_mapCode_SelectCity;

		return *this;
	}

	bool LoadCodeFromXls(CString strXlsFileName)
	{
		Book* book = xlCreateBook();
		book->setKey(LIBXL_KEY_1, LIBXL_KEY_2);
		CString strSheetName;
		bool bResult = false;

		BOOL bExist = CFileUtils::ExistFile(strXlsFileName);

		try
		{
			m_mapCode_Hdj.clear();
			m_mapCode_SelectCity.clear();

			if (book->load(strXlsFileName))
			{
				int nCount = book->sheetCount();

				Sheet* sheet = book->getSheet(0);  // 시도 코드 시트

				int nLastRow = sheet->lastRow();

				if (sheet)
				{
					for (int nRow = 1; nRow < nLastRow; nRow++)  // 타이틀 줄을 빼고 읽어 들인다.
					{
						CString strHjdCode = sheet->readStr(nRow, 0);
						INT nCityCode = GETINT(sheet->readStr(nRow, 3));

						if (nCityCode != 0)
						{
							if (!strHjdCode.IsEmpty())
							{
								m_mapCode_Hdj[nCityCode] = strHjdCode;
							}

							m_mapCode_SelectCity[nCityCode] = FALSE;
						}
					}

				}
				PrintLog_Message(strXlsFileName + _T("가져오기 성공!!"), FALSE);
				bResult = true;

			}
			else
			{
				PrintLog_Message(strXlsFileName + _T(" 가져오기 실패!!!"), TRUE);
				bResult = false;
			}
		}
		catch (CException* e)
		{
			PrintLog_Message(CString(book->errorMessage()), TRUE);
			bResult = false;
		}

		book->release();

		return bResult;
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		INT nSize = 0;

		if (ar.IsStoring())
		{
			ar << m_nAutoReadingPeriod;
			ar << m_bAutoReading;
			ar << m_nDisplayDuration;
		}
		else
		{
			ar >> m_nAutoReadingPeriod;
			ar >> m_bAutoReading;
			ar >> m_nDisplayDuration;
		}

		Serialize_Map<INT, BOOL>(ar, m_mapCode_SelectCity);
	}

	CString GetHdjFromCode(INT nCity)
	{
		if (nCity >= NUM_CITY) return BLANK_STRING;

		if (m_mapCode_SelectCity.find(nCity) != m_mapCode_SelectCity.end())
		{
			return m_mapCode_Hdj[nCity];
		}

		return BLANK_STRING;
	}

	BOOL IsSelectedCity(INT nCity)
	{
		if (nCity >= NUM_CITY) return FALSE;

		if (m_mapCode_SelectCity.find(nCity) != m_mapCode_SelectCity.end())
		{
			return m_mapCode_SelectCity[nCity];
		}

		return FALSE;
	}

	void SelectCity(INT nCity, BOOL bVal)
	{
		if (nCity >= NUM_CITY) return;

		if (m_mapCode_SelectCity.find(nCity) != m_mapCode_SelectCity.end())
		{
			m_mapCode_SelectCity[nCity] = bVal;
		}
	}

	void DeSelectAllCity(void)
	{
		for (auto& iter : m_mapCode_SelectCity)
		{
			iter.second = FALSE;
		}
	}

	void SelectAllCity(void)
	{
		for (auto& iter : m_mapCode_SelectCity)
		{
			iter.second = TRUE;
		}
	}

	bool ExistSelectCity(void)
	{
		for (auto& iter : m_mapCode_SelectCity)
		{
			if (iter.second) return true;
		}

		return false;
	}

	std::map<INT, CString> MapCode_Hdj() { return m_mapCode_Hdj; }

	std::map<INT, BOOL> MapCode_SelectCity() { return m_mapCode_SelectCity; }

	void MapCode_SelectCity(std::map<INT, BOOL>& mapSelectCity) { m_mapCode_SelectCity = mapSelectCity; }
	BOOL AutoReading() const { return m_bAutoReading; }
	void AutoReading(BOOL val) { m_bAutoReading = val; }


	UINT AutoReadingPeriod() { return m_nAutoReadingPeriod; }
	void AutoReadingPeriod(UINT val) { m_nAutoReadingPeriod = val; }

	UINT DisplayDuration() { return m_nDisplayDuration; }
	void DisplayDuration(UINT val) { m_nDisplayDuration = val; }

	BOOL ExceptAir() { return m_bExceptAir; }
	void ExceptAir(BOOL bExceptAir) { m_bExceptAir = bExceptAir; }
};

class CNoticeConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CNoticeConfig)

	CNoticeConfig() :
		m_nAgeMode(FROM_E(AGE_MODE::AGE_ALL)),
		m_nRepetition(999)
	{
	}

	~CNoticeConfig() {}

private:
	INT m_nAgeMode;
	INT m_nRepetition;
	std::vector<CString> m_vTemplateFiles;

public:
	const CNoticeConfig& operator= (const CNoticeConfig& rhs)
	{
		m_nAgeMode = rhs.m_nAgeMode;
		m_nRepetition = rhs.m_nRepetition;

		return *this;
	}


	CString TemplateFilesForSpread()
	{ 
		return GetStringFromVector(m_vTemplateFiles, TAB_STRING);
	}

	std::vector<CString> TemplateFiles(void) { return m_vTemplateFiles; }

	void RefreshTemplateFiles(void) {

		std::vector<CString> vNoticeFiles;

		CFileUtils::GetFileList(vNoticeFiles, GetNoticeDir(), CString(FPG_EXT).Trim(_T(".")));
		

		m_vTemplateFiles.clear();
		for (auto& iter : vNoticeFiles)
		{
			m_vTemplateFiles.emplace_back(CFileUtils::ExtractFileName(iter));
		}

		m_vTemplateFiles.emplace_back(BLANK_STRING);
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_nAgeMode;
			ar << m_nRepetition;
		}
		else
		{
			ar >> m_nAgeMode;
			ar >> m_nRepetition;
		}
	}

	INT AgeMode() const { return m_nAgeMode; }
	void AgeMode(INT val) { m_nAgeMode = val; }
	INT Repetition() const { return m_nRepetition; }
	void Repetition(INT val) { m_nRepetition = val; }

};

class CManualUpConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CManualUpConfig)

	CManualUpConfig()
	{
		m_strManualUpTemplate = _T("ManualUp_Default.FPG");
	}
	~CManualUpConfig() {}

public:
	CString ManualUpTemplate() const { return m_strManualUpTemplate; }
	void ManualUpTemplate(CString val) { m_strManualUpTemplate = val; }

	const CManualUpConfig& operator= (const CManualUpConfig& rhs)
	{
		m_strManualUpTemplate = rhs.m_strManualUpTemplate;

		return *this;
	}

	void Serialize(CArchive& ar) override
	{
		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strManualUpTemplate;
		}
		else
		{
			ar >> m_strManualUpTemplate;
		}
	}

private:
	CString  m_strManualUpTemplate;
};


struct SLiveVideoPos
{
	Point m_ptLiveVideo; 
	Point m_ptMask;

	SLiveVideoPos() :
		m_ptLiveVideo(0, 0),
		m_ptMask(0,0)
	{}

	~SLiveVideoPos() {}

	SLiveVideoPos& operator= (const SLiveVideoPos& rhs)
	{
		m_ptLiveVideo = rhs.m_ptLiveVideo;
		m_ptMask = rhs.m_ptMask;

		return *this;
	}
};


class CLiveVideoConfig : public CClockDataObject
{
	DECLARE_DYNAMIC(CLiveVideoConfig)

	CLiveVideoConfig() :
		m_strTitle(BLANK_STRING),
		m_bMoveEnable(TRUE),
		m_nMovingSize(5),
		m_ptOffset(0, 0),
		m_strTemplate(DEFAULT_LIVE_VIDEO_TEMPLATE)
	{}

	~CLiveVideoConfig() {}

public:
	CString Title() const { return m_strTitle; }
	void Title(CString val) { m_strTitle = val; }

	BOOL MoveEnable() const { return m_bMoveEnable; }
	void MoveEnable(BOOL val) { m_bMoveEnable = val; }

	INT MovingSize() const { return m_nMovingSize; }
	void MovingSize(INT val) { m_nMovingSize = val; }

	Point Offset() const { return m_ptOffset; }
	void Offset(Point val) { m_ptOffset = val; }

	SLiveVideoPos& OrigPos() { return m_OrigPos; }
	void OrigPos(SLiveVideoPos& val) { m_OrigPos = val; }

	CString Template() {
		return m_strTemplate;
	}

	void Template(CString& strTemplate) { m_strTemplate = strTemplate; }


	CLiveVideoConfig& operator= (const CLiveVideoConfig& rhs)
	{
		m_strTitle = rhs.m_strTitle;
		m_bMoveEnable = rhs.m_bMoveEnable;
		m_nMovingSize = rhs.m_nMovingSize;
		m_ptOffset = rhs.m_ptOffset;
		m_OrigPos = rhs.m_OrigPos;
	//	m_strTemplate = rhs.m_strTemplate;

		return *this;
	}

	void Serialize(CArchive& ar) override
	{

		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
			ar << m_strTitle;
			ar << m_bMoveEnable;
			ar << m_nMovingSize;
			ar << m_ptOffset.X;
			ar << m_ptOffset.Y;
		//	ar << m_strTemplate;
		}
		else
		{
			ar >> m_strTitle;
			ar >> m_bMoveEnable;
			ar >> m_nMovingSize;
			ar >> m_ptOffset.X;
			ar >> m_ptOffset.Y;
		//	ar >> m_strTemplate;
		}
	}

private:
	CString m_strTitle;
	BOOL m_bMoveEnable;
	INT m_nMovingSize;
	Point m_ptOffset;
	SLiveVideoPos m_OrigPos;
	CString m_strTemplate;
};


enum  UI_FONT {
	FONT_BUTTON_LARGE = 0,
	FONT_BUTTON_MEDIUM,
	FONT_EDIT_LARGE,
	FONT_EDIT_MEDIUM,
	FONT_STATIC_LARGE,
	FONT_STATIC_MEDIUM,
	FONT_STATIC_TIMER,
	FONT_EDIT_SCORE,
	FONT_STATIC_BOLD,
	FONT_COMBO_SMALL,
	MAX_FONT_NUM
};

class CUIFonts : public CObject
{
	DECLARE_DYNAMIC(CUIFonts)

public:
	CUIFonts() :
		m_pFontButtonLarge(new CFont()),
		m_pFontButtonMedium(new CFont()),
		m_pFontEditLarge(new CFont()),
		m_pFontEditMedium(new CFont()),
		m_pFontStaticLarge(new CFont()),
		m_pFontStaticMedium(new CFont()),
		m_pFontStaticTimer(new CFont()),
		m_pFontEditScore(new CFont()),
		m_pFontStaticBold(new CFont()),
		m_pFontComboSmall(new CFont())
	{
		SetFont(_T("맑은고딕"), CSize(-30, 20), FW_BOLD, m_pFontButtonLarge);
		SetFont(_T("맑은고딕"), CSize(-30, 10), FW_BOLD, m_pFontButtonMedium);
		SetFont(_T("맑은고딕"), CSize(-30, 20), FW_BOLD, m_pFontEditLarge);
		SetFont(_T("맑은고딕"), CSize(-30, 10), FW_BOLD, m_pFontEditMedium);
		SetFont(_T("맑은고딕"), CSize(-30, 20), FW_BOLD, m_pFontStaticLarge);
		SetFont(_T("맑은고딕"), CSize(-30, 10), FW_BOLD, m_pFontStaticMedium);
		SetFont(_T("맑은고딕"), CSize(30, 20), FW_BOLD, m_pFontStaticTimer);
		SetFont(_T("맑은고딕"), CSize(-30, 10), FW_BOLD, m_pFontEditScore);
		SetFont(_T("맑은고딕"), CSize(-20, 10), FW_BOLD, m_pFontStaticBold);

		SetFont(_T("YD윤고딕 340"), CSize(15, 8), FW_BOLD, m_pFontComboSmall);
	}

	~CUIFonts() {
		SAFE_DELETE(m_pFontButtonLarge);
		SAFE_DELETE(m_pFontButtonMedium);
		SAFE_DELETE(m_pFontEditLarge);
		SAFE_DELETE(m_pFontEditMedium);
		SAFE_DELETE(m_pFontStaticLarge);
		SAFE_DELETE(m_pFontStaticMedium);
		SAFE_DELETE(m_pFontStaticTimer);
		SAFE_DELETE(m_pFontEditScore);
		SAFE_DELETE(m_pFontStaticBold);
		SAFE_DELETE(m_pFontComboSmall);

	}

private:
	CFont* m_pFontButtonLarge;
	CFont* m_pFontButtonMedium;
	CFont* m_pFontEditLarge;
	CFont* m_pFontEditMedium;
	CFont* m_pFontStaticLarge;
	CFont* m_pFontStaticMedium;
	CFont* m_pFontStaticTimer;
	CFont* m_pFontEditScore;
	CFont* m_pFontStaticBold;
	CFont* m_pFontComboSmall;

public:

	void SetFont(CString strFontName, CSize sizeFont, LONG lFontWeight, CFont* pFont)
	{
		LOGFONT logfont;
		logfont.lfHeight = sizeFont.cx;
		logfont.lfWidth = sizeFont.cy;
		logfont.lfWeight = lFontWeight;
		logfont.lfItalic = 0;
		logfont.lfEscapement = 0;
		logfont.lfOrientation = 0;
		logfont.lfUnderline = 0;
		logfont.lfStrikeOut = 0;
		logfont.lfCharSet = DEFAULT_CHARSET;
		logfont.lfOutPrecision = OUT_TT_PRECIS;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality = DEFAULT_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

		_tcscpy(logfont.lfFaceName, strFontName);

		pFont->CreateFontIndirect(&logfont);

	}

	CFont* GetFont(INT nIndex)
	{

		switch (nIndex)
		{
		case FONT_BUTTON_LARGE:
			return m_pFontButtonLarge;

		case FONT_BUTTON_MEDIUM:
			return m_pFontButtonMedium;

		case FONT_EDIT_LARGE:
			return m_pFontEditLarge;

		case FONT_EDIT_MEDIUM:
			return m_pFontEditMedium;

		case FONT_STATIC_LARGE:
			return m_pFontStaticLarge;

		case FONT_STATIC_MEDIUM:
			return m_pFontStaticMedium;

		case FONT_STATIC_TIMER:
			return m_pFontStaticTimer;

		case FONT_EDIT_SCORE:
			return m_pFontEditScore;

		case FONT_STATIC_BOLD:
			return	m_pFontStaticBold;

		case FONT_COMBO_SMALL:
			return m_pFontComboSmall;

		default:
			return nullptr;
			break;
		}

		return nullptr;
	}
};


/////////////////////////////////////////////////////////////////////
//////////////////// CStatus ////////////////////////////////////////
class CStatus : public CClockDataObject
{
	DECLARE_DYNAMIC(CStatus)

public:
	CStatus() :
		m_pNTPConf(new CNTPConfig),
		m_pClockConf(new CClockConfig),
		m_pBigClockConf(new CBigClockConfig),
		m_pLogoConf(new CLogoConfig),
		m_pNewsConf(new CNewsConfig),
		m_pWeatherConf(new CWeatherConfig),
		m_pNoticeConf(new CNoticeConfig),
		m_pManualUpConf(new CManualUpConfig),
		m_pLiveVideoConf(new CLiveVideoConfig),
		m_pUIFonts(new CUIFonts)
	{
		m_pPtrListNews = new CPtrList();
		m_pPtrListNews->AddTail(new CNewsDataList());

		m_pPtrListWeather = new CPtrList();
		m_pPtrListWeather->AddTail(new CWeatherDataList());


		InitNoticeFieldVectors();
		m_pPtrListNotice = new CPtrList();
		m_pPtrListNotice->AddTail(new CNoticeDataList());

		SetDispStateOff();
	}

	~CStatus()
	{
		ClearPtrDataList<CNewsDataList>(m_pPtrListNews);
		ClearPtrDataList<CWeatherDataList>(m_pPtrListWeather);
		ClearPtrDataList<CNoticeDataList>(m_pPtrListNotice);

		SAFE_DELETE(m_pNTPConf);
		SAFE_DELETE(m_pClockConf);
		SAFE_DELETE(m_pBigClockConf);
		SAFE_DELETE(m_pLogoConf);
		SAFE_DELETE(m_pNewsConf);
		SAFE_DELETE(m_pWeatherConf);
		SAFE_DELETE(m_pNoticeConf);
		SAFE_DELETE(m_pManualUpConf);
		SAFE_DELETE(m_pLiveVideoConf);
		SAFE_DELETE(m_pUIFonts);
	}

public:

	CPtrList* PtrListNews() const { return m_pPtrListNews; }
	void PtrListNews(CPtrList* val) { m_pPtrListNews = val; }

	CPtrList* PtrListWeather() const { return m_pPtrListWeather; }
	void PtrListWeather(CPtrList* val) { m_pPtrListWeather = val; }

	CPtrList* PtrListNotice() const { return m_pPtrListNotice; }
	void PtrListNotice(CPtrList* val) { m_pPtrListNotice = val; }

	std::array<BOOL, MAX_DISP_MENU_NUM> m_bDispState;
	CTimeString m_TimeString;

	CNTPConfig* NTPConf() const { return m_pNTPConf; }
	void NTPConf(CNTPConfig* val) { m_pNTPConf = val; }

	CClockConfig* ClockConf() const { return m_pClockConf; }
	void ClockConf(CClockConfig* val) { m_pClockConf = val; }

	CBigClockConfig* BigClockConf() const { return m_pBigClockConf; }
	void BigClockConf(CBigClockConfig* val) { m_pBigClockConf = val; }

	CLogoConfig* LogoConf() const { return m_pLogoConf; }
	void LogoConf(CLogoConfig* val) { m_pLogoConf = val; }

	CNewsConfig* NewsConf() const { return m_pNewsConf; }
	void NewsConf(CNewsConfig* val) { m_pNewsConf = val; }

	CWeatherConfig* WeatherConf() const { return m_pWeatherConf; }
	void WeatherConf(CWeatherConfig* val) { m_pWeatherConf = val; }

	CNoticeConfig* NoticeConf() const { return m_pNoticeConf; }
	void NoticeConf(CNoticeConfig* val) { m_pNoticeConf = val; }

	CManualUpConfig* ManualUpConf() const { return m_pManualUpConf; }
	void ManualUpConf(CManualUpConfig* val) { m_pManualUpConf = val; }

	CLiveVideoConfig* LiveVideoConf() const { return m_pLiveVideoConf; }
	void LiveVideoConf(CLiveVideoConfig* val) { m_pLiveVideoConf = val; }

	CUIFonts* UIFonts() const { return m_pUIFonts; }
	void UIFonts(CUIFonts* val) { m_pUIFonts = val; }

	std::array<CClockConfig, NUM_SNAP> ClockSnaps() const { return m_ClockSnaps; }
	void ClockSnaps(std::array<CClockConfig, NUM_SNAP> val) { m_ClockSnaps = val; }

	CClockConfig& ClockSnap(INT nIndex) { return m_ClockSnaps[nIndex]; }
	void ClockSnap(CClockConfig& clockSnap, INT nIndex) { m_ClockSnaps[nIndex] = clockSnap; }

	std::array<CBigClockConfig, NUM_SNAP> BigClockSnaps() const { return m_BigClockSnaps; }
	void BigClockSnaps(std::array<CBigClockConfig, NUM_SNAP> val) { m_BigClockSnaps = val; }

	CBigClockConfig& BigClockSnap(INT nIndex) { return m_BigClockSnaps[nIndex]; }
	void BigClockSnap(CBigClockConfig& bigClockSnap, INT nIndex) { m_BigClockSnaps[nIndex] = bigClockSnap; }

	std::array<CLogoConfig, NUM_SNAP> LogoSnaps() const { return m_LogoSnaps; }
	void LogoSnaps(std::array<CLogoConfig, NUM_SNAP> val) { m_LogoSnaps = val; }

	CLogoConfig& LogoSnap(INT nIndex) { return m_LogoSnaps[nIndex]; }
	void LogoSnap(CLogoConfig& logoSnap, INT nIndex) { m_LogoSnaps[nIndex] = logoSnap; }

	std::array<CLiveVideoConfig, NUM_SNAP> LiveVideoSnaps() const { return m_LiveVideoSnaps; }
	void LiveVideoSnaps(std::array<CLiveVideoConfig, NUM_SNAP> val) { m_LiveVideoSnaps = val; }

	CLiveVideoConfig& LiveVideoSnap(INT nIndex) { return m_LiveVideoSnaps[nIndex]; }
	void LiveVideoSnap(CLiveVideoConfig& liveVideoSnap, INT nIndex) { m_LiveVideoSnaps[nIndex] = liveVideoSnap; }

	void Serialize(CArchive& ar) override 
	{
		CClockDataObject::Serialize(ar);

		if (ar.IsStoring())
		{
		}
		else
		{
		}

		m_pNTPConf->Serialize(ar);
		m_pClockConf->Serialize(ar);

		for (INT i = 0; i < NUM_SNAP; i++)
		{
			m_ClockSnaps[i].Serialize(ar);
		}
		
		m_pBigClockConf->Serialize(ar);

		for (INT i = 0; i < NUM_SNAP; i++)
		{
			m_BigClockSnaps[i].Serialize(ar);
		}


		m_pLogoConf->Serialize(ar);

		for (INT i = 0; i < NUM_SNAP; i++)
		{
			m_LogoSnaps[i].Serialize(ar);
		}
		
		m_pNewsConf->Serialize(ar);

		m_pWeatherConf->Serialize(ar);
		m_pNoticeConf->Serialize(ar);

		m_pManualUpConf->Serialize(ar);

		m_pLiveVideoConf->Serialize(ar);

		for (INT i = 0; i < NUM_SNAP; i++)
		{
			m_LiveVideoSnaps[i].Serialize(ar);
		}
				
		GetPtrDataByIndex<CNewsDataList>(m_pPtrListNews, 0)->Serialize(ar);

		GetPtrDataByIndex<CWeatherDataList>(m_pPtrListWeather, 0)->Serialize(ar);

		GetPtrDataByIndex<CNoticeDataList>(m_pPtrListNotice, 0)->Serialize(ar);
	}

	CTimeString GetTimeString() { return m_TimeString; }
	void SetTimeString(CTimeString TimeString) { m_TimeString = TimeString; }

	void SaveClockSnap(INT nIndex)
	{
		ClockSnap(*m_pClockConf, nIndex);
	}

	void LoadClockSnap(INT nIndex)
	{
		*m_pClockConf = ClockSnap(nIndex);
	}

	void SaveBigClockSnap(INT nIndex)
	{
		BigClockSnap(*m_pBigClockConf, nIndex);
	}

	void LoadBigClockSnap(INT nIndex)
	{
		*m_pBigClockConf = BigClockSnap(nIndex);
	}

	void SaveLogoSnap(INT nIndex)
	{
		LogoSnap(*m_pLogoConf, nIndex);
	}

	void LoadLogoSnap(INT nIndex)
	{
		*m_pLogoConf = LogoSnap(nIndex);
	}

	void SaveLiveVideoSnap(INT nIndex)
	{
		LiveVideoSnap(*m_pLiveVideoConf, nIndex);
	}

	void LoadLiveVideoSnap(INT nIndex)
	{
		*m_pLiveVideoConf = LiveVideoSnap(nIndex);
	}

	CWeatherDataList* GetWeatherList(void) { return GetPtrDataByIndex<CWeatherDataList>(m_pPtrListWeather, 0); }
	CNewsDataList* GetNewsList(void) { return GetPtrDataByIndex<CNewsDataList>(m_pPtrListNews, 0); }
	CNoticeDataList* GetNoticeList(void) { return GetPtrDataByIndex<CNoticeDataList>(m_pPtrListNotice, 0); }

	BOOL GetDispState(int nDispState) { return m_bDispState[nDispState]; }

	BOOL IsOnAir(void)
	{
		for (int i = 0; i < MAX_DISP_MENU_NUM; i++)
		{
			if (m_bDispState[i] == TRUE) return TRUE;
		}
		return FALSE;
	}

	void SetDispState(int nDispState, BOOL bValue)
	{
		for (auto& iter : m_bDispState)
		{
			iter = FALSE;
		}
		m_bDispState[nDispState] = bValue;
	}

	void SetDispStateForPage(int nDispState, BOOL bValue, INT nPageNumber)
	{
		switch (nPageNumber)
		{
		case MANUALUP_PAGE_NUMBER:
			for (int i = DISP_MANUALUP; i < DISP_NEWS; i++)
			{
				m_bDispState[i] = FALSE;
			}
			break;

		case NEWS_PAGE_NUMBER:
			for (int i = DISP_NEWS; i < DISP_WEATHER; i++)
			{
				m_bDispState[i] = FALSE;
			}
			break;

		case WEATHER_PAGE_NUMBER:
			for (int i = DISP_WEATHER; i < DISP_BIG_CLOCK; i++)
			{
				m_bDispState[i] = FALSE;
			}
			break;


		case CLOCK_PAGE_NUMBER:
			for (int i = DISP_BIG_CLOCK; i < DISP_STATION_LOGO; i++)
			{
				m_bDispState[i] = FALSE;
			}
			break;

		case LOGO_PAGE_NUMBER:

			for (int i = DISP_STATION_LOGO; i < DISP_NOTICE; i++)
			{
				m_bDispState[i] = FALSE;
			}
			break;

		case NOTICE_PAGE_NUMBER:
		case AGE_PAGE_NUMBER:
			for (int i = DISP_NOTICE; i < DISP_LIVE_VIDEO; i++)
			{
				m_bDispState[i] = FALSE;
			}
			break;  

		
			//for (int i = DISP_AGE; i < DISP_LIVE_VIDEO; i++)
			//{
			//	m_bDispState[i] = FALSE;
			//}
			//break;

		case LIVE_VIDEO_PAGE_NUMBER:
			for (int i = DISP_LIVE_VIDEO; i < MAX_DISP_MENU_NUM; i++)
			{
				m_bDispState[i] = FALSE;
			}
			break;


		default:
			break;

		}
		m_bDispState[nDispState] = bValue;
	}
	void SetDispStateOn(void)
	{
		for (auto& iter : m_bDispState)
		{
			iter = TRUE;
		}
	}
	void SetDispStateOff(void)
	{
		for (auto& iter : m_bDispState)
		{
			iter = FALSE;
		}
	}

private:
	CNTPConfig* m_pNTPConf;
	CClockConfig* m_pClockConf;
	CBigClockConfig* m_pBigClockConf;
	CLogoConfig* m_pLogoConf;
	CNewsConfig* m_pNewsConf;
	CWeatherConfig* m_pWeatherConf;
	CNoticeConfig* m_pNoticeConf;
	CManualUpConfig* m_pManualUpConf;
	CLiveVideoConfig* m_pLiveVideoConf;
	CUIFonts* m_pUIFonts;

	std::array<CClockConfig, NUM_SNAP> m_ClockSnaps;
	std::array<CBigClockConfig, NUM_SNAP> m_BigClockSnaps;
	std::array<CLogoConfig, NUM_SNAP> m_LogoSnaps;
	std::array<CLiveVideoConfig, NUM_SNAP> m_LiveVideoSnaps;

	CPtrList* m_pPtrListNews;
	CPtrList* m_pPtrListWeather;
	CPtrList* m_pPtrListNotice;

public:
	CString m_strFileName;
	CString m_strFPRFileName;


};

