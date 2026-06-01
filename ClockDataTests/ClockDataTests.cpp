// ClockDataTests.cpp: unit tests for ClockData
#include "pch.h"
#include "CppUnitTest.h"
#include "ClockData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClockDataTests
{
    // =========================================================
    // CCharInfo 테스트
    // =========================================================
    TEST_CLASS(CCharInfoTest)
    {
    public:
        TEST_METHOD(DefaultConstructor_InitialValues)
        {
            CCharInfo ci;
            Assert::AreEqual(CString(BLANK_STRING), ci.Char());
            Assert::AreEqual(0, ci.Size());
            Assert::AreEqual((ULONG)0, ci.Color());
            Assert::AreEqual(CString(BLANK_STRING), ci.Font());
            Assert::AreEqual((BYTE)129, ci.CharSet());
        }

        TEST_METHOD(SettersGetters_WorkCorrectly)
        {
            CCharInfo ci;
            ci.Char(_T("가"));
            ci.Size(100);
            ci.Color(0xFF0000);
            ci.Font(_T("맑은고딕"));
            ci.CharSet(129);

            Assert::AreEqual(CString(_T("가")), ci.Char());
            Assert::AreEqual(100, ci.Size());
            Assert::AreEqual((ULONG)0xFF0000, ci.Color());
            Assert::AreEqual(CString(_T("맑은고딕")), ci.Font());
            Assert::AreEqual((BYTE)129, ci.CharSet());
        }

        TEST_METHOD(EqualityOperator_SameValues_ReturnsTrue)
        {
            CCharInfo a, b;
            a.Char(_T("A")); a.Size(50); a.Color(0x00FF00); a.Font(_T("Arial")); a.CharSet(0);
            b.Char(_T("A")); b.Size(50); b.Color(0x00FF00); b.Font(_T("Arial")); b.CharSet(0);

            Assert::IsTrue((bool)(a == b));
        }

        TEST_METHOD(EqualityOperator_DifferentChar_ReturnsFalse)
        {
            CCharInfo a, b;
            a.Char(_T("A")); b.Char(_T("B"));
            Assert::IsTrue((bool)(a != b));
        }

        TEST_METHOD(CopyConstructor_CopiesAllFields)
        {
            CCharInfo original;
            original.Char(_T("테"));
            original.Size(80);
            original.Color(0x123456);
            original.Font(_T("굴림"));
            original.CharSet(129);

            CCharInfo copy(original);
            Assert::IsTrue((bool)(copy == original));
        }

        TEST_METHOD(IsImageChar_WithImageTag_ReturnsTrue)
        {
            CCharInfo ci;
            ci.Font(IMAGE_FILE_TAG);
            Assert::IsTrue(ci.IsImageChar());
        }

        TEST_METHOD(IsImageChar_WithTextFont_ReturnsFalse)
        {
            CCharInfo ci;
            ci.Font(_T("맑은고딕"));
            Assert::IsFalse(ci.IsImageChar());
        }

        TEST_METHOD(Clear_ResetsToDefault)
        {
            CCharInfo ci;
            ci.Char(_T("X")); ci.Size(99); ci.Color(0xABCDEF);
            ci.Clear();

            Assert::AreEqual(CString(BLANK_STRING), ci.Char());
            Assert::AreEqual(0, ci.Size());
            Assert::AreEqual((ULONG)0, ci.Color());
        }
    };

    // =========================================================
    // CCharInfoList 테스트
    // =========================================================
    TEST_CLASS(CCharInfoListTest)
    {
    public:
        TEST_METHOD(IsEmpty_OnNewList_ReturnsTrue)
        {
            CCharInfoList list;
            Assert::IsTrue(list.IsEmpty());
        }

        TEST_METHOD(AddCharInfo_IncreasesSize)
        {
            CCharInfoList list;
            CCharInfo ci;
            ci.Char(_T("가")); ci.Font(_T("굴림"));
            list.AddCharInfo(ci);
            Assert::AreEqual(1, list.Size());
        }

        TEST_METHOD(SetAndGetCharInfoString_RoundTrip)
        {
            CCharInfoList list;
            CString strContents = _T("가") + CString(CHARINFO_DELIMITER) + _T("나");
            CString strSizes    = _T("100") + CString(CHARINFO_DELIMITER) + _T("80");
            CString strColors   = _T("255") + CString(CHARINFO_DELIMITER) + _T("128");
            CString strFonts    = _T("굴림") + CString(CHARINFO_DELIMITER) + _T("굴림");
            CString strCharSets = _T("129") + CString(CHARINFO_DELIMITER) + _T("129");

            list.SetCharInfoString(strContents, strSizes, strColors, strFonts, strCharSets);

            Assert::AreEqual(2, list.Size());

            CString outContents, outSizes, outColors, outFonts, outCharSets;
            list.GetCharInfoString(outContents, outSizes, outColors, outFonts, outCharSets);

            Assert::AreEqual(strContents, outContents);
            Assert::AreEqual(strSizes, outSizes);
        }

        TEST_METHOD(CalcMixed_AllSameFont_ReturnsFalse)
        {
            CCharInfoList list;
            for (int i = 0; i < 3; i++)
            {
                CCharInfo ci;
                ci.Char(_T("A")); ci.Font(_T("굴림")); ci.Size(100); ci.Color(0xFFFFFF);
                list.AddCharInfo(ci);
            }
            Assert::IsFalse((bool)list.CalcMixed());
        }

        TEST_METHOD(CalcMixed_DifferentFont_ReturnsTrue)
        {
            CCharInfoList list;
            CCharInfo a; a.Char(_T("A")); a.Font(_T("굴림")); a.Size(100); a.Color(0xFFFFFF);
            CCharInfo b; b.Char(_T("B")); b.Font(_T("Arial")); b.Size(100); b.Color(0xFFFFFF);
            list.AddCharInfo(a);
            list.AddCharInfo(b);
            Assert::IsTrue((bool)list.CalcMixed());
        }

        TEST_METHOD(CalcMixed_ImageFont_ReturnsTrue)
        {
            CCharInfoList list;
            CCharInfo ci;
            ci.Char(_T("img.png")); ci.Font(IMAGE_FILE_TAG); ci.Size(100); ci.Color(0);
            list.AddCharInfo(ci);
            Assert::IsTrue((bool)list.CalcMixed());
        }

        TEST_METHOD(Remove_RemovesCorrectRange)
        {
            CCharInfoList list;
            for (int i = 0; i < 5; i++)
            {
                CCharInfo ci;
                CString ch; ch.Format(_T("%d"), i);
                ci.Char(ch); ci.Font(_T("굴림"));
                list.AddCharInfo(ci);
            }
            list.Remove(1, 2); // 인덱스 1부터 2개 제거 → [0, 3, 4]
            Assert::AreEqual(3, list.Size());
            Assert::AreEqual(CString(_T("0")), list.GetAt(0).Char());
            Assert::AreEqual(CString(_T("3")), list.GetAt(1).Char());
        }

        TEST_METHOD(Mid_ExtractsSubrange)
        {
            CCharInfoList list;
            for (int i = 0; i < 5; i++)
            {
                CCharInfo ci;
                CString ch; ch.Format(_T("%d"), i);
                ci.Char(ch); ci.Font(_T("굴림"));
                list.AddCharInfo(ci);
            }
            CCharInfoList sub;
            list.Mid(1, 3, sub); // 인덱스 1~2
            Assert::AreEqual(2, sub.Size());
            Assert::AreEqual(CString(_T("1")), sub.GetAt(0).Char());
            Assert::AreEqual(CString(_T("2")), sub.GetAt(1).Char());
        }

        TEST_METHOD(Clear_EmptiesList)
        {
            CCharInfoList list;
            CCharInfo ci; ci.Char(_T("X")); ci.Font(_T("굴림"));
            list.AddCharInfo(ci);
            list.Clear();
            Assert::IsTrue(list.IsEmpty());
        }
    };

    // =========================================================
    // CWeatherData 테스트
    // =========================================================
    TEST_CLASS(CWeatherDataTest)
    {
    public:
        TEST_METHOD(MakeWeatherCode_FormatsCorrectly)
        {
            CWeatherData wd;
            CString result = wd.MakeWeatherCode(_T("DW001"));
            Assert::AreEqual(CString(_T("DW001")), result);

            result = wd.MakeWeatherCode(_T("DW010"));
            Assert::AreEqual(CString(_T("DW010")), result);
        }

        TEST_METHOD(IsValidWeatherCode_ValidCode_ReturnsTrue)
        {
            CWeatherData wd;
            Assert::IsTrue((bool)wd.IsValidWeatherCode(_T("DW001")));
            Assert::IsTrue((bool)wd.IsValidWeatherCode(_T("DW022")));
        }

        TEST_METHOD(IsValidWeatherCode_InvalidCode_ReturnsFalse)
        {
            CWeatherData wd;
            Assert::IsFalse((bool)wd.IsValidWeatherCode(_T("DW000")));
            Assert::IsFalse((bool)wd.IsValidWeatherCode(_T("DW003"))); // 코드 3은 미사용
            Assert::IsFalse((bool)wd.IsValidWeatherCode(_T("DW024")));
        }

        TEST_METHOD(DecisionAirLevel_GoodRange)
        {
            CWeatherData wd;
            CString level = wd.DecisionAirLevel(_T("10.0"));
            Assert::AreEqual(CString(_T("좋음")), level);
        }

        TEST_METHOD(DecisionAirLevel_NormalRange)
        {
            CWeatherData wd;
            CString level = wd.DecisionAirLevel(_T("25.0"));
            Assert::AreEqual(CString(_T("보통")), level);
        }

        TEST_METHOD(DecisionAirLevel_BadRange)
        {
            CWeatherData wd;
            CString level = wd.DecisionAirLevel(_T("50.0"));
            Assert::AreEqual(CString(_T("나쁨")), level);
        }

        TEST_METHOD(DecisionAirLevel_VeryBadRange)
        {
            CWeatherData wd;
            CString level = wd.DecisionAirLevel(_T("80.0"));
            Assert::AreEqual(CString(_T("매우나쁨")), level);
        }

        TEST_METHOD(DecisionAirLevel_NegativeValue_ReturnsNone)
        {
            CWeatherData wd;
            CString level = wd.DecisionAirLevel(_T("-1.0"));
            Assert::AreEqual(CString(_T("")), level);
        }

        TEST_METHOD(IsEmptyWeather_EmptyCode_ReturnsTrue)
        {
            CWeatherData wd;
            Assert::IsTrue((bool)wd.IsEmptyWeather());
        }
    };

    // =========================================================
    // CWeatherDataList 헬퍼 테스트
    // =========================================================
    TEST_CLASS(CWeatherDataListHelperTest)
    {
    public:
        TEST_METHOD(CalcAvg_NormalValues_ReturnsCorrectAverage)
        {
            CWeatherDataList wdl;
            std::vector<int> values = { 10, 20, 30 };
            float avg = wdl.CalcAvg(values);
            Assert::AreEqual(20.0f, avg, 0.001f);
        }

        TEST_METHOD(CalcAvg_AllInvalid_ReturnsInvalidValue)
        {
            CWeatherDataList wdl;
            std::vector<int> values = { INVALID_PM25VALUE, INVALID_PM25VALUE };
            float avg = wdl.CalcAvg(values);
            Assert::AreEqual((float)INVALID_PM25VALUE, avg, 0.001f);
        }

        TEST_METHOD(CalcAvg_MixedWithInvalid_IgnoresInvalid)
        {
            CWeatherDataList wdl;
            std::vector<int> values = { INVALID_PM25VALUE, 10, 30 };
            float avg = wdl.CalcAvg(values);
            Assert::AreEqual(20.0f, avg, 0.001f);
        }

        TEST_METHOD(IsJsonFormatsSimple_ValidObject_ReturnsTrue)
        {
            CWeatherDataList wdl;
            Assert::IsTrue(wdl.IsJsonFormatsSimple(_T("{\"key\":\"value\"}")));
        }

        TEST_METHOD(IsJsonFormatsSimple_ValidArray_ReturnsTrue)
        {
            CWeatherDataList wdl;
            Assert::IsTrue(wdl.IsJsonFormatsSimple(_T("[1,2,3]")));
        }

        TEST_METHOD(IsJsonFormatsSimple_EmptyString_ReturnsFalse)
        {
            CWeatherDataList wdl;
            Assert::IsFalse(wdl.IsJsonFormatsSimple(_T("")));
        }

        TEST_METHOD(IsJsonFormatsSimple_HtmlString_ReturnsFalse)
        {
            CWeatherDataList wdl;
            Assert::IsFalse(wdl.IsJsonFormatsSimple(_T("<html><body>error</body></html>")));
        }

        TEST_METHOD(DecisionAirLevel_BoundaryValues)
        {
            CWeatherDataList wdl;
            Assert::IsTrue(wdl.DecisionAirLevel(0.0f)   == AIR_LEVEL::GOOD);
            Assert::IsTrue(wdl.DecisionAirLevel(15.0f)  == AIR_LEVEL::GOOD);
            Assert::IsTrue(wdl.DecisionAirLevel(15.1f)  == AIR_LEVEL::NOMAL);
            Assert::IsTrue(wdl.DecisionAirLevel(35.0f)  == AIR_LEVEL::NOMAL);
            Assert::IsTrue(wdl.DecisionAirLevel(35.1f)  == AIR_LEVEL::BAD);
            Assert::IsTrue(wdl.DecisionAirLevel(75.0f)  == AIR_LEVEL::BAD);
            Assert::IsTrue(wdl.DecisionAirLevel(75.1f)  == AIR_LEVEL::VERY_BAD);
            Assert::IsTrue(wdl.DecisionAirLevel(-1.0f)  == AIR_LEVEL::NONE);
        }
    };

    // =========================================================
    // CStatus 표출 상태 테스트
    // =========================================================
    TEST_CLASS(CStatusDispStateTest)
    {
    public:
        TEST_METHOD(InitialState_IsNotOnAir)
        {
            InitNoticeFieldVectors();
            CStatus status;
            Assert::IsFalse((bool)status.IsOnAir());
        }

        TEST_METHOD(SetDispState_TurnsOnCorrectState)
        {
            InitNoticeFieldVectors();
            CStatus status;
            status.SetDispState(DISP_NEWS, TRUE);
            Assert::IsTrue((bool)status.GetDispState(DISP_NEWS));
            Assert::IsTrue((bool)status.IsOnAir());
        }

        TEST_METHOD(SetDispState_TurnsOffOtherStates)
        {
            InitNoticeFieldVectors();
            CStatus status;
            status.SetDispState(DISP_NEWS, TRUE);
            status.SetDispState(DISP_WEATHER, TRUE);
            // SetDispState는 모두 끄고 하나만 켬
            Assert::IsFalse((bool)status.GetDispState(DISP_NEWS));
            Assert::IsTrue((bool)status.GetDispState(DISP_WEATHER));
        }

        TEST_METHOD(SetDispStateOff_AllOff)
        {
            InitNoticeFieldVectors();
            CStatus status;
            status.SetDispState(DISP_NEWS, TRUE);
            status.SetDispStateOff();
            Assert::IsFalse((bool)status.IsOnAir());
        }

        TEST_METHOD(SetDispStateOn_AllOn)
        {
            InitNoticeFieldVectors();
            CStatus status;
            status.SetDispStateOn();
            Assert::IsTrue((bool)status.IsOnAir());
            for (int i = 0; i < MAX_DISP_MENU_NUM; i++)
            {
                Assert::IsTrue((bool)status.GetDispState(i));
            }
        }
    };

    // =========================================================
    // CNoticeData 별칭 테스트
    // =========================================================
    TEST_CLASS(CNoticeDataAliasTest)
    {
    public:
        TEST_METHOD(SetAndGetAlias_WorksCorrectly)
        {
            InitNoticeFieldVectors();
            CNoticeData nd;
            nd.SetAlias(0, _T("제목"));
            Assert::AreEqual(CString(_T("제목")), nd.GetAlias(0));
        }

        TEST_METHOD(SetAndGetAliasValue_WorksCorrectly)
        {
            InitNoticeFieldVectors();
            CNoticeData nd;
            nd.SetAlias(0, _T("제목"));
            nd.SetAliasValue(0, _T("KBS 뉴스"));
            Assert::AreEqual(CString(_T("KBS 뉴스")), nd.GetAliasValue(0));
        }

        TEST_METHOD(GetAliasIndex_FindsCorrectIndex)
        {
            InitNoticeFieldVectors();
            CNoticeData nd;
            nd.SetAlias(0, _T("제목"));
            nd.SetAlias(1, _T("내용"));
            CString strAlias = _T("내용");
            Assert::AreEqual(1, nd.GetAliasIndex(strAlias));
        }

        TEST_METHOD(GetAliasIndex_NotFound_ReturnsMinusOne)
        {
            InitNoticeFieldVectors();
            CNoticeData nd;
            CString strAlias = _T("없는별칭");
            Assert::AreEqual(-1, nd.GetAliasIndex(strAlias));
        }

        TEST_METHOD(ClearAliasAndValue_ClearsAll)
        {
            InitNoticeFieldVectors();
            CNoticeData nd;
            nd.SetAlias(0, _T("제목"));
            nd.SetAliasValue(0, _T("KBS"));
            nd.ClearAliasAndValue();
            Assert::AreEqual(CString(BLANK_STRING), nd.GetAlias(0));
            Assert::AreEqual(CString(BLANK_STRING), nd.GetAliasValue(0));
        }

        TEST_METHOD(CountAlias_CountsNonEmptyNonAgeAliases)
        {
            InitNoticeFieldVectors();
            CNoticeData nd;
            nd.SetAlias(0, _T("제목"));
            nd.SetAlias(1, _T("내용"));
            nd.SetAlias(2, AGE_ALIAS); // 연령은 카운트에서 제외
            Assert::AreEqual(2, nd.CountAlias());
        }

        TEST_METHOD(OutOfRangeIndex_ReturnsBlank)
        {
            InitNoticeFieldVectors();
            CNoticeData nd;
            Assert::AreEqual(CString(BLANK_STRING), nd.GetAlias(-1));
            Assert::AreEqual(CString(BLANK_STRING), nd.GetAlias(MAX_ALIAS_NUM));
        }
    };

    // =========================================================
    // CClockConfig 스냅샷 테스트
    // =========================================================
    TEST_CLASS(CClockConfigTest)
    {
    public:
        TEST_METHOD(AssignmentOperator_CopiesAllFields)
        {
            CClockConfig src, dst;
            src.Title(_T("테스트시계"));
            src.B24Hour(TRUE);
            src.DispSecond(TRUE);
            src.MovingSize(10);
            src.HideFirstDigit0OfHour(FALSE);

            dst = src;

            Assert::AreEqual(CString(_T("테스트시계")), dst.Title());
            Assert::IsTrue((bool)dst.B24Hour());
            Assert::IsTrue((bool)dst.DispSecond());
            Assert::AreEqual(10, dst.MovingSize());
            Assert::IsFalse((bool)dst.HideFirstDigit0OfHour());
        }

        TEST_METHOD(DefaultFormat_IsSet)
        {
            CClockConfig cfg;
            Assert::AreEqual(CString(DEFAULT_DATE_FORMAT), cfg.Format());
        }
    };
}
