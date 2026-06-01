#pragma once
#ifndef PCH_H
#define PCH_H

#define _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR
#define NOMINMAX

#include "..\framework.h"
#include "CppUnitTest.h"

// CString → wstring 변환 특수화 (Assert::AreEqual 등에서 사용)
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<CString>(const CString& q)
    {
        return std::wstring(q.GetString());
    }
}

#endif // PCH_H